#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace {
    using namespace GW;

    typedef void (__fastcall *SendUIMessage_pt)(uint32_t message, void *wParam, void *lParam);
    SendUIMessage_pt SendUIMessage_Func;

    typedef void (__fastcall *LoadSettings_pt)(uint32_t size, uint8_t *data);
    LoadSettings_pt LoadSettings_Func;

    uintptr_t GameSettings_Addr;
    uintptr_t ui_drawn_addr;
    uintptr_t shift_screen_addr;

    struct AsyncBuffer {
        void *buffer;
        size_t size;
    };

    void __fastcall __callback_copy_char(void *param, const wchar_t *s) {
        assert(param && s);
        AsyncBuffer *abuf = (AsyncBuffer *)param;
        char *outstr = (char *)abuf->buffer;
        for (size_t i = 0; i < abuf->size; i++) {
            outstr[i] = s[i] & 0x7F;
            if (!s[i]) break;
        }
        delete abuf;
    }

    void __fastcall __callback_copy_wchar(void *param, const wchar_t *s) {
        assert(param && s);
        AsyncBuffer *abuf = (AsyncBuffer *)param;
        wcsncpy((wchar_t *)abuf->buffer, s, abuf->size);
        delete abuf;
    }

    void __fastcall __calback_copy_wstring(void *param, const wchar_t *s) {
        assert(param && s);
        std::wstring *str = (std::wstring *)param;
        *str = s;
    }

    typedef void (__fastcall *DecodeStr_Callback)(void *param, const wchar_t *s);
    void AsyncDecodeStr(const wchar_t *enc_str, DecodeStr_Callback callback, void *param) {
        typedef void(__fastcall *AsyncDecodeStr_t)(const wchar_t *s, DecodeStr_Callback cb, void *param);
        AsyncDecodeStr_t Gw_AsyncDecodeStr = (AsyncDecodeStr_t)MemoryMgr::AsyncDecodeStringPtr;
        assert(enc_str && Gw_AsyncDecodeStr && callback);
        Gw_AsyncDecodeStr(enc_str, callback, param);
    }

    void Init() {
        SendUIMessage_Func = (SendUIMessage_pt)Scanner::Find(
            "\x8B\xDA\x1B\xF6\xF7\xDE\x4E\x83\xFF\x40\x73\x14\x68", "xxxxxxxxxxxxx", -0xB);
        printf("[SCAN] SendUIMessage = %p\n", SendUIMessage_Func);

        LoadSettings_Func = (LoadSettings_pt)Scanner::Find(
            "\x33\xC9\xE8\x00\x00\x00\x00\x5F\x5E\xE9", "xxx????xxx", -37);
        printf("[SCAN] LoadSettings = %p\n", LoadSettings_Func);

        {
            uintptr_t address = Scanner::Find("\x57\x77\x04\x8B\xF8\xEB\x18", "xxxxxxx", +15);
            printf("[SCAN] GameSettings = %p\n", (void *)address);
            if (Verify(address))
                GameSettings_Addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find("\x85\xC0\x74\x58\x5F\x5E\xE9", "xxxxxxx", +16);
            printf("[SCAN] ui_drawn_addr = %p\n", (void *)address);
            if (Verify(address))
                ui_drawn_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\x85\xC0\x0F\x85\x00\x00\x00\x00\x85\xC9\x75\x00", "xxxx????xxx?", +14);
            printf("[SCAN] shift_screen_addr = %p\n", (void *)address);
            shift_screen_addr = *(uintptr_t *)address;
        }
    }
}

namespace GW {

    Module UIModule = {
        "UIModule",     // name
        NULL,           // param
        NULL,           // init_module
        NULL,           // exit_module
        NULL,           // exit_module
        NULL,           // remove_hooks
    };

    void UI::SendUIMessage(unsigned message, unsigned int wParam, int lParam)
    {
        if (Verify(SendUIMessage_Func))
            SendUIMessage_Func(message, (void *)wParam, (void *)lParam);
    }

    void UI::SendUIMessage(unsigned message, void *wParam, void *lParam)
    {
        if (Verify(SendUIMessage_Func))
            SendUIMessage_Func(message, (void *)wParam, (void *)lParam);
    }

    void UI::DrawOnCompass(unsigned session_id, unsigned pt_count, CompassPoint *pts)
    {
        struct P037 {                   // Used to send pings and drawings in the minimap. Related to StoC::P133
            const unsigned header = CtoGS_MSGDrawMap;
            unsigned session_id = 0;     // unique for every player and shape. changes for every ping or shape.
            unsigned pt_count;           // number of points in the following array
            CompassPoint pts[8]; // in world coordinates divided by 100
            unsigned unk[8];
        };
        static P037 p;
        p.session_id = session_id;
        p.pt_count = pt_count;
        for (unsigned i = 0; i < pt_count; ++i)
            p.pts[i] = pts[i];
        CtoS::SendPacket(&p);
    }

    void UI::LoadSettings(size_t size, uint8_t *data) {
        if (Verify(LoadSettings_Func))
            LoadSettings_Func(size, data);
    }

    UI::ArrayByte UI::GetSettings() {
        ArrayByte *GameSettings = (ArrayByte *)GameSettings_Addr;
        if (Verify(GameSettings))
            return *GameSettings;
        else
            return ArrayByte();
    }

    bool UI::GetIsUIDrawn() {
        uint32_t *ui_drawn = (uint32_t *)ui_drawn_addr;
        if (Verify(ui_drawn))
            return (*ui_drawn == 0);
        else
            return true;
    }

    bool UI::GetIsShiftScrennShot() {
        uint32_t *shift_screen = (uint32_t *)shift_screen_addr;
        if (Verify(shift_screen))
            return (*shift_screen != 0);
        else
            return false;
    }

    void UI::AsyncDecodeStr(const wchar_t *enc_str, wchar_t *buffer, size_t size) {
        // @Enhancement: Should use a pool of this buffer, but w/e for now
        AsyncBuffer *abuf = new AsyncBuffer;
        abuf->buffer = buffer;
        abuf->size = size;
        ::AsyncDecodeStr(enc_str, __callback_copy_wchar, abuf);
    }

    void UI::AsyncDecodeStr(const wchar_t *enc_str, char *buffer, size_t size) {
        // @Enhancement: Should use a pool of this buffer, but w/e for now
        AsyncBuffer *abuf = new AsyncBuffer;
        abuf->buffer = buffer;
        abuf->size = size;
        ::AsyncDecodeStr(enc_str, __callback_copy_char, abuf);
    }

    void UI::AsyncDecodeStr(const wchar_t *enc_str, std::wstring *out) {
        ::AsyncDecodeStr(enc_str, __calback_copy_wstring, out);
    }

    #define WORD_BIT_MORE       (0x8000)
    #define WORD_VALUE_BASE     (0x100)
    #define WORD_VALUE_RANGE    (WORD_BIT_MORE - WORD_VALUE_BASE)
    bool UI::UInt32ToEncStr(uint32_t value, wchar_t *buffer, size_t count) {
        // Each "case" in the array of wchar_t contains a value in the range [0, WORD_VALUE_RANGE)
        // This value is offseted by WORD_VALUE_BASE and if it take more than 1 "case" it set the bytes WORD_BIT_MORE
        size_t case_required = (value + WORD_VALUE_RANGE - 1) / WORD_VALUE_RANGE;
        if (case_required + 1 > count)
            return false;
        buffer[case_required] = 0;
        for (size_t i = case_required - 1; i < case_required; i--) {
            buffer[i] = WORD_VALUE_BASE + (value % WORD_VALUE_RANGE);
            value /= WORD_VALUE_RANGE;
            if (i != case_required - 1)
                buffer[i] |= WORD_BIT_MORE;
        }
        return true;
    }

    uint32_t UI::EncStrToUInt32(const wchar_t *enc_str) {
        uint32_t val = 0;
        do {
            assert(*enc_str >= WORD_VALUE_BASE);
            val *= WORD_VALUE_RANGE;
            val += (*enc_str & ~WORD_BIT_MORE) - WORD_VALUE_BASE;
        } while (*enc_str++ & WORD_BIT_MORE);
        return val;
    }
} // namespace GW
