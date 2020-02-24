#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
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

    typedef void (__cdecl *SendUIMessage_pt)(uint32_t msgid, void *wParam, void *lParam);
    SendUIMessage_pt SendUIMessage_Func;
    SendUIMessage_pt RetSendUIMessage;

    typedef void (__cdecl *LoadSettings_pt)(uint32_t size, uint8_t *data);
    LoadSettings_pt LoadSettings_Func;

    bool open_links = false;
    HookEntry open_template_hook;

    uintptr_t GameSettings_Addr;
    uintptr_t ui_drawn_addr;
    uintptr_t shift_screen_addr;
    uintptr_t AsyncDecodeStringPtr;
    uint32_t *preferences_array;

    static void OnOpenTemplate(HookStatus *hook_status, uint32_t msgid, void *wParam, void *lParam)
    {
        if (msgid != UI::kOpenTemplate)
            return;
        UI::ChatTemplate *info = static_cast<UI::ChatTemplate *>(wParam);
        if (!(open_links && info && info->code.valid() && info->name))
            return;
        if (!wcsncmp(info->name, L"http://", 7) || !wcsncmp(info->name, L"https://", 8)) {
            hook_status->blocked = true;
            ShellExecuteW(NULL, L"open", info->name, NULL, NULL, SW_SHOWNORMAL);
        }
    }

    std::unordered_map<HookEntry *, UI::UIMessageCallback> UIMessage_callbacks;
    static void __cdecl OnSendUIMessage(uint32_t msgid, void *wParam, void *lParam)
    {
        HookBase::EnterHook();
        HookStatus status;
        for (auto& it : UIMessage_callbacks) {
            it.second(&status, msgid, wParam, lParam);
            ++status.altitude;
        }
        if (!status.blocked)
            RetSendUIMessage(msgid, wParam, lParam);
        HookBase::LeaveHook();
    }

    struct AsyncBuffer {
        void *buffer;
        size_t size;
    };

    void __cdecl __callback_copy_char(void *param, const wchar_t *s) {
        assert(param && s);
        AsyncBuffer *abuf = (AsyncBuffer *)param;
        char *outstr = (char *)abuf->buffer;
        for (size_t i = 0; i < abuf->size; i++) {
            outstr[i] = s[i] & 0x7F;
            if (!s[i]) break;
        }
        delete abuf;
    }

    void __cdecl __callback_copy_wchar(void *param, const wchar_t *s) {
        assert(param && s);
        AsyncBuffer *abuf = (AsyncBuffer *)param;
        wcsncpy((wchar_t *)abuf->buffer, s, abuf->size);
        delete abuf;
    }

    void __cdecl __calback_copy_wstring(void *param, const wchar_t *s) {
        assert(param && s);
        std::wstring *str = (std::wstring *)param;
        *str = s;
    }

    typedef void (__cdecl *DecodeStr_Callback)(void *param, const wchar_t *s);
    void AsyncDecodeStr(const wchar_t *enc_str, DecodeStr_Callback callback, void *param) {
        typedef void(__cdecl *AsyncDecodeStr_pt)(const wchar_t *s, DecodeStr_Callback cb, void *param);
        AsyncDecodeStr_pt Gw_AsyncDecodeStr = (AsyncDecodeStr_pt)AsyncDecodeStringPtr;
        assert(enc_str && Gw_AsyncDecodeStr && callback);
        Gw_AsyncDecodeStr(enc_str, callback, param);
    }

    void Init() {
        SendUIMessage_Func = (SendUIMessage_pt)Scanner::Find(
            "\xE8\x00\x00\x00\x00\x5D\xC3\x89\x45\x08\x5D\xE9", "x????xxxxxxx", -0x1A);
        printf("[SCAN] SendUIMessage = %p\n", SendUIMessage_Func);

        LoadSettings_Func = (LoadSettings_pt)Scanner::Find(
            "\xE8\x00\x00\x00\x00\xFF\x75\x0C\xFF\x75\x08\x6A\x00", "x????xxxxxxxx", -0x1E);
        printf("[SCAN] LoadSettings = %p\n", LoadSettings_Func);

        {
            uintptr_t address = Scanner::Find("\x8D\x4B\x28\x89\x73\x24\x8B\xD7", "xxxxxxx", +0x10);
            printf("[SCAN] GameSettings = %p\n", (void *)address);
            if (Verify(address))
                GameSettings_Addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find("\x83\xF8\x01\x75\x40\xD9\xEE\x8D\x45", "xxxxxxxxx", +0x6C);
            printf("[SCAN] ui_drawn_addr = %p\n", (void *)address);
            if (Verify(address))
                ui_drawn_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\x75\x19\x6A\x00\xC7\x05\x00\x00\x00\x00\x01\x00", "xxxxxx????xx", +6);
            printf("[SCAN] shift_screen_addr = %p\n", (void *)address);
            shift_screen_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = GW::Scanner::Find("\x75\xF6\x33\xF6\x39\x34\x9D", "xxxxxxx", +7);
            printf("[SCAN] preferences_array = %p\n", (void *)address);
            if (Verify(address)) {
                address = *(uintptr_t *)address;
                preferences_array = reinterpret_cast<uint32_t *>(address);
            }
        }

        AsyncDecodeStringPtr = Scanner::Find("\x83\xC4\x10\x3B\xC6\x5E\x74\x14", "xxxxxxxx", -0x70);
        printf("[SCAN] AsyncDecodeStringPtr = %08lX\n", AsyncDecodeStringPtr);

        if (Verify(SendUIMessage_Func))
            HookBase::CreateHook(SendUIMessage_Func, OnSendUIMessage, (void **)&RetSendUIMessage);

        UI::RegisterUIMessageCallback(&open_template_hook, OnOpenTemplate);
    }

    void Exit()
    {
        UI::RemoveUIMessageCallback(&open_template_hook);
    }
}

namespace GW {

    Module UIModule = {
        "UIModule",     // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,         // exit_module
        NULL,           // enable_hooks
        NULL,           // disable_hooks
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
            const unsigned header = GAME_CMSG_DRAW_MAP;
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

    bool UI::GetIsShiftScreenShot() {
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

    void UI::SetOpenLinks(bool toggle)
    {
        open_links = toggle;
    }

    uint32_t UI::GetPreference(Preference pref)
    {
        return preferences_array[pref];
    }

    void UI::SetPreference(Preference pref, uint32_t value)
    {
        preferences_array[value];
    }

    void UI::RegisterUIMessageCallback(
        HookEntry *entry,
        UIMessageCallback callback)
    {
        UIMessage_callbacks.insert({entry, callback});
    }

    void UI::RemoveUIMessageCallback(
        HookEntry *entry)
    {
        auto it = UIMessage_callbacks.find(entry);
        if (it != UIMessage_callbacks.end())
            UIMessage_callbacks.erase(it);
    }
} // namespace GW
