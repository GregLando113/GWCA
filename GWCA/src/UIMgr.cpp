#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace {
    typedef void __fastcall GwSendUIMessage_t(unsigned message, void* wParam, void* lParam);
    GwSendUIMessage_t* g_uiSendMessage = nullptr;

    typedef void __fastcall GwLoadSettings_t(uint32_t size, uint8_t *data);
    GwLoadSettings_t *GwLoadSettings = nullptr;

    GW::UI::ArrayByte *GwSettings = nullptr;

    DWORD *gw_ui_drawn = nullptr;
    DWORD *gw_shift_screen = nullptr;

    void Initialize() {
        g_uiSendMessage = (GwSendUIMessage_t *)GW::Scanner::Find(
            "\x8B\xDA\x1B\xF6\xF7\xDE\x4E\x83\xFF\x40\x73\x14\x68", 
            "xxxxxxxxxxxxx", -0xB);
        printf("[SCAN] UI::SendUIMessage = %p\n", g_uiSendMessage);

        GwLoadSettings = (GwLoadSettings_t *)GW::Scanner::Find("\x33\xC9\xE8\x00\x00\x00\x00\x5F\x5E\xE9", "xxx????xxx", -37);
        printf("[SCAN] UI::LoadSettings = %p\n", GwLoadSettings);

        {
            uintptr_t temp = GW::Scanner::Find("\x57\x77\x04\x8B\xF8\xEB\x18", "xxxxxxx", +15);
            printf("[SCAN] UI::GwSettings = %p\n", (void *)temp);
            GwSettings = *(GW::UI::ArrayByte **)temp;
        }

        {
            uintptr_t temp = GW::Scanner::Find("\x85\xC0\x74\x58\x5F\x5E\xE9", "xxxxxxx", +16);
            printf("[SCAN] UI::gw_ui_drawn = %p\n", (void *)temp);
            gw_ui_drawn = *(DWORD **)temp;
        }

        {
            uintptr_t temp = GW::Scanner::Find("\x85\xC0\x0F\x85\x00\x00\x00\x00\x85\xC9\x75\x00", "xxxx????xxx?", +14);
            printf("[SCAN] UI::gw_shift_screen = %p\n", (void *)temp);
            gw_shift_screen = *(DWORD **)temp;
        }
    }
}


void GW::UI::SendUIMessage(unsigned message, unsigned int wParam, int lParam)
{
    if (!g_uiSendMessage) {
        ::Initialize();
    }
    g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}

void GW::UI::SendUIMessage(unsigned message, void * wParam, void * lParam)
{
    if (!g_uiSendMessage) {
        ::Initialize();
    }
    g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}

void GW::UI::DrawOnCompass(unsigned sessionid, unsigned ptcount, CompassPoint pts[8])
{
    struct P037 {                   // Used to send pings and drawings in the minimap. Related to StoC::P133
        const unsigned header = CtoGS_MSGDrawMap;
        unsigned sessionid = 0;     // unique for every player and shape. changes for every ping or shape.
        unsigned ptcount;           // number of points in the following array
        CompassPoint pts[8]; // in world coordinates divided by 100
        unsigned unk[8];
    };
    static P037 p = {};
    p.sessionid = sessionid;
    p.ptcount = ptcount;
    for (unsigned i = 0; i < ptcount; ++i)
        p.pts[i] = pts[i];
    CtoS::SendPacket(&p);
}

void GW::UI::LoadSettings(size_t size, uint8_t *data) {
    if (!GwLoadSettings) {
        ::Initialize();
    }
    GwLoadSettings(size, data);
}

GW::UI::ArrayByte GW::UI::GetSettings() {
    if (!GwSettings) {
        ::Initialize();
    }
    return *GwSettings;
}

bool GW::UI::GetIsUIDrawn() {
    if (!gw_ui_drawn) {
        ::Initialize();
    }
    return (*gw_ui_drawn == 0);
}

bool GW::UI::GetIsShiftScrennShot() {
    if (!gw_shift_screen) {
        ::Initialize();
    }
    return (*gw_shift_screen != 0);
}

namespace {
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
        AsyncDecodeStr_t Gw_AsyncDecodeStr = (AsyncDecodeStr_t)GW::MemoryMgr::AsyncDecodeStringPtr;
        assert(enc_str && Gw_AsyncDecodeStr && callback);
        Gw_AsyncDecodeStr(enc_str, callback, param);
    }
}

void GW::UI::AsyncDecodeStr(const wchar_t *enc_str, wchar_t *buffer, size_t size) {
    // @Enhancement: Should use a pool of this buffer, but w/e for now
    AsyncBuffer *abuf = new AsyncBuffer;
    abuf->buffer = buffer;
    abuf->size = size;
    ::AsyncDecodeStr(enc_str, __callback_copy_wchar, abuf);
}

void GW::UI::AsyncDecodeStr(const wchar_t *enc_str, char *buffer, size_t size) {
    // @Enhancement: Should use a pool of this buffer, but w/e for now
    AsyncBuffer *abuf = new AsyncBuffer;
    abuf->buffer = buffer;
    abuf->size = size;
    ::AsyncDecodeStr(enc_str, __callback_copy_char, abuf);
}

void GW::UI::AsyncDecodeStr(const wchar_t *enc_str, std::wstring *out) {
    ::AsyncDecodeStr(enc_str, __calback_copy_wstring, out);
}

#define WORD_BIT_MORE       (0x8000)
#define WORD_VALUE_BASE     (0x100)
#define WORD_VALUE_RANGE    (WORD_BIT_MORE - WORD_VALUE_BASE)
bool GW::UI::UInt32ToEncStr(uint32_t value, wchar_t *buffer, size_t count) {
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

uint32_t GW::UI::EncStrToUInt32(const wchar_t *enc_str) {
    uint32_t val = 0;
    do {
        assert(*enc_str >= WORD_VALUE_BASE);
        val *= WORD_VALUE_RANGE;
        val += (*enc_str & ~WORD_BIT_MORE) - WORD_VALUE_BASE;
    } while (*enc_str++ & WORD_BIT_MORE);
    return val;
}