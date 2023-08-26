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

        typedef void(__cdecl* DecodeStr_Callback)(void* param, wchar_t* s);

        struct ChatTemplate {
            uint32_t        agent_id;
            uint32_t        type; // 0 = build, 1 = equipment
            Array<wchar_t>  code;
            wchar_t        *name;
        };

        struct UIChatMessage {
            uint32_t channel;
            wchar_t* message;
            uint32_t channel2;
        };

        struct ChangeTargetUIMsg {
            uint32_t        manual_target_id;
            uint32_t        unk1;
            uint32_t        auto_target_id;
            uint32_t        unk2;
            // ...
        };

        struct InteractionMessage {
            InteractionMessage(uint32_t _message_id) : message_id(_message_id) {
                action_type = 0xF;
                wParam = (void**)&wParam; // Naughty, but not got this mapped yet and needed to pass checks
            }
            uint32_t action_type;
            uint32_t message_id; // Same as UIMessage from UIMgr, but includes things like mouse move, click etc
            void** wParam;
        };

        typedef void(__cdecl* UIInteractionCallback)(InteractionMessage* message, void* wParam, void* lParam);

        struct AgentNameTagInfo {
            /* +h0000 */ uint32_t agent_id;
            /* +h0004 */ uint32_t h0002;
            /* +h0008 */ uint32_t h0003;
            /* +h000C */ wchar_t* name_enc;
            /* +h0010 */ uint8_t h0010;
            /* +h0011 */ uint8_t h0012;
            /* +h0012 */ uint8_t h0013;
            /* +h0013 */ uint8_t background_alpha; // ARGB, NB: Actual color is ignored, only alpha is used
            /* +h0014 */ uint32_t text_color; // ARGB
            /* +h0014 */ uint32_t label_attributes; // bold/size etc
            /* +h001C */ uint8_t font_style; // Text style (bitmask) / bold | 0x1 / strikthrough | 0x80
            /* +h001D */ uint8_t underline; // Text underline (bool) = 0x01 - 0xFF
            /* +h001E */ uint8_t h001E;
            /* +h001F */ uint8_t h001F;
            /* +h0020 */ wchar_t* extra_info_enc; // Title etc
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
            float width(float multiplier = 1.f) const { return right(multiplier) - left(multiplier); }
            float height(float multiplier = 1.f) const { return bottom(multiplier) - top(multiplier); }
        };

        struct MapEntryMessage {
            wchar_t* title;
            wchar_t* subtitle;
        };

        struct DialogBodyInfo {
            uint32_t type;
            uint32_t agent_id;
            wchar_t* message_enc;
        };
        struct DialogButtonInfo {
            uint32_t button_icon; // byte
            wchar_t* message;
            uint32_t dialog_id;
            uint32_t skill_id; // Default 0xFFFFFFF
        };

        struct DecodingString {
            std::wstring encoded;
            std::wstring decoded;
            void* original_callback;
            void* original_param;
            void* ecx;
            void* edx;
        };

        enum class UIMessage : uint32_t {
            kUpdateAgentEffects         = 0x10000000 | 0x9,
            kRerenderAgentModel         = 0x10000000 | 0x7, // wparam = uint32_t agent_id
            kShowAgentNameTag           = 0x10000000 | 0x19, // wparam = AgentNameTagInfo*
            kHideAgentNameTag           = 0x10000000 | 0x1A,
            kSetAgentNameTagAttribs     = 0x10000000 | 0x1B, // wparam = AgentNameTagInfo*
            kChangeTarget               = 0x10000000 | 0x20, // wparam = ChangeTargetUIMsg*
            kAgentStartCasting          = 0x10000000 | 0x27, // wparam = { uint32_t agent_id, uint32_t skill_id }
            kShowMapEntryMessage        = 0x10000000 | 0x29, // wparam = { wchar_t* title, wchar_t* subtitle }
            kSetCurrentPlayerData       = 0x10000000 | 0x2A, // fired after setting the worldcontext player name
            kHeroAgentAdded             = 0x10000000 | 0x38, // hero assigned to agent/inventory/ai mode
            kHeroDataAdded              = 0x10000000 | 0x39, // hero info received from server (name, level etc)
            kShowXunlaiChest            = 0x10000000 | 0x40,
            kMinionCountUpdated         = 0x10000000 | 0x46,
            kMoraleChange               = 0x10000000 | 0x47, // wparam = {agent id, morale percent }
            kLoginStateChanged          = 0x10000000 | 0x50, // wparam = {bool is_logged_in, bool unk }
            kEffectAdd                  = 0x10000000 | 0x55, // wparam = {agent_id, GW::Effect*}
            kEffectRenew                = 0x10000000 | 0x56, // wparam = GW::Effect*
            kEffectRemove               = 0x10000000 | 0x57, // wparam = effect id
            kUpdateSkillbar             = 0x10000000 | 0x5E, // wparam ={ uint32_t agent_id , ... }
            kSkillActivated             = 0x10000000 | 0x5b, // wparam ={ uint32_t agent_id , uint32_t skill_id }
            kTitleProgressUpdated       = 0x10000000 | 0x65, // wparam = title_id
            kExperienceGained           = 0x10000000 | 0x66, // wparam = experience amount
            kWriteToChatLog             = 0x10000000 | 0x7E,
            kPlayerChatMessage          = 0x10000000 | 0x81, // wparam = { uint32_t channel, wchar_t* message, uint32_t player_number }
            kFriendUpdated              = 0x10000000 | 0x89, // wparam = { GW::Friend*, ... }
            kMapLoaded                  = 0x10000000 | 0x8A,
            kOpenWhisper                = 0x10000000 | 0x90, // wparam = wchar* name
            kLogout                     = 0x10000000 | 0x9b, // wparam = { bool unknown, bool character_select }
            kDialogBody                 = 0x10000000 | 0xA4, // wparam = DialogBodyInfo*
            kDialogButton               = 0x10000000 | 0xA1, // wparam = DialogButtonInfo*
            kTargetNPCPartyMember       = 0x10000000 | 0xB1, // wparam = { uint32_t unk, uint32_t agent_id }
            kTargetPlayerPartyMember    = 0x10000000 | 0xB2, // wparam = { uint32_t unk, uint32_t player_number }
            kInitMerchantList           = 0x10000000 | 0xB3, // wparam = { uint32_t merchant_tab_type, uint32_t unk, uint32_t merchant_agent_id, uint32_t is_pending }
            kQuotedItemPrice            = 0x10000000 | 0xBB, // wparam = { uint32_t item_id, uint32_t price }
            kStartMapLoad               = 0x10000000 | 0xC0, // wparam = { uint32_t map_id, ...}
            kWorldMapUpdated            = 0x10000000 | 0xC5, // Triggered when an area in the world map has been discovered/updated
            kGuildMemberUpdated         = 0x10000000 | 0xD8, // wparam = { GuildPlayer::name_ptr }
            kShowHint                   = 0x10000000 | 0xDF, // wparam = { uint32_t icon_type, wchar_t* message_enc }
            kUpdateGoldCharacter        = 0x10000000 | 0xEA, // wparam = { uint32_t unk, uint32_t gold_character }
            kUpdateGoldStorage          = 0x10000000 | 0xEB, // wparam = { uint32_t unk, uint32_t gold_storage }
            kInventorySlotUpdated       = 0x10000000 | 0xEC, // undocumented. Triggered when an item is moved into a slot
            kEquipmentSlotUpdated       = 0x10000000 | 0xED, // undocumented. Triggered when an item is moved into a slot
            kInventorySlotCleared       = 0x10000000 | 0xEF, // undocumented. Triggered when an item has been removed from a slot
            kEquipmentSlotCleared       = 0x10000000 | 0xF0, // undocumented. Triggered when an item has been removed from a slot
            kPvPWindowContent           = 0x10000000 | 0xF8,
            kItemUpdated                = 0x10000000 | 0x104, // wparam = *ItemGeneral packet
            kMapChange                  = 0x10000000 | 0x10F, // wparam = map id
            kCalledTargetChange         = 0x10000000 | 0x113, // wparam = { player_number, target_id }
            kErrorMessage               = 0x10000000 | 0x117, // wparam = { int error_index, wchar_t* error_encoded_string }
            kEnumPreference             = 0x10000000 | 0x13E, // wparam = { EnumPreference pref_id, uint32_t val }
            kCheckboxPreference         = 0x10000000 | 0x13F,
            kPreferenceChanged          = 0x10000000 | 0x140,
            kUIPositionChanged          = 0x10000000 | 0x141,
            kQuestAdded                 = 0x10000000 | 0x149, // wparam = { quest_id, ... }
            kQuestDetailsChanged        = 0x10000000 | 0x14A, // wparam = { quest_id, ... }
            kClientActiveQuestChanged   = 0x10000000 | 0x14C, // wparam = { quest_id, ... }. Triggered when the game requests the current quest to change
            kServerActiveQuestChanged   = 0x10000000 | 0x14E, // wparam = { quest_id, ... }. Triggered when the server requests the current quest to change
            kObjectiveComplete          = 0x10000000 | 0x156, // wparam = { objective_id, ... }
            kCheckUIState               = 0x10000000 | 0x170, // Undocumented
            kGuildHall                  = 0x10000000 | 0x177, // wparam = gh key (uint32_t[4])
            kLeaveGuildHall             = 0x10000000 | 0x179,
            kTravel                     = 0x10000000 | 0x17A,
            kOpenWikiUrl                = 0x10000000 | 0x17B, // wparam = char* url
            kAppendMessageToChat        = 0x10000000 | 0x189, // wparam = wchar_t* message
            kHideHeroPanel              = 0x10000000 | 0x197, // wparam = hero_id
            kShowHeroPanel              = 0x10000000 | 0x198, // wparam = hero_id
            kMoveItem                   = 0x10000000 | 0x19e, // wparam = { item_id, to_bag, to_slot, bool prompt }
            kInitiateTrade              = 0x10000000 | 0x1A0,
            kOpenTemplate               = 0x10000000 | 0x1B9, // wparam = GW::UI::ChatTemplate*

            // GWCA Client to Server commands. Only added the ones that are used for hooks, everything else goes straight into GW
            kSendDialog                 = 0x30000000 | 0x1,  // wparam = dialog_id
            kSendEnterMission           = 0x30000000 | 0x2,  // wparam = arena_id
            kSendLoadSkillbar           = 0x30000000 | 0x3,  // wparam = { uint32_t agent_id, uint32_t* skill_ids }
            kSendPingWeaponSet          = 0x30000000 | 0x4,  // wparam = { uint32_t agent_id, uint32_t weapon_item_id, uint32_t offhand_item_id }
            kSendMoveItem               = 0x30000000 | 0x5,  // wparam = { uint32_t item_id, uint32_t quantity, uint32_t bag_id, uint32_t slot }
            kSendMerchantRequestQuote   = 0x30000000 | 0x6,  // wparam = { Merchant::TransactionType type,uint32_t gold_give,Merchant::TransactionInfo give,uint32_t gold_recv,Merchant::TransactionInfo recv }
            kSendMerchantTransactItem   = 0x30000000 | 0x7,  // wparam = { Merchant::TransactionType type, uint32_t unknown, Merchant::QuoteInfo give, Merchant::QuoteInfo recv }
            kSendUseItem                = 0x30000000 | 0x8,  // wparam = uint32_t item_id
            kSendSetActiveQuest         = 0x30000000 | 0x9,  // wparam = uint32_t quest_id
            kSendAbandonQuest           = 0x30000000 | 0x10,  // wparam = uint32_t quest_id
        };

        enum class NumberCommandLineParameter : uint32_t {
            Unk1,
            Unk2,
            FPS,
            Count = 0x3
        };

        enum class EnumPreference : uint32_t {
            CharSortOrder,
            AntiAliasing, // multi sampling
            Reflections,
            ShaderQuality,
            ShadowQuality,
            TerrainQuality,
            InterfaceSize,
            FrameLimiter,
            Count = 0x8
        };
        enum class StringPreference : uint32_t {
            Unk1,
            Unk2,
            LastCharacterName,
            Count = 0x3
        };

        enum class NumberPreference : uint32_t {
            // number preferences
            AutoTournPartySort,
            ChatState, // 1 == showing chat window, 0 = not showing chat window
            ChatTab,
            DistrictLastVisitedLanguage,
            DistrictLastVisitedLanguage2,
            DistrictLastVisitedNonInternationalLanguage,
            DistrictLastVisitedNonInternationalLanguage2,
            DamageTextSize, // 0 to 100
            FullscreenGamma, // 0 to 100
            InventoryBag, // Selected bag in inventory window
            TextLanguage,
            AudioLanguage,
            ChatFilterLevel,
            RefreshRate,
            ScreenSizeX,
            ScreenSizeY,
            SkillListFilterRarity,
            SkillListSortMethod,
            SkillListViewMode,
            SoundQuality, // 0 to 100
            StorageBagPage,
            Territory,
            TextureQuality, // TextureLod
            UseBestTextureFiltering,
            EffectsVolume, // 0 to 100
            DialogVolume, // 0 to 100
            BackgroundVolume, // 0 to 100
            MusicVolume, // 0 to 100
            UIVolume, // 0 to 100
            Vote,
            WindowPosX,
            WindowPosY,
            WindowSizeX,
            WindowSizeY,
            SealedSeed, // Used in codex arena
            SealedCount, // Used in codex arena
            FieldOfView, // 0 to 100
            CameraRotationSpeed, // 0 to 100
            ScreenBorderless, // 0x1 = Windowed Borderless, 0x2 = Windowed Fullscreen
            MasterVolume, // 0 to 100
            ClockMode,
            Count = 0x29
        };
        enum class FlagPreference : uint32_t {
            // boolean preferences
            ChannelAlliance = 0x4,
            ChannelEmotes = 0x6,
            ChannelGuild,
            ChannelLocal,
            ChannelGroup,
            ChannelTrade,
            ShowTextInSkillFloaters = 0x11,
            ShowKRGBRatingsInGame,
            AutoHideUIOnLoginScreen = 0x14,
            DoubleClickToInteract,
            InvertMouseControlOfCamera,
            DisableMouseWalking,
            AutoCameraInObserveMode,
            AutoHideUIInObserveMode,
            RememberAccountName = 0x2D,
            IsWindowed,
            ShowSpendAttributesButton = 0x31, // The game uses this hacky method of showing the "spend attributes" button next to the exp bar.
            ConciseSkillDescriptions,
            DoNotShowSkillTipsOnEffectMonitor,
            DoNotShowSkillTipsOnSkillBars,
            MuteWhenGuildWarsIsInBackground = 0x37,
            AutoTargetFoes = 0x39,
            AutoTargetNPCs,
            AlwaysShowNearbyNamesPvP,
            FadeDistantNameTags,
            DoNotCloseWindowsOnEscape = 0x45,
            ShowMinimapOnWorldMap,
            WaitForVSync = 0x54,
            WhispersFromFriendsEtcOnly,
            ShowChatTimestamps,
            ShowCollapsedBags,
            ItemRarityBorder,
            AlwaysShowAllyNames,
            AlwaysShowFoeNames,
            LockCompassRotation = 0x5c,
            Count = 0x5d
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
            WindowID_VaultBox = 0x3F,
            WindowID_InventoryBags = 0x40,
            WindowID_MissionMap = 0x42,
            WindowID_Observe = 0x44,
            WindowID_Options = 0x45,
            WindowID_PartyWindow = 0x48, // NB: state flag is ignored for party window, but position is still good
            WindowID_PartySearch = 0x49,
            WindowID_QuestLog = 0x4F,
            WindowID_Merchant = 0x5C,
            WindowID_Hero4 = 0x5E,
            WindowID_Hero5 = 0x5F,
            WindowID_Hero6 = 0x60,
            WindowID_Hero7 = 0x61,
            WindowID_Count = 0x66
        };

        enum ControlAction : uint32_t {
            ControlAction_None = 0,
            ControlAction_Screenshot = 0xAE,
            // Panels
            ControlAction_CloseAllPanels = 0x85,
            ControlAction_ToggleInventoryWindow = 0x8B,
            ControlAction_OpenScoreChart = 0xBD,
            ControlAction_OpenTemplateManager = 0xD3,
            ControlAction_OpenSaveEquipmentTemplate = 0xD4,
            ControlAction_OpenSaveSkillTemplate = 0xD5,
            ControlAction_OpenParty = 0xBF,
            ControlAction_OpenGuild = 0xBA,
            ControlAction_OpenFriends = 0xB9,
            ControlAction_ToggleAllBags = 0xB8,
            ControlAction_OpenMissionMap = 0xB6,
            ControlAction_OpenBag2 = 0xB5,
            ControlAction_OpenBag1 = 0xB4,
            ControlAction_OpenBelt = 0xB3,
            ControlAction_OpenBackpack = 0xB2,
            ControlAction_OpenSkillsAndAttributes = 0x8F,
            ControlAction_OpenQuestLog = 0x8E,
            ControlAction_OpenWorldMap = 0x8C,
            ControlAction_OpenHero = 0x8A,

            // Weapon sets
            ControlAction_CycleEquipment = 0x86,
            ControlAction_ActivateWeaponSet1 = 0x81,
            ControlAction_ActivateWeaponSet2,
            ControlAction_ActivateWeaponSet3,
            ControlAction_ActivateWeaponSet4,

            ControlAction_DropItem = 0xCD, // drops bundle item >> flags, ashes, etc

            // Chat
            ControlAction_CharReply = 0xBE,
            ControlAction_OpenChat = 0xA1,
            ControlAction_OpenAlliance = 0x88,

            ControlAction_ReverseCamera = 0x90,
            ControlAction_StrafeLeft = 0x91,
            ControlAction_StrafeRight = 0x92,
            ControlAction_TurnLeft = 0xA2,
            ControlAction_TurnRight = 0xA3,
            ControlAction_MoveBackward = 0xAC,
            ControlAction_MoveForward = 0xAD,
            ControlAction_CancelAction = 0xAF,
            ControlAction_Interact = 0x80,
            ControlAction_ReverseDirection = 0xB1,
            ControlAction_Autorun = 0xB7,
            ControlAction_Follow = 0xCC,

            // Targeting
            ControlAction_TargetPartyMember1 = 0x96,
            ControlAction_TargetPartyMember2,
            ControlAction_TargetPartyMember3,
            ControlAction_TargetPartyMember4,
            ControlAction_TargetPartyMember5,
            ControlAction_TargetPartyMember6,
            ControlAction_TargetPartyMember7,
            ControlAction_TargetPartyMember8,
            ControlAction_TargetPartyMember9 = 0xC6,
            ControlAction_TargetPartyMember10,
            ControlAction_TargetPartyMember11,
            ControlAction_TargetPartyMember12,

            ControlAction_TargetNearestItem = 0xC3,
            ControlAction_TargetNextItem = 0xC4,
            ControlAction_TargetPreviousItem = 0xC5,
            ControlAction_TargetPartyMemberNext = 0xCA,
            ControlAction_TargetPartyMemberPrevious = 0xCB,
            ControlAction_TargetAllyNearest = 0xBC,
            ControlAction_ClearTarget = 0xE3,
            ControlAction_TargetSelf = 0xA0, // also 0x96
            ControlAction_TargetPriorityTarget = 0x9F,
            ControlAction_TargetNearestEnemy = 0x93,
            ControlAction_TargetNextEnemy = 0x95,
            ControlAction_TargetPreviousEnemy = 0x9E,

            ControlAction_ShowOthers = 0x89,
            ControlAction_ShowTargets = 0x94,

            ControlAction_CameraZoomIn = 0xCE,
            ControlAction_CameraZoomOut = 0xCF,

            // Party/Hero commands
            ControlAction_ClearPartyCommands = 0xDB,
            ControlAction_CommandParty = 0xD6,
            ControlAction_CommandHero1,
            ControlAction_CommandHero2,
            ControlAction_CommandHero3,
            ControlAction_CommandHero4 = 0x102,
            ControlAction_CommandHero5,
            ControlAction_CommandHero6,
            ControlAction_CommandHero7,

            ControlAction_OpenHero1PetCommander = 0xE0,
            ControlAction_OpenHero2PetCommander,
            ControlAction_OpenHero3PetCommander,
            ControlAction_OpenHero4PetCommander = 0xFE,
            ControlAction_OpenHero5PetCommander,
            ControlAction_OpenHero6PetCommander,
            ControlAction_OpenHero7PetCommander,
            ControlAction_OpenHeroCommander1 = 0xDC,
            ControlAction_OpenHeroCommander2,
            ControlAction_OpenHeroCommander3,
            ControlAction_OpenHeroCommander4 = 0x126,
            ControlAction_OpenHeroCommander5,
            ControlAction_OpenHeroCommander6,
            ControlAction_OpenHeroCommander7,

            ControlAction_Hero1Skill1 = 0xE5,
            ControlAction_Hero1Skill2,
            ControlAction_Hero1Skill3,
            ControlAction_Hero1Skill4,
            ControlAction_Hero1Skill5,
            ControlAction_Hero1Skill6,
            ControlAction_Hero1Skill7,
            ControlAction_Hero1Skill8,
            ControlAction_Hero2Skill1,
            ControlAction_Hero2Skill2,
            ControlAction_Hero2Skill3,
            ControlAction_Hero2Skill4,
            ControlAction_Hero2Skill5,
            ControlAction_Hero2Skill6,
            ControlAction_Hero2Skill7,
            ControlAction_Hero2Skill8,
            ControlAction_Hero3Skill1,
            ControlAction_Hero3Skill2,
            ControlAction_Hero3Skill3,
            ControlAction_Hero3Skill4,
            ControlAction_Hero3Skill5,
            ControlAction_Hero3Skill6,
            ControlAction_Hero3Skill7,
            ControlAction_Hero3Skill8,
            ControlAction_Hero4Skill1 = 0x106,
            ControlAction_Hero4Skill2,
            ControlAction_Hero4Skill3,
            ControlAction_Hero4Skill4,
            ControlAction_Hero4Skill5,
            ControlAction_Hero4Skill6,
            ControlAction_Hero4Skill7,
            ControlAction_Hero4Skill8,
            ControlAction_Hero5Skill1,
            ControlAction_Hero5Skill2,
            ControlAction_Hero5Skill3,
            ControlAction_Hero5Skill4,
            ControlAction_Hero5Skill5,
            ControlAction_Hero5Skill6,
            ControlAction_Hero5Skill7,
            ControlAction_Hero5Skill8,
            ControlAction_Hero6Skill1,
            ControlAction_Hero6Skill2,
            ControlAction_Hero6Skill3,
            ControlAction_Hero6Skill4,
            ControlAction_Hero6Skill5,
            ControlAction_Hero6Skill6,
            ControlAction_Hero6Skill7,
            ControlAction_Hero6Skill8,
            ControlAction_Hero7Skill1,
            ControlAction_Hero7Skill2,
            ControlAction_Hero7Skill3,
            ControlAction_Hero7Skill4,
            ControlAction_Hero7Skill5,
            ControlAction_Hero7Skill6,
            ControlAction_Hero7Skill7,
            ControlAction_Hero7Skill8,

            // Skills
            ControlAction_UseSkill1 = 0xA4,
            ControlAction_UseSkill2,
            ControlAction_UseSkill3,
            ControlAction_UseSkill4,
            ControlAction_UseSkill5,
            ControlAction_UseSkill6,
            ControlAction_UseSkill7,
            ControlAction_UseSkill8

        };
        struct FloatingWindow {
            void* unk1; // Some kind of function call
            wchar_t* name;
            uint32_t unk2;
            uint32_t unk3;
            uint32_t save_preference; // 1 or 0; if 1, will save to UI layout preferences.
            uint32_t unk4;
            uint32_t unk5;
            uint32_t unk6;
            uint32_t window_id; // Maps to window array
        };
        static_assert(sizeof(FloatingWindow) == 0x24);

        struct CompassPoint {
            CompassPoint() : x(0), y(0) {}
            CompassPoint(short _x, short _y) : x(_x), y(_y) {}
            short x;
            short y;
        };

        enum class TooltipType : uint32_t {
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
            uint32_t unk0; // can use used as an enum in this case
            uint32_t unk1;
            uint32_t unk2;
            UI::TooltipType type() {
                // Without sniffing into each render function to determine the source, we have to guess based on the arguments passed.
                switch ((UI::TooltipType)unk0) {
                case UI::TooltipType::Item:
                    // 0x8 also used for attribute tooltips, title tooltips and more
                    if(payload[1] != 0xff) // NB: Item tooltip has 2 item_id values, second is always 0xff
                        return UI::TooltipType::None;
                    break;
                default:
                    break;
                }
                return static_cast<UI::TooltipType>(unk0);
            }
        };

        struct CreateUIComponentPacket {
            uint32_t frame_id;
            uint32_t component_flags;
            uint32_t tab_index;
            void* event_callback;
            wchar_t* name_enc;
            wchar_t* component_label;
        };

        // SendMessage for Guild Wars UI messages, most UI interactions will use this.
        GWCA_API bool SendUIMessage(UI::UIMessage msgid, void* wParam = nullptr, void* lParam = nullptr);

        GWCA_API bool Keydown(ControlAction key);
        GWCA_API bool Keyup(ControlAction key);
        GWCA_API bool Keypress(ControlAction key);

        GWCA_API UI::WindowPosition* GetWindowPosition(UI::WindowID window_id);
        GWCA_API bool SetWindowVisible(UI::WindowID window_id, bool is_visible);
        GWCA_API bool SetWindowPosition(UI::WindowID window_id, UI::WindowPosition* info);

        GWCA_API bool DrawOnCompass(unsigned session_id, unsigned pt_count, CompassPoint* pts);

        // Call from GameThread to be safe
        GWCA_API void LoadSettings(size_t size, uint8_t *data);
        GWCA_API ArrayByte* GetSettings();

        GWCA_API bool GetIsUIDrawn();
        GWCA_API bool GetIsShiftScreenShot();
        GWCA_API bool GetIsWorldMapShowing();

        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, wchar_t *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, char    *buffer, size_t size);
        GWCA_API void AsyncDecodeStr(const wchar_t* enc_str, DecodeStr_Callback callback, void* callback_param = 0, uint32_t language_id = -1);
        GWCA_API void AsyncDecodeStr(const wchar_t *enc_str, std::wstring *out, uint32_t language_id = -1);

        GWCA_API bool UInt32ToEncStr(uint32_t value, wchar_t *buffer, size_t count);
        GWCA_API uint32_t EncStrToUInt32(const wchar_t *enc_str);

        GWCA_API void SetOpenLinks(bool toggle);

        GWCA_API uint32_t GetPreference(EnumPreference pref);
        GWCA_API uint32_t GetPreferenceOptions(EnumPreference pref, uint32_t** options_out = 0);
        GWCA_API uint32_t GetPreference(NumberPreference pref);
        GWCA_API bool GetPreference(FlagPreference pref);
        GWCA_API wchar_t* GetPreference(StringPreference pref);
        GWCA_API bool SetPreference(EnumPreference pref, uint32_t value);
        GWCA_API bool SetPreference(NumberPreference pref, uint32_t value);
        GWCA_API bool SetPreference(FlagPreference pref, bool value);
        GWCA_API bool SetPreference(StringPreference pref, wchar_t* value);

        // Returns actual hard frame limit, factoring in vsync, monitor refresh rate and in-game preferences
        GWCA_API uint32_t GetFrameLimit();
        // Set a hard upper limit for frame rate. Actual limit may be lower (but not higher) depending on vsync/in-game preference
        GWCA_API bool SetFrameLimit(uint32_t value);

        //GWCA_API void SetPreference(Preference pref, uint32_t value);


        typedef HookCallback<uint32_t> KeyCallback;
        GWCA_API void RegisterKeydownCallback(
            HookEntry* entry,
            const KeyCallback& callback);
        GWCA_API void RemoveKeydownCallback(
            HookEntry* entry);
        GWCA_API void RegisterKeyupCallback(
            HookEntry* entry,
            const KeyCallback& callback);
        GWCA_API void RemoveKeyupCallback(
            HookEntry* entry);

        typedef HookCallback<UIMessage, void *, void *> UIMessageCallback;
        GWCA_API void RegisterUIMessageCallback(
            HookEntry *entry,
            UIMessage message_id,
            const UIMessageCallback& callback,
            int altitude = -0x8000);

        GWCA_API void RemoveUIMessageCallback(
            HookEntry *entry);

        GWCA_API TooltipInfo* GetCurrentTooltip();

        typedef std::function<void (CreateUIComponentPacket*)> CreateUIComponentCallback;
        GWCA_API void RegisterCreateUIComponentCallback(
            HookEntry *entry,
            const CreateUIComponentCallback& callback);

        GWCA_API void RemoveCreateUIComponentCallback(
            HookEntry *entry);

    }
}
