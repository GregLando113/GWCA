#pragma once
#include "Types.h"

namespace GW {
    typedef uint32_t Color;
    namespace Constants {
        enum class Campaign { Core, Prophecies, Factions, Nightfall, EyeOfTheNorth, BonusMissionPack };
        enum class Difficulty { Normal, Hard };

        enum class InstanceType { Outpost, Explorable, Loading };

        enum class Profession {
            None, Warrior, Ranger, Monk, Necromancer, Mesmer,
            Elementalist, Assassin, Ritualist, Paragon, Dervish
        };
        static char* GetProfessionAcronym(Profession prof) {
            switch (prof) {
            case Profession::None: return "X";
            case Profession::Warrior: return "W";
            case Profession::Ranger: return "R";
            case Profession::Monk: return "Mo";
            case Profession::Necromancer: return "N";
            case Profession::Mesmer: return "Me";
            case Profession::Elementalist: return "E";
            case Profession::Assassin: return "A";
            case Profession::Ritualist: return "Rt";
            case Profession::Paragon: return "P";
            case Profession::Dervish: return "D";
            default: return "";
            }
        }
        static wchar_t* GetWProfessionAcronym(Profession prof) {
            switch (prof) {
            case GW::Constants::Profession::None: return L"X";
            case GW::Constants::Profession::Warrior: return L"W";
            case GW::Constants::Profession::Ranger: return L"R";
            case GW::Constants::Profession::Monk: return L"Mo";
            case GW::Constants::Profession::Necromancer: return L"N";
            case GW::Constants::Profession::Mesmer: return L"Me";
            case GW::Constants::Profession::Elementalist: return L"E";
            case GW::Constants::Profession::Assassin: return L"A";
            case GW::Constants::Profession::Ritualist: return L"Rt";
            case GW::Constants::Profession::Paragon: return L"P";
            case GW::Constants::Profession::Dervish: return L"D";
            default: return L"";
            }
        }

        enum class Attribute {
            FastCasting, IllusionMagic, DominationMagic, InspirationMagic,      // mesmer
            BloodMagic, DeathMagic, SoulReaping, Curses,                        // necro
            AirMagic, EarthMagic, FireMagic, WaterMagic, EnergyStorage,         // ele
            HealingPrayers, SmitingPrayers, ProtectionPrayers, DivineFavor,     // monk
            Strength, AxeMastery, HammerMastery, Swordsmanship, Tactics,        // warrior
            BeastMastery, Expertise, WildernessSurvival, Marksmanship,          // ranger
            DaggerMastery = 29, DeadlyArts, ShadowArts,                         // assassin (most)
            Communing, RestorationMagic, ChannelingMagic,                       // ritualist (most)
            CriticalStrikes, SpawningPower,                                     // sin/rit primary (gw is weird)
            SpearMastery, Command, Motivation, Leadership,                      // paragon
            ScytheMastery, WindPrayers, EarthPrayers, Mysticism,                // derv
            None = 0xff
        };

        enum class OnlineStatus { OFFLINE, ONLINE, DO_NOT_DISTURB, AWAY };


		// Order of storage panes.
        enum class StoragePane {
            Storage_1, Storage_2, Storage_3, Storage_4, Storage_5,
            Storage_6, Storage_7, Storage_8, Storage_9, Storage_10,
            Storage_11, Storage_12, Storage_13, Storage_14, Material_Storage
        };

        enum class HeroID {
            NoHero, Norgu, Goren, Tahlkora, MasterOfWhispers, AcolyteJin, Koss,
            Dunkoro, AcolyteSousuke, Melonni, ZhedShadowhoof, GeneralMorgahn,
            MargridTheSly, Zenmai, Olias, Razah, MOX, KeiranThackeray, Jora,
            PyreFierceshot, Anton, Livia, Hayda, Kahmu, Gwen, Xandra, Vekk,
            Ogden, Merc1, Merc2, Merc3, Merc4, Merc5, Merc6, Merc7, Merc8,
            Miku, ZeiRi
        };

        constexpr Profession HeroProfs[] = {
            Profession::None,
            Profession::Mesmer, // Norgu
            Profession::Warrior,// Goren
            Profession::Monk, // Tahlkora
            Profession::Necromancer, // Master Of Whispers
            Profession::Ranger, // Acolyte Jin
            Profession::Warrior, // Koss
            Profession::Monk, // Dunkoro
            Profession::Elementalist, // Acolyte Sousuke
            Profession::Dervish, // Melonni
            Profession::Elementalist, // Zhed Shadowhoof
            Profession::Paragon, // General Morgahn
            Profession::Ranger, // Magrid The Sly
            Profession::Assassin, // Zenmai
            Profession::Necromancer, // Olias
            Profession::None, // Razah
            Profession::Dervish, // MOX
            Profession::Paragon, // Keiran Thackeray
            Profession::Warrior, // Jora
            Profession::Ranger, // Pyre Fierceshot
            Profession::Assassin, // Anton
            Profession::Necromancer, // Livia
            Profession::Paragon, // Hayda
            Profession::Dervish, // Kahmu
            Profession::Mesmer, // Gwen
            Profession::Ritualist, // Xandra
            Profession::Elementalist, // Vekk
            Profession::Monk, // Ogden
            Profession::None, // Mercenary Hero 1
            Profession::None, // Mercenary Hero 2
            Profession::None, // Mercenary Hero 3
            Profession::None, // Mercenary Hero 4
            Profession::None, // Mercenary Hero 5
            Profession::None, // Mercenary Hero 6
            Profession::None, // Mercenary Hero 7
            Profession::None, // Mercenary Hero 8
            Profession::Assassin, // Miku
            Profession::Ritualist, // Zei Ri
        };

        enum class HeroBehavior {
            Fight, Guard, AvoidCombat
        };

        enum class TitleID {
            Hero, TyrianCarto, CanthanCarto, Gladiator, Champion, Kurzick,
            Luxon, Drunkard,
            Deprecated_SkillHunter, // Pre hard mode update version
            Survivor, KoaBD,
            Deprecated_TreasureHunter, // Old title, non-account bound
            Deprecated_Wisdom, // Old title, non-account bound
            ProtectorTyria,
            ProtectorCantha, Lucky, Unlucky, Sunspear, ElonianCarto,
            ProtectorElona, Lightbringer, LDoA, Commander, Gamer,
            SkillHunterTyria, VanquisherTyria, SkillHunterCantha,
            VanquisherCantha, SkillHunterElona, VanquisherElona,
            LegendaryCarto, LegendaryGuardian, LegendarySkillHunter,
            LegendaryVanquisher, Sweets, GuardianTyria, GuardianCantha,
            GuardianElona, Asuran, Deldrimor, Vanguard, Norn, MasterOfTheNorth,
            Party, Zaishen, TreasureHunter, Wisdom, Codex,

            None = -1
        };

        enum class Tick { NOT_READY, READY };

        enum class InterfaceSize { SMALL = 0, NORMAL, LARGE, LARGER };
        namespace HealthbarHeight {
            constexpr size_t Small = 24;
            constexpr size_t Normal = 22;
            constexpr size_t Large = 26;
            constexpr size_t Larger = 30;
        };



        namespace Range {
            constexpr float Adjacent = 166.0f;
            constexpr float Nearby = 252.0f;
            constexpr float Area = 322.0f;
            constexpr float Earshot = 1012.0f;
            constexpr float Spellcast = 1248.0f;
            constexpr float Spirit = 2500.0f;
            constexpr float Compass = 5000.0f;
        };

        namespace SqrRange {
            constexpr float Adjacent = Range::Adjacent * Range::Adjacent;
            constexpr float Nearby = Range::Nearby * Range::Nearby;
            constexpr float Area = Range::Area * Range::Area;
            constexpr float Earshot = Range::Earshot * Range::Earshot;
            constexpr float Spellcast = Range::Spellcast * Range::Spellcast;
            constexpr float Spirit = Range::Spirit * Range::Spirit;
            constexpr float Compass = Range::Compass * Range::Compass;  
        };

        enum class DialogID {
            UwTeleVale = 138,
            UwTelePlanes = 139,
            UwTeleWastes = 140,
            UwTeleLab = 141,
            UwTeleMnt = 142,
            UwTelePits = 143,
            UwTelePools = 144,

            FowCraftArmor = 127,

            FerryKamadanToDocks = 133, // Assistant Hahnna
            FerryDocksToKaineng = 136, // Mhenlo
            FerryDocksToLA = 137,      // Mhenlo
            FerryGateToLA = 133,       // Lionguard Neiro

            // Profession Changer Dialogs.
            ProfChangeWarrior = 0x184,
            ProfChangeRanger = 0x284,
            ProfChangeMonk = 0x384,
            ProfChangeNecro = 0x484,
            ProfChangeMesmer = 0x584,
            ProfChangeEle = 0x684,
            ProfChangeAssassin = 0x784,
            ProfChangeRitualist = 0x884,
            ProfChangeParagon = 0x984,
            ProfChangeDervish = 0xA84,

            FactionMissionOutpost = 0x80000B,
            NightfallMissionOutpost = 0x85,
        };

        enum class EffectID {
            black_cloud = 1,
            mesmer_symbol = 4,
            green_cloud = 7,
            green_sparks = 8,
            necro_symbol = 9,
            ele_symbol = 11,
            white_clouds = 13,
            monk_symbol = 18,
            bleeding = 23,
            blind = 24,
            burning = 25,
            disease = 26,
            poison = 27,
            dazed = 28,
            weakness = 29, //cracked_armor has same EffectID
            assasin_symbol = 34,
            ritualist_symbol = 35,
            dervish_symbol = 36,
        };

        namespace Camera {
            constexpr float FIRST_PERSON_DIST = 2.f;
            constexpr float DEFAULT_DIST = 750.f;
        }
    }
}