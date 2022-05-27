#pragma once
#include <GWCA/Constants/Chat.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {
    namespace UI {

        struct ChatTemplate {
            uint32_t        unk0;
            uint32_t        type; // 0 = build, 1 = equipement
            Array<wchar_t>  code;
            wchar_t* name;
        };

        struct UIChatMessage {
            Chat::Channel channel;
            wchar_t* message;
            Chat::Channel channel2;
        };

        struct ChangeTargetUIMsg {
            AgentID        manual_target_id;
            uint32_t        unk1;
            AgentID        auto_target_id;
            uint32_t        unk2;
            // ...
        };

        struct AgentNameTagInfo {
            /* +h0000 */ AgentID agent_id;
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


        struct DialogBodyInfo {
            uint32_t type;
            AgentID agent_id;
            wchar_t* message_enc;
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
            kShowAgentNameTag = 0x10000000 | 0x19, // wparam = AgentNameTagInfo*
            kHideAgentNameTag = 0x10000000 | 0x1A,
            kSetAgentNameTagAttribs = 0x10000000 | 0x1B, // wparam = AgentNameTagInfo*
            kChangeTarget = 0x10000000 | 0x20, // wparam = ChangeTargetUIMsg*
            kShowXunlaiChest = 0x10000000 | 0x40,

            kMoraleChange = 0x10000000 | 0x47, // wparam = {agent id, morale percent }
            kEffectAdd = 0x10000000 | 0x55, // wparam = {agent_id, GW::Effect*}
            kEffectRenew = 0x10000000 | 0x56, // wparam = GW::Effect*
            kEffectRemove = 0x10000000 | 0x57, // wparam = effect id
            kWriteToChatLog = 0x10000000 | 0x7E,
            kFriendUpdated = 0x10000000 | 0x89, // wparam = { GW::Friend*, ... }
            kMapLoaded = 0x10000000 | 0x8A,
            kOpenWhisper = 0x10000000 | 0x90, // wparam = wchar* name
            kLogout = 0x10000000 | 0x9b, // wparam = { bool unknown, bool character_select } 
            kDialogBody = 0x10000000 | 0xA4, // wparam = DialogBodyInfo*
            kDialogButton = 0x10000000 | 0xA1, // wparam = button info, undocumented atm
            kQuotedItemPrice = 0x10000000 | 0xBB, // wparam = { uint32_t item_id, uint32_t price }
            kStartMapLoad = 0x10000000 | 0xC0, // wparam = { uint32_t map_id, ...}
            kWorldMapUpdated = 0x10000000 | 0xC5, // Triggered when an area in the world map has been discovered/updated
            kGuildMemberUpdated = 0x10000000 | 0xD8, // wparam = { wchar_t* name, uint32_t unk0, uint32_t status }
            kShowHint = 0x10000000 | 0xDF, // wparam = { uint32_t icon_type, wchar_t* message_enc }
            kPvPWindowContent = 0x10000000 | 0xF8,
            kMapChange = 0x10000000 | 0x10F, // wparam = map id
            kCheckboxPreference = 0x10000000 | 0x13F,
            kPreferenceChanged = 0x10000000 | 0x140,
            kUIPositionChanged = 0x10000000 | 0x141,
            kQuestAdded = 0x10000000 | 0x149, // wparam = { quest_id, ... }
            kCurrentQuestChanged = 0x10000000 | 0x14C, // wparam = { quest_id, ... }
            kObjectiveComplete = 0x10000000 | 0x156, // wparam = { objective_id, ... }
            kDestroyUIObject = 0x10000000 | 0x170, // Undocumented
            kGuildHall = 0x10000000 | 0x177, // wparam = gh key (uint32_t[4])
            kLeaveGuildHall = 0x10000000 | 0x179,
            kTravel = 0x10000000 | 0x17A,
            kOpenWikiUrl = 0x10000000 | 0x17B, // wparam = url
            kHideHeroPanel = 0x10000000 | 0x197, // wparam = hero_id
            kShowHeroPanel = 0x10000000 | 0x198, // wparam = hero_id
            kMoveItem = 0x10000000 | 0x19e, // wparam = { item_id, to_bag, to_slot, bool prompt }
            kOpenTemplate = 0x10000000 | 0x1B9,
        };

        enum class Preference : uint32_t {
            // preferences_array
            CharSortOrder = 0x0, // Unused
            AntiAliasing = 0x1, // multi sampling
            Reflections = 0x2,
            ShaderQuality = 0x3,
            ShadowQuality = 0x4,
            TerrainQuality = 0x5,
            InterfaceSize = 0x6,
            FrameLimiter = 0x7,

            // more_preferences_array | 0x800
            ChatState = 0x801, // 1 == showing chat window, 0 = not showing chat window
            ChatTab = 0x802,
            DistrictLastVisitedLanguage = 0x803,
            DistrictLastVisitedLanguage2 = 0x804,
            DistrictLastVisitedNonInternationalLanguage = 0x805,
            DistrictLastVisitedNonInternationalLanguage2 = 0x806,
            DamageTextSize = 0x807, // 0 to 100
            FullscreenGamma = 0x808, // 0 to 100
            InventoryBag = 0x809, // Selected bag in inventory window
            TextLanguage = 0x80A,
            AudioLanguage = 0x80B,
            ChatFilterLevel = 0x80C,
            SkillListSortMethod = 0x811,
            SkillListViewMode = 0x812,
            SoundQuality = 0x813, // 0 to 100
            StorageBagPage = 0x814,
            TextureQuality = 0x816, // TextureLod
            UseBestTextureFiltering = 0x817,
            EffectsVolume = 0x818, // 0 to 100
            DialogVolume = 0x819, // 0 to 100
            BackgroundVolume = 0x81A, // 0 to 100
            MusicVolume = 0x81B, // 0 to 100
            UIVolume = 0x81C, // 0 to 100
            WindowPosX = 0x81E,
            WindowPosY = 0x81F,
            WindowSizeX = 0x820,
            WindowSizeY = 0x821,
            SealedSeed = 0x822, // Used in codex arena
            SealedCount = 0x823, // Used in codex arena
            FieldOfView = 0x824, // 0 to 100
            CameraRotationSpeed = 0x825, // 0 to 100
            ScreenBorderless = 0x826, // 0x1 = Windowed Borderless, 0x2 = Windowed Fullscreen
            MasterVolume = 0x827, // 0 to 100
            ClockMode = 0x828,

            // checkbox preference array | 0x8000 ( count = 0x5d)
            ChannelAlliance = 0x8004,
            ChannelEmotes = 0x8006,
            ChannelGuild = 0x8007,
            ChannelLocal = 0x8008,
            ChannelGroup = 0x8009,
            ChannelTrade = 0x800A,
            ShowTextInSkillFloaters = 0x8011,
            InvertMouseControlOfCamera = 0x8016,
            DisableMouseWalking = 0x8017,
            ConciseSkillDescriptions = 0x8032,
            DoNotShowSkillTipsOnEffectMonitor = 0x8033,
            DoNotShowSkillTipsOnSkillBars = 0x8034,
            AutoTargetFoes = 0x8039,
            AutoTargetNPCs = 0x803a,
            AlwaysShowNearbyNamesPvP = 0x803b,
            FadeDistantNameTags = 0x803c,
            DoNotCloseWindowsOnEscape = 0x8045,
            WaitForVSync = 0x8054,
            WhispersFromFriendsEtcOnly = 0x8055,
            ShowChatTimestamps = 0x8056,
            ShowCollapsedBags = 0x8057,
            ItemRarityBorder = 0x8058,
            AlwaysShowAllyNames = 0x8059,
            AlwaysShowFoeNames = 0x805a,
            LockCompassRotation = 0x805c
        };
        // Used with GetWindowPosition
        enum class WindowID : uint32_t {
            Dialogue1 = 0x0,
            Dialogue2 = 0x1,
            MissionGoals = 0x2,
            DropBundle = 0x3,
            Chat = 0x4,
            InGameClock = 0x6,
            Compass = 0x7,
            DamageMonitor = 0x8,
            PerformanceMonitor = 0xB,
            EffectsMonitor = 0xC,
            Hints = 0xD,
            MissionStatusAndScoreDisplay = 0xF,
            Notifications = 0x11,
            Skillbar = 0x14,
            SkillMonitor = 0x15,
            UpkeepMonitor = 0x17,
            SkillWarmup = 0x18,
            Menu = 0x1A,
            EnergyBar = 0x1C,
            ExperienceBar = 0x1D,
            HealthBar = 0x1E,
            TargetDisplay = 0x1F,
            MissionProgress = 0xE,
            TradeButton = 0x21,
            WeaponBar = 0x22,
            Hero1 = 0x33,
            Hero2 = 0x34,
            Hero3 = 0x35,
            Hero = 0x36,
            SkillsAndAttributes = 0x38,
            Friends = 0x3A,
            Guild = 0x3B,
            Help = 0x3D,
            Inventory = 0x3E,
            VaultBox = 0x3F,
            InventoryBags = 0x40,
            MissionMap = 0x42,
            Observe = 0x44,
            Options = 0x45,
            PartyWindow = 0x48, // NB: state flag is ignored for party window, but position is still good
            PartySearch = 0x49,
            QuestLog = 0x4F,
            Merchant = 0x5C,
            Hero4 = 0x5E,
            Hero5 = 0x5F,
            Hero6 = 0x60,
            Hero7 = 0x61,
            Count = 0x66
        };

        enum class ControlAction : uint32_t {
            None = 0,
            Screenshot = 0xAE,
            // Panels
            CloseAllPanels = 0x85,
            ToggleInventoryWindow = 0x8B,
            OpenScoreChart = 0xBD,
            OpenTemplateManager = 0xD3,
            OpenSaveEquipmentTemplate = 0xD4,
            OpenSaveSkillTemplate = 0xD5,
            OpenParty = 0xBF,
            OpenGuild = 0xBA,
            OpenFriends = 0xB9,
            ToggleAllBags = 0xB8,
            OpenMissionMap = 0xB6,
            OpenBag2 = 0xB5,
            OpenBag1 = 0xB4,
            OpenBelt = 0xB3,
            OpenBackpack = 0xB2,
            OpenSkillsAndAttributes = 0x8F,
            OpenQuestLog = 0x8E,
            OpenWorldMap = 0x8C,
            OpenHero = 0x8A,

            // Weapon sets
            CycleEquipment = 0x86,
            ActivateWeaponSet1 = 0x81,
            ActivateWeaponSet2,
            ActivateWeaponSet3,
            ActivateWeaponSet4,

            DropItem = 0xCD, // drops bundle item >> flags, ashes, etc

            // Chat
            CharReply = 0xBE,
            OpenChat = 0xA1,
            OpenAlliance = 0x88,

            ReverseCamera = 0x90,
            StrafeLeft = 0x91,
            StrafeRight = 0x92,
            TurnLeft = 0xA2,
            TurnRight = 0xA3,
            MoveBackward = 0xAC,
            MoveForward = 0xAD,
            CancelAction = 0xAF,
            Interact = 0x80,
            ReverseDirection = 0xB1,
            Autorun = 0xB7,
            Follow = 0xCC,

            // Targeting
            TargetPartyMember1 = 0x96,
            TargetPartyMember2,
            TargetPartyMember3,
            TargetPartyMember4,
            TargetPartyMember5,
            TargetPartyMember6,
            TargetPartyMember7,
            TargetPartyMember8,
            TargetPartyMember9 = 0xC6,
            TargetPartyMember10,
            TargetPartyMember11,
            TargetPartyMember12,

            TargetNearestItem = 0xC3,
            TargetNextItem = 0xC4,
            TargetPreviousItem = 0xC5,
            TargetPartyMemberNext = 0xCA,
            TargetPartyMemberPrevious = 0xCB,
            TargetAllyNearest = 0xBC,
            ClearTarget = 0xE3,
            TargetSelf = 0xA0, // also 0x96
            TargetPriorityTarget = 0x9F,
            TargetNearestEnemy = 0x93,
            TargetNextEnemy = 0x95,
            TargetPreviousEnemy = 0x9E,

            ShowOthers = 0x89,
            ShowTargets = 0x94,

            CameraZoomIn = 0xCE,
            CameraZoomOut = 0xCF,

            // Party/Hero commands
            ClearPartyCommands = 0xDB,
            CommandParty = 0xD6,
            CommandHero1,
            CommandHero2,
            CommandHero3,
            CommandHero4 = 0x102,
            CommandHero5,
            CommandHero6,
            CommandHero7,

            OpenHero1PetCommander = 0xE0,
            OpenHero2PetCommander,
            OpenHero3PetCommander,
            OpenHero4PetCommander = 0xFE,
            OpenHero5PetCommander,
            OpenHero6PetCommander,
            OpenHero7PetCommander,
            OpenHeroCommander1 = 0xDC,
            OpenHeroCommander2,
            OpenHeroCommander3,
            OpenHeroCommander4 = 0x126,
            OpenHeroCommander5,
            OpenHeroCommander6,
            OpenHeroCommander7,

            Hero1Skill1 = 0xE5,
            Hero1Skill2,
            Hero1Skill3,
            Hero1Skill4,
            Hero1Skill5,
            Hero1Skill6,
            Hero1Skill7,
            Hero1Skill8,
            Hero2Skill1,
            Hero2Skill2,
            Hero2Skill3,
            Hero2Skill4,
            Hero2Skill5,
            Hero2Skill6,
            Hero2Skill7,
            Hero2Skill8,
            Hero3Skill1,
            Hero3Skill2,
            Hero3Skill3,
            Hero3Skill4,
            Hero3Skill5,
            Hero3Skill6,
            Hero3Skill7,
            Hero3Skill8,
            Hero4Skill1 = 0x106,
            Hero4Skill2,
            Hero4Skill3,
            Hero4Skill4,
            Hero4Skill5,
            Hero4Skill6,
            Hero4Skill7,
            Hero4Skill8,
            Hero5Skill1,
            Hero5Skill2,
            Hero5Skill3,
            Hero5Skill4,
            Hero5Skill5,
            Hero5Skill6,
            Hero5Skill7,
            Hero5Skill8,
            Hero6Skill1,
            Hero6Skill2,
            Hero6Skill3,
            Hero6Skill4,
            Hero6Skill5,
            Hero6Skill6,
            Hero6Skill7,
            Hero6Skill8,
            Hero7Skill1,
            Hero7Skill2,
            Hero7Skill3,
            Hero7Skill4,
            Hero7Skill5,
            Hero7Skill6,
            Hero7Skill7,
            Hero7Skill8,

            // Skills
            UseSkill1 = 0xA4,
            UseSkill2,
            UseSkill3,
            UseSkill4,
            UseSkill5,
            UseSkill6,
            UseSkill7,
            UseSkill8

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


    }
}