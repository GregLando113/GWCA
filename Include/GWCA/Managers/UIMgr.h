#pragma once
#include <GWCA/Constants/UI.h>
#include <GWCA/Constants/Maps.h>

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/GamePos.h>

namespace GW {

    struct Module;
    extern Module UIModule;
    namespace UI {
        // Note: some windows are affected by UI scale (e.g. party members), others are not (e.g. compass)
        struct WindowPosition {
            uint32_t state; // & 0x1 == visible
            Vec2f p1;
            Vec2f p2;
            bool visible() const { return (state & 0x1) != 0; }
            // Returns vector of from X coord, to X coord.
            Vec2f xAxis(float multiplier = 1.f) const;
            // Returns vector of from Y coord, to Y coord.
            Vec2f yAxis(float multiplier = 1.f) const;
            float left(float multiplier = 1.f) const;
            float right(float multiplier = 1.f) const;
            float top(float multiplier = 1.f) const;
            float bottom(float multiplier = 1.f) const;
            float width(float multiplier = 1.f) const { return right(multiplier) - left(multiplier); }
            float height(float multiplier = 1.f) const { return bottom(multiplier) - top(multiplier); }
        };
        enum class TooltipType {
            None = 0x0,
            EncString1 = 0x4,
            EncString2 = 0x6,
            Item = 0x8,
            WeaponSet = 0xC,
            Skill = 0x14,
            Attribute = 0x4000
        };
        struct TooltipInfo {
            uint32_t bit_field;
            void* render; // Function that the game uses to draw the content
            uint32_t* payload; // uint32_t* for skill or item, wchar_t* for encoded string
            TooltipType unk0; // can use used as an enum in this case
            uint32_t unk1;
            uint32_t unk2;
            TooltipType type() {
                // Without sniffing into each render function to determine the source, we have to guess based on the arguments passed.
                switch (unk0) {
                case TooltipType::Item:
                    // 0x8 also used for attribute tooltips, title tooltips and more
                    if (payload[1] != 0xff) // NB: Item tooltip has 2 item_id values, second is always 0xff
                        return TooltipType::None;
                    break;
                }
                return unk0;
            }
        };

        // SendMessage for Guild Wars UI messages, most UI interactions will use this.
        GWCA_API void SendUIMessage(UIMessage message, void* wParam = nullptr, void* lParam = nullptr);

        GWCA_API bool Keydown(ControlAction key);
        GWCA_API bool Keyup(ControlAction key);
        GWCA_API bool Keypress(ControlAction key);

        GWCA_API UI::WindowPosition* GetWindowPosition(UI::WindowID window_id);
        GWCA_API bool SetWindowVisible(WindowID window_id, bool is_visible);
        GWCA_API bool SetWindowPosition(WindowID window_id, WindowPosition* info);

        GWCA_API void DrawOnCompass(unsigned sessionid, unsigned ptcount, CompassPoint pts[8]);

        // Call from GameThread to be safe
        GWCA_API void LoadSettings(size_t size, uint8_t *data);

        typedef GW::Array<unsigned char> ArrayByte;
        GWCA_API ArrayByte GetSettings();

        GWCA_API bool GetIsUIDrawn();
        GWCA_API bool GetIsShiftScreenShot();
        GWCA_API bool GetIsWorldMapShowing();

        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, wchar_t *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, char    *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, std::wstring *out, Constants::TextLanguage language_id = Constants::TextLanguage::None);

        GWCA_API bool UInt32ToEncStr(uint32_t value, wchar_t *buffer, size_t count);
        GWCA_API uint32_t EncStrToUInt32(const wchar_t *enc_str);

        GWCA_API void SetOpenLinks(bool toggle);

        GWCA_API uint32_t GetPreference(Preference pref);
        GWCA_API void SetPreference(Preference pref, uint32_t value);

        typedef HookCallback<uint32_t> KeyCallback;
        GWCA_API void RegisterKeydownCallback(
            HookEntry* entry,
            KeyCallback callback);
        GWCA_API void RemoveKeydownCallback(
            HookEntry* entry);
        GWCA_API void RegisterKeyupCallback(
            HookEntry* entry,
            KeyCallback callback);
        GWCA_API void RemoveKeyupCallback(
            HookEntry* entry);

        typedef HookCallback<UIMessage, void *, void *> UIMessageCallback;
        GWCA_API void RegisterUIMessageCallback(
            HookEntry *entry,
            UIMessageCallback callback,
            int altitude = -0x8000);

        GWCA_API void RemoveUIMessageCallback(
            HookEntry *entry);

        typedef HookCallback<TooltipInfo*> TooltipCallback;
        GWCA_API void RegisterTooltipCallback(
            HookEntry* entry,
            TooltipCallback callback,
            int altitude = -0x8000);

        GWCA_API void RemoveTooltipCallback(
            HookEntry* entry);

        // Return nullptr to block the string from being decoded, or override with a new string
        typedef std::function<wchar_t*(HookStatus*, DecodingString*)> DecodeStrCallback;
        GWCA_API void RegisterDecodeStringCallback(
            HookEntry* entry,
            DecodeStrCallback callback,
            int altitude = -0x8000);

        GWCA_API void RemoveDecodeStringCallback(
            HookEntry* entry);

        GWCA_API TooltipInfo* GetCurrentTooltip();
    }
}
