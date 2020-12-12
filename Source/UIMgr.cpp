#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>

#include <GWCA/Utilities/Debug.h>
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
    SendUIMessage_pt SendUIMessage_Func = 0;
    SendUIMessage_pt RetSendUIMessage = 0;

    typedef void(__cdecl* SetWindowVisible_pt)(uint32_t window_id, uint32_t is_visible, void* wParam, void* lParam);
    SetWindowVisible_pt SetWindowVisible_Func = 0;

    typedef void(__cdecl* SetTickboxPref_pt)(uint32_t preference_index, uint32_t value, uint32_t unk0);
    SetTickboxPref_pt SetTickboxPref_Func = 0;
    SetTickboxPref_pt RetSetTickboxPref = 0;

    typedef void(__fastcall* DoAction_pt)(void* ecx, void* edx, uint32_t msgid, void* arg1, void* arg2);
    DoAction_pt DoAction_Func = 0;
    DoAction_pt RetDoAction = 0;

    struct KeypressPacket {
        uint32_t key = 0;
        uint32_t unk1 = 0x4000;
        uint32_t unk2 = 0;
    };
    GW::Array<uintptr_t>* s_FrameCache = nullptr;
    static uintptr_t GetActionContext()
    {
        if (!(s_FrameCache && s_FrameCache->size() > 1))
            return 0;
        return (*s_FrameCache)[1] + 0xA0;
    }

    typedef void (__cdecl *LoadSettings_pt)(uint32_t size, uint8_t *data);
    LoadSettings_pt LoadSettings_Func = 0;

    bool open_links = false;
    HookEntry open_template_hook;

    uintptr_t CommandAction_Addr = 0;
    uintptr_t GameSettings_Addr;
    uintptr_t ui_drawn_addr;
    uintptr_t shift_screen_addr;
    uintptr_t AsyncDecodeStringPtr;
    uint32_t *preferences_array;
    uint32_t* preferences_array2;
    UI::WindowPosition* window_positions_array = 0;

    static void OnOpenTemplate(HookStatus *hook_status, uint32_t msgid, void *wParam, void *lParam)
    {
        UNREFERENCED_PARAMETER(lParam);

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

    static void __cdecl OnSetTickboxPreference(uint32_t preference_index, uint32_t value, uint32_t unk0) {
        HookBase::EnterHook();
        RetSetTickboxPref(preference_index, value, unk0);
        HookBase::LeaveHook();
    }
    std::unordered_map<HookEntry*, UI::KeyCallback> OnKeydown_callbacks;
    std::unordered_map<HookEntry*, UI::KeyCallback> OnKeyup_callbacks;
    static void __fastcall OnDoAction(void* ecx, void* edx, uint32_t action_type, void* arg1, void* arg2) {
        HookBase::EnterHook();
        switch (action_type) {
        case 0x1E: // Keydown
        case 0x20: // Keyup
        {
            GW::HookStatus status;
            uint32_t key_pressed = *(uint32_t*)arg1;
            auto* callbacks = action_type == 0x1e ? &OnKeydown_callbacks : &OnKeyup_callbacks;
            for (auto& it : *callbacks) {
                it.second(&status, key_pressed);
                ++status.altitude;
            }
            if (!status.blocked)
                RetDoAction(ecx, edx, action_type, arg1, arg2);
        }
            break;
        default:
            RetDoAction(ecx, edx, action_type, arg1, arg2);
            break;
        }
        HookBase::LeaveHook();
    }

    struct AsyncBuffer {
        void *buffer;
        size_t size;
    };

    void __cdecl __callback_copy_char(void *param, const wchar_t *s) {
        GWCA_ASSERT(param && s);
        AsyncBuffer *abuf = (AsyncBuffer *)param;
        char *outstr = (char *)abuf->buffer;
        for (size_t i = 0; i < abuf->size; i++) {
            outstr[i] = s[i] & 0x7F;
            if (!s[i]) break;
        }
        delete abuf;
    }

    void __cdecl __callback_copy_wchar(void *param, const wchar_t *s) {
        GWCA_ASSERT(param && s);
        AsyncBuffer *abuf = (AsyncBuffer *)param;
        wcsncpy((wchar_t *)abuf->buffer, s, abuf->size);
        delete abuf;
    }

    void __cdecl __calback_copy_wstring(void *param, const wchar_t *s) {
        GWCA_ASSERT(param && s);
        std::wstring *str = (std::wstring *)param;
        *str = s;
    }

    typedef void (__cdecl *DecodeStr_Callback)(void *param, const wchar_t *s);
    void AsyncDecodeStr(const wchar_t *enc_str, DecodeStr_Callback callback, void *param) {
        typedef void(__cdecl *AsyncDecodeStr_pt)(const wchar_t *s, DecodeStr_Callback cb, void *param);
        AsyncDecodeStr_pt Gw_AsyncDecodeStr = (AsyncDecodeStr_pt)AsyncDecodeStringPtr;
        GWCA_ASSERT(enc_str && Gw_AsyncDecodeStr && callback);
        Gw_AsyncDecodeStr(enc_str, callback, param);
    }

    void Init() {
        uintptr_t FrameCache_addr = Scanner::Find("\x68\x00\x10\x00\x00\x8B\x1C\x98\x8D", "xxxxxxxxx", -4);
        if (Verify(FrameCache_addr))
            s_FrameCache = *(GW::Array<uintptr_t>**)FrameCache_addr;
        GWCA_INFO("[SCAN] FrameCache_addr = %p\n", FrameCache_addr);

        DoAction_Func = (DoAction_pt)Scanner::Find("\x8B\x75\x08\x57\x8B\xF9\x83\xFE\x09\x75", "xxxxxxxxxx", -0x4);
        GWCA_INFO("[SCAN] DoAction = %p\n", DoAction_Func);

        if (Verify(DoAction_Func))
            HookBase::CreateHook(DoAction_Func, OnDoAction, (void**)&RetDoAction);

        SendUIMessage_Func = (SendUIMessage_pt)Scanner::Find(
            "\xE8\x00\x00\x00\x00\x5D\xC3\x89\x45\x08\x5D\xE9", "x????xxxxxxx", -0x1A);
        GWCA_INFO("[SCAN] SendUIMessage = %p\n", SendUIMessage_Func);

        LoadSettings_Func = (LoadSettings_pt)Scanner::Find(
            "\xE8\x00\x00\x00\x00\xFF\x75\x0C\xFF\x75\x08\x6A\x00", "x????xxxxxxxx", -0x1E);
        GWCA_INFO("[SCAN] LoadSettings = %p\n", LoadSettings_Func);

        {
            uintptr_t address = Scanner::Find("\x8D\x4B\x28\x89\x73\x24\x8B\xD7", "xxxxxxx", +0x10);
            GWCA_INFO("[SCAN] GameSettings = %p\n", (void *)address);
            if (Verify(address))
                GameSettings_Addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find("\x83\xF8\x01\x75\x40\xD9\xEE\x8D\x45", "xxxxxxxxx", +0x6C);
            GWCA_INFO("[SCAN] ui_drawn_addr = %p\n", (void *)address);
            if (Verify(address))
                ui_drawn_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\x75\x19\x6A\x00\xC7\x05\x00\x00\x00\x00\x01\x00", "xxxxxx????xx", +6);
            GWCA_INFO("[SCAN] shift_screen_addr = %p\n", (void *)address);
            shift_screen_addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = GW::Scanner::Find("\x75\xF6\x33\xF6\x39\x34\x9D", "xxxxxxx", +7);
            GWCA_INFO("[SCAN] preferences_array = %p\n", (void *)address);
            if (Verify(address)) {
                address = *(uintptr_t *)address;
                preferences_array = reinterpret_cast<uint32_t *>(address);
            }
        }

        SetTickboxPref_Func = (SetTickboxPref_pt)Scanner::Find(
            "\x8B\x75\x0C\x33\xC9\x39\x0C\xBD\x00\x00\x00\x00\x0F\x95\xC1\x33", "xxxxxxxx????xxxx", -0x6F);
        GWCA_INFO("[SCAN] SetTickboxPref = %p\n", SetTickboxPref_Func);

        if (Verify(SetTickboxPref_Func)) {
            uintptr_t address = (uintptr_t)SetTickboxPref_Func + 0x77;
            GWCA_INFO("[SCAN] preferences_array2 = %p\n", (void*)address);
            address = *(uintptr_t*)address;
            preferences_array2 = reinterpret_cast<uint32_t*>(address);

            HookBase::CreateHook(SetTickboxPref_Func, OnSetTickboxPreference, (void**)&RetSetTickboxPref);
        }

        SetWindowVisible_Func = (SetWindowVisible_pt)Scanner::Find("\x8B\x75\x08\x83\xFE\x66\x7C\x19\x68", "xxxxxxxxx", -0x7);
        GWCA_INFO("[SCAN] SetWindowVisible_Func = %08X\n", SetWindowVisible_Func);
        if (SetWindowVisible_Func) {
            uintptr_t address = (uintptr_t)SetWindowVisible_Func + 0x49;
            if (Verify(address)) {
                address = *(uintptr_t*)address;
                window_positions_array = reinterpret_cast<UI::WindowPosition*>(address);
            }
        }
        GWCA_INFO("[SCAN] window_positions_array = %p\n", (void*)window_positions_array);

        AsyncDecodeStringPtr = Scanner::Find("\x83\xC4\x10\x3B\xC6\x5E\x74\x14", "xxxxxxxx", -0x70);
        GWCA_INFO("[SCAN] AsyncDecodeStringPtr = %08X\n", AsyncDecodeStringPtr);

        if (Verify(SendUIMessage_Func))
            HookBase::CreateHook(SendUIMessage_Func, OnSendUIMessage, (void **)&RetSendUIMessage);

        UI::RegisterUIMessageCallback(&open_template_hook, OnOpenTemplate);
    }

    void Exit()
    {
        UI::RemoveUIMessageCallback(&open_template_hook);
        if(SetTickboxPref_Func) 
            HookBase::RemoveHook(SetTickboxPref_Func);
        if (DoAction_Func) 
            HookBase::RemoveHook(DoAction_Func);
        if(SendUIMessage_Func)
            HookBase::RemoveHook(SendUIMessage_Func);
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
    bool UI::Keydown(ControlAction key) {
        uintptr_t ecx = GetActionContext();
        if (!(ecx && DoAction_Func))
            return false;
        KeypressPacket action;
        action.key = key;
        OnDoAction((void*)ecx, 0, 0x1E, &action, 0);
        return true;
    }
    bool UI::Keyup(ControlAction key) {
        uintptr_t ecx = GetActionContext();
        if (!(ecx && DoAction_Func))
            return false;
        KeypressPacket action;
        action.key = key;
        OnDoAction((void*)ecx, 0, 0x20, &action, 0);
        return true;
    }

    bool UI::SetWindowVisible(UI::WindowID window_id,bool is_visible) {
        if (!SetWindowVisible_Func || window_id >= UI::WindowID::WindowID_Count)
            return false;
        SetWindowVisible_Func(window_id, is_visible ? 1 : 0, 0, 0);
        return true;
    }
    UI::WindowPosition* UI::GetWindowPosition(UI::WindowID window_id) {
        if (!window_positions_array || window_id >= UI::WindowID::WindowID_Count)
            return nullptr;
        return &window_positions_array[window_id];
    }

    bool UI::Keypress(ControlAction key) {
        if (!Keydown(key))
            return false;
        GW::GameThread::Enqueue([key]() {
            Keyup(key);
            });
        return true;
    }
    void UI::DrawOnCompass(unsigned session_id, unsigned pt_count, CompassPoint *pts)
    {
        struct P037 {                   // Used to send pings and drawings in the minimap. Related to StoC::P133
            uint32_t header;
            uint32_t session_id;     // unique for every player and shape. changes for every ping or shape.
            uint32_t pt_count;           // number of points in the following array
            CompassPoint pts[8]; // in world coordinates divided by 100
            uint32_t unk[8];
        };
        P037 pack = {0};
        pack.header = GAME_CMSG_DRAW_MAP;
        pack.session_id = session_id;
        pack.pt_count = pt_count;
        for (unsigned i = 0; i < pt_count; ++i)
            pack.pts[i] = pts[i];
        CtoS::SendPacket(&pack);
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
            GWCA_ASSERT(*enc_str >= WORD_VALUE_BASE);
            val *= WORD_VALUE_RANGE;
            val += (*enc_str & ~WORD_BIT_MORE) - WORD_VALUE_BASE;
        } while (*enc_str++ & WORD_BIT_MORE);
        return val;
    }

    void UI::SetOpenLinks(bool toggle)
    {
        open_links = toggle;
    }

    uint32_t UI::GetCheckboxPreference(CheckboxPreference pref) {
        return preferences_array2[pref];
    }
    void UI::SetCheckboxPreference(CheckboxPreference pref, uint32_t value) {
        if ((value != 1 && value != 0) || preferences_array2[pref] == value)
            return; // Invalid checkbox value
        if(Verify(SetTickboxPref_Func))
            RetSetTickboxPref(pref, value, 0);
    }

    uint32_t UI::GetPreference(Preference pref)
    {
        return preferences_array[pref];
    }

    void UI::SetPreference(Preference pref, uint32_t value)
    {
        preferences_array[pref] = value;
    }

    void UI::RegisterKeyupCallback(HookEntry* entry, KeyCallback callback) {
        OnKeyup_callbacks.insert({ entry, callback });
    }
    void UI::RemoveKeyupCallback(HookEntry* entry) {
        auto it = OnKeyup_callbacks.find(entry);
        if (it != OnKeyup_callbacks.end())
            OnKeyup_callbacks.erase(it);
    }

    void UI::RegisterKeydownCallback(HookEntry* entry, KeyCallback callback) {
        OnKeydown_callbacks.insert({ entry, callback });
    }
    void UI::RemoveKeydownCallback(HookEntry* entry) {
        auto it = OnKeydown_callbacks.find(entry);
        if (it != OnKeydown_callbacks.end())
            OnKeydown_callbacks.erase(it);
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
