#pragma once

#include <Windows.h>
#include <string>
#include <Windows.h>

#include "Skills.h"
#include "Maps.h"
#include "ItemIDs.h"
#include "AgentIDs.h"
#include "QuestIDs.h"

namespace GW {
	namespace Constants {
		enum class Difficulty { Normal, Hard };

		enum class InstanceType { Outpost, Explorable, Loading };

		enum class Profession {
			None, Warrior, Ranger, Monk, Necromancer, Mesmer,
			Elementalist, Assassin, Ritualist, Paragon, Dervish
		};
		static std::string GetProfessionAcronym(Profession prof) {
			switch (prof) {
			case GW::Constants::Profession::None: return "X";
			case GW::Constants::Profession::Warrior: return "W";
			case GW::Constants::Profession::Ranger: return "R";
			case GW::Constants::Profession::Monk: return "Mo";
			case GW::Constants::Profession::Necromancer: return "N";
			case GW::Constants::Profession::Mesmer: return "Me";
			case GW::Constants::Profession::Elementalist: return "E";
			case GW::Constants::Profession::Assassin: return "A";
			case GW::Constants::Profession::Ritualist: return "Rt";
			case GW::Constants::Profession::Paragon: return "P";
			case GW::Constants::Profession::Dervish: return "D";
			default: return "";
			}
		}
		static std::wstring GetWProfessionAcronym(Profession prof) {
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
			FastCasting, IllusionMagic, DominationMagic, InspirationMagic,		// mesmer
			BloodMagic, DeathMagic, SoulReaping, Curses,						// necro
			AirMagic, EarthMagic, FireMagic, WaterMagic, EnergyStorage,			// ele
			HealingPrayers, SmitingPrayers, ProtectionPrayers, DivineFavor,		// monk
			Strength, AxeMastery, HammerMastery, Swordsmanship, Tactics,		// warrior
			BeastMastery, Expertise, WildernessSurvival, Marksmanship,			// ranger
			DaggerMastery = 29, DeadlyArts, ShadowArts,							// assassin (most)
			Communing, RestorationMagic, ChannelingMagic,						// ritualist (most)
			CriticalStrikes, SpawningPower,										// sin/rit primary (gw is weird)
			SpearMastery, Command, Motivation, Leadership,						// paragon
			ScytheMastery, WindPrayers, EarthPrayers, Mysticism,				// derv
			None = 0xff
		};

		enum class OnlineStatus { OFFLINE, ONLINE, DO_NOT_DISTURB, AWAY };

		enum class Bag {
			None, Backpack, Belt_Pouch, Bag_1, Bag_2, Equipment_Pack,
			Material_Storage, Unclaimed_Items, Storage_1, Storage_2,
			Storage_3, Storage_4, Storage_5, Storage_6, Storage_7,
			Storage_8, Storage_9, Equipped_Items
		};

		enum class AgentType {
			Living = 0xDB, Chest_Signpost = 0x200, Item = 0x400
		};

		enum class ItemType {
			Salvage, Axe = 2, Bag, Boots, Bow, Chestpiece = 7, Rune_Mod, Usable, Dye,
			Materials_Zcoins, Offhand, Gloves, Hammer = 15, Headpiece, CC_Shards,
			Key, Leggings, Gold_Coin, Quest_Item, Wand, Shield = 24, Staff = 26, Sword,
			Kit = 29, Trophy, Scroll, Daggers, Minipet, Scythe, Spear, Costume = 45
		};

		enum HeroID : DWORD {
			Norgu = 1, Goren, Tahlkora, MasterOfWhispers, AcolyteJin, Koss,
			Dunkoro, AcolyteSousuke, Melonni, ZhedShadowhoof, GeneralMorgahn,
			MargridTheSly, Zenmai, Olias, Razah, MOX, Jora = 18, PyreFierceshot,
			Anton, Livia, Hayda, Kahmu, Gwen, Xandra, Vekk, Ogden, Merc1, Merc2, Merc3,
			Merc4, Merc5, Merc6, Merc7, Merc8, Miku, ZeiRi
		};

		enum HeroState : DWORD {
			Fight, Guard, AvoidCombat
		};

		enum TitleID : DWORD {
			Hero, TyrianCarto, CanthanCarto, Gladiator, Champion, Kurzick,
			Luxon, Drunkard, Survivor = 0x9, KoaBD, ProtectorTyria = 0xD,
			ProtectorCantha, Lucky, Unlucky, Sunspear, ElonianCarto,
			ProtectorElona, Lightbringer, LDoA, Commander, Gamer,
			SkillHunterTyria, VanquisherTyria, SkillHunterCantha,
			VanquisherCantha, SkillHunterElona, VanquisherElona,
			LegendaryCarto, LegendaryGuardian, LegendarySkillHunter,
			LegendaryVanquisher, Sweets, GuardianTyria, GuardianCantha,
			GuardianElona, Asuran, Deldrimor, Vanguard, Norn, MasterOfTheNorth,
			Party, Zaishen, TreasureHunter, Wisdom, Codex
		};

		enum class Tick { NOT_READY, READY };

		enum class InterfaceSize { SMALL = 0, NORMAL, LARGE, LARGER };
		namespace HealthbarHeight {
			const int Small = 25;
			const int Normal = 22;
			const int Large = 28;
			const int Larger = 31;
		}

		// travel, region, districts
		namespace Region { // in-game region code
			const int International = -2;
			const int America = 0;
			const int Korea = 1;
			const int Europe = 2;
			const int China = 3;
			const int Japan = 4;
		};

		namespace EuropeLanguage { // in-game language code for the european region
			const int English = 0;
			const int French = 2;
			const int German = 3;
			const int Italian = 4;
			const int Spanish = 5;
			const int Polish = 9;
			const int Russian = 10;
		};

		enum class District { // arbitrary enum for game district
			Current,
			International,
			American,
			EuropeEnglish,
			EuropeFrench,
			EuropeGerman,
			EuropeItalian,
			EuropeSpanish,
			EuropePolish,
			EuropeRussian,
			AsiaKorean,
			AsiaChinese,
			AsiaJapanese,
		};


		namespace Range {
			const float Adjacent = 166.0f;
			const float Nearby = 238.0f;
			const float Area = 322.0f;
			const float Earshot = 1010.0f;
			const float Spellcast = 1246.0f;
			const float Spirit = 2500.0f;
			const float Compass = 5000.0f;
		};

		namespace SqrRange {
			const float Adjacent = Range::Adjacent * Range::Adjacent;
			const float Nearby = Range::Nearby * Range::Nearby;
			const float Area = Range::Area * Range::Area;
			const float Earshot = Range::Earshot * Range::Earshot;
			const float Spellcast = Range::Spellcast * Range::Spellcast;
			const float Spirit = Range::Spirit * Range::Spirit;
			const float Compass = Range::Compass * Range::Compass;
		};

		namespace DialogID {
			const int UwTeleVale = 138;
			const int UwTelePlanes = 139;
			const int UwTeleWastes = 140;
			const int UwTeleLab = 141;
			const int UwTeleMnt = 142;
			const int UwTelePits = 143;
			const int UwTelePools = 144;

			const int FowCraftArmor = 127;

			const int FerryKamadanToDocks = 133; // Assistant Hahnna
			const int FerryDocksToKaineng = 136; // Mhenlo
			const int FerryDocksToLA = 137;		 // Mhenlo
			const int FerryGateToLA = 133;		 // Lionguard Neiro

			// Profession Changer Dialogs.
			const int ProfChangeWarrior = 0x184;
			const int ProfChangeRanger = 0x284;
			const int ProfChangeMonk = 0x384;
			const int ProfChangeNecro = 0x484;
			const int ProfChangeMesmer = 0x584;
			const int ProfChangeEle = 0x684;
			const int ProfChangeAssassin = 0x784;
			const int ProfChangeRitualist = 0x884;
			const int ProfChangeParagon = 0x984;
			const int ProfChangeDervish = 0xA84;
		}

		namespace Camera {
			const float FIRST_PERSON_DIST = 2.f;
			const float DEFAULT_DIST = 750.f;
		}
	}
}
