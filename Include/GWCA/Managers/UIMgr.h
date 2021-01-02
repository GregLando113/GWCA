#pragma once

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/GamePos.h>

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

        struct ChangeTargetUIMsg {
            uint32_t        manual_target_id;
            uint32_t        unk1;
            uint32_t        auto_target_id;
            uint32_t        unk2;
            // ...
        };
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
            float width() const { return right() - left(); }
            float height() const { return bottom() - top(); }
        };

        enum UIMessage : uint32_t {
            kShowAgentNameTag       = 0x10000000 | 0x19,
            kHideAgentNameTag       = 0x10000000 | 0x1A,
            kSetAgentNameTagAttribs = 0x10000000 | 0x1B,
            kChangeTarget           = 0x10000000 | 0x20,
            kShowXunlaiChest        = 0x10000000 | 0x40,
            kWriteToChatLog         = 0x10000000 | 0x7E,
            kOpenWhisper            = 0x10000000 | 0x90, // wparam = wchar* name
            kCheckboxPreference     = 0x10000000 | 0x13F,
            kTravel                 = 0x10000000 | 0x17A,
            kHideHeroPanel          = 0x10000000 | 0x197, // wparam = hero_id
            kShowHeroPanel          = 0x10000000 | 0x198, // wparam = hero_id
            kOpenTemplate           = 0x10000000 | 0x1B9,
        };

        enum Preference : uint32_t {
            Preference_InterfaceSize = 6,

            Preference_Count = 8
        };
        // Used with GetWindowPosition
        enum WindowID : uint32_t {
            WindowID_Dialogue1 = 0x0,
            WindowID_Dialogue2 = 0x1,
            WindowID_MissionGoals = 0x2,
            WindowID_DropBundle = 0x3,
            WindowID_Chat = 0x4,
            WindowID_InGameClock = 0x6,
            WindowID_Compass = 0x7,
            WindowID_DamageMonitor = 0x8,
            WindowID_PerformanceMonitor = 0xB,
            WindowID_EffectsMonitor = 0xC,
            WindowID_Hints = 0xD,
            WindowID_MissionStatusAndScoreDisplay = 0xF,
            WindowID_Notifications = 0x11,
            WindowID_Skillbar = 0x14,
            WindowID_SkillMonitor = 0x15,
            WindowID_UpkeepMonitor = 0x17,
            WindowID_SkillWarmup = 0x18,
            WindowID_Menu = 0x1A,
            WindowID_EnergyBar = 0x1C,
            WindowID_ExperienceBar = 0x1D,
            WindowID_HealthBar = 0x1E,
            WindowID_TargetDisplay = 0x1F,
            WindowID_MissionProgress = 0xE,
            WindowID_TradeButton = 0x21,
            WindowID_WeaponBar = 0x22,
            WindowID_Hero1 = 0x33,
            WindowID_Hero2 = 0x34,
            WindowID_Hero3 = 0x35,
            WindowID_Hero = 0x36,
            WindowID_SkillsAndAttributes = 0x38,
            WindowID_Friends = 0x3A,
            WindowID_Guild = 0x3B,
            WindowID_Help = 0x3D,
            WindowID_Inventory = 0x3E,
            WindowID_InventoryBags = 0x40,
            WindowID_MissionMap = 0x42,
            WindowID_Observe = 0x44,
            WindowID_Options = 0x45,
            WindowID_PartyWindow = 0x48, // NB: state flag is ignored for party window, but position is still good
            WindowID_PartySearch = 0x49,
            WindowID_QuestLog = 0x4F,
            WindowID_Hero4 = 0x5E,
            WindowID_Hero5 = 0x5F,
            WindowID_Hero6 = 0x60,
            WindowID_Hero7 = 0x61,
            WindowID_Count = 0x66
        };
        enum CheckboxPreference : uint32_t {
            CheckboxPreference_ChannelAlliance = 0x4,
            CheckboxPreference_ChannelEmotes = 0x6,
            CheckboxPreference_ChannelGuild = 0x7,
            CheckboxPreference_ChannelLocal = 0x8,
            CheckboxPreference_ChannelGroup = 0x9,
            CheckboxPreference_ChannelTrade = 0xA,

            CheckboxPreference_InvertMouseControlOfCamera = 0x16,
            CheckboxPreference_DisableMouseWalking = 0x17,

            CheckboxPreference_WaitForVSync = 0x54,
            CheckboxPreference_ShowChatTimestamps = 0x56,
            CheckboxPreference_LockCompassRotation = 0x5c,
            CheckboxPreference_Count = 0x5D
        };

        enum ControlAction : uint32_t {
            ControlAction_None = 0,
            ControlAction_CloseAllPanels = 0x85,
            ControlAction_ReverseCamera = 0x90,
            ControlAction_StrafeLeft = 0x91,
            ControlAction_StrafeRight = 0x92,
            ControlAction_TurnLeft = 0xA2,
            ControlAction_TurnRight = 0xA3,
            ControlAction_MoveBackward = 0xAC,
            ControlAction_MoveForward = 0xAD,
            ControlAction_ReverseDirection = 0xB1,
            ControlAction_Autorun = 0xB7,
            ControlAction_TargetNearestItem = 0xC3,
            

            ControlAction_CommandParty = 0xD6,
            ControlAction_CommandHero1 = 0xD7,
            ControlAction_CommandHero2 = 0xD8,
            ControlAction_CommandHero3 = 0xD9,
            ControlAction_CommandHero4 = 0x102,
            ControlAction_CommandHero5 = 0x103,
            ControlAction_CommandHero6 = 0x104,
            ControlAction_CommandHero7 = 0x105
            
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

        GWCA_API bool Keydown(ControlAction key);
        GWCA_API bool Keyup(ControlAction key);
        GWCA_API bool Keypress(ControlAction key);

        GWCA_API UI::WindowPosition* GetWindowPosition(UI::WindowID window_id);
        GWCA_API bool SetWindowVisible(UI::WindowID window_id, bool is_visible);
        GWCA_API bool SetWindowPosition(UI::WindowID window_id, UI::WindowPosition* info);

        GWCA_API void DrawOnCompass(unsigned sessionid, unsigned ptcount, CompassPoint pts[8]);

        // Call from GameThread to be safe
        GWCA_API void LoadSettings(size_t size, uint8_t *data);
        GWCA_API ArrayByte GetSettings();

        GWCA_API bool GetIsUIDrawn();
        GWCA_API bool GetIsShiftScreenShot();
        GWCA_API bool GetIsWorldMapShowing();

        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, wchar_t *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, char    *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, std::wstring *out);

        GWCA_API bool UInt32ToEncStr(uint32_t value, wchar_t *buffer, size_t count);
        GWCA_API uint32_t EncStrToUInt32(const wchar_t *enc_str);

        GWCA_API void SetOpenLinks(bool toggle);

        GWCA_API uint32_t GetPreference(Preference pref);
        GWCA_API void SetPreference(Preference pref, uint32_t value);

        GWCA_API uint32_t GetCheckboxPreference(CheckboxPreference pref);
        GWCA_API void SetCheckboxPreference(CheckboxPreference pref, uint32_t value);

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

        typedef HookCallback<uint32_t, void *, void *> UIMessageCallback;
        GWCA_API void RegisterUIMessageCallback(
            HookEntry *entry,
            UIMessageCallback callback);

        GWCA_API void RemoveUIMessageCallback(
            HookEntry *entry);
    }
}
