#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hook.h>

#include <cstdint>



namespace GW {

    struct Module;
    extern Module UIModule;

    namespace UI {
        typedef GW::Array<unsigned char> ArrayByte;

        struct ChatTemplate {
            uint32_t        unk0;
            uint32_t        type; // 0 = build, 1 = equipement
            Array<wchar_t>  code;
            wchar_t        *name;
        };

        enum UIMessage : uint32_t {
            kShowAgentNameTag       = 0x10000000 | 0x19,
            kHideAgentNameTag       = 0x10000000 | 0x1A,
            kSetAgentNameTagAttribs = 0x10000000 | 0x1B,
            kWriteToChatLog         = 0x10000000 | 0x7E,
            kOpenWhisper            = 0x10000000 | 0x90, // wparam = wchar* name
            kTravel                 = 0x10000000 | 0x17A,
            kOpenTemplate           = 0x10000000 | 0x1B9,
        };

        enum Preference : uint32_t {
            Preference_InterfaceSize = 6,

            Preference_Count = 8
        };

        struct CompassPoint {
            CompassPoint() : x(0), y(0) {}
            CompassPoint(short _x, short _y) : x(_x), y(_y) {}
            short x;
            short y;
        };

        // SendMessage for Guild Wars UI messages, most UI interactions will use this.
        GWCA_API void SendUIMessage(unsigned message, unsigned int wParam = 0, int lParam = 0);
        GWCA_API void SendUIMessage(unsigned message, void* wParam = nullptr, void* lParam = nullptr);

        GWCA_API void DrawOnCompass(unsigned sessionid, unsigned ptcount, CompassPoint pts[8]);

        // Call from GameThread to be safe
        GWCA_API void LoadSettings(size_t size, uint8_t *data);
        GWCA_API ArrayByte GetSettings();

        GWCA_API bool GetIsUIDrawn();
        GWCA_API bool GetIsShiftScreenShot();

        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, wchar_t *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, char    *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, std::wstring *out);

        GWCA_API bool UInt32ToEncStr(uint32_t value, wchar_t *buffer, size_t count);
        GWCA_API uint32_t EncStrToUInt32(const wchar_t *enc_str);

        GWCA_API void SetOpenLinks(bool toggle);

        GWCA_API uint32_t GetPreference(Preference pref);
        GWCA_API void SetPreference(Preference pref, uint32_t value);

        typedef HookCallback<uint32_t, void *, void *> UIMessageCallback;
        GWCA_API void RegisterUIMessageCallback(
            HookEntry *entry,
            UIMessageCallback callback);

        GWCA_API void RemoveUIMessageCallback(
            HookEntry *entry);
    }
}
