#pragma once

#include <Windows.h>
#include <string>
#include <Windows.h>

#include "GwConstantsSkills.h"
#include "GwConstantsMaps.h"

namespace GwConstants {

	enum class Difficulty { Normal, Hard };

	enum class InstanceType { Outpost, Explorable, Loading };

	enum class Profession {
		None, Warrior, Ranger, Monk, Necromancer, Mesmer,
		Elementalist, Assassin, Ritualist, Paragon, Dervish
	};
	static std::wstring to_wstring(Profession prof) {
		switch (prof) {
		case GwConstants::Profession::None: return L"X";
		case GwConstants::Profession::Warrior: return L"W";
		case GwConstants::Profession::Ranger: return L"R";
		case GwConstants::Profession::Monk: return L"Mo";
		case GwConstants::Profession::Necromancer: return L"N";
		case GwConstants::Profession::Mesmer: return L"Me";
		case GwConstants::Profession::Elementalist: return L"E";
		case GwConstants::Profession::Assassin: return L"A";
		case GwConstants::Profession::Ritualist: return L"Rt";
		case GwConstants::Profession::Paragon: return L"P";
		case GwConstants::Profession::Dervish: return L"D";
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

	enum class OnlineStatus{ OFFLINE, ONLINE, DO_NOT_DISTURB, AWAY };

	enum class Bag{	None,Backpack,Belt_Pouch,Bag_1,Bag_2,Equipment_Pack,
					Material_Storage,Unclaimed_Items,Storage_1,Storage_2,
					Storage_3,Storage_4,Storage_5,Storage_6,Storage_7,
					Storage_8,Storage_9,Equipped_Items};

	enum class AgentType {
		Living = 0xDB,Chest_Signpost = 0x200,Item = 0x400
	};

	enum class ItemType {
		Salvage,Axe=2,Bag,Boots,Bow,Chestpiece=7,Rune_Mod,Usable,Dye,
		Materials_Zcoins,Offhand,Gloves,Hammer=15,Headpiece,CC_Shards,
		Key,Leggings,Gold_Coin,Quest_Item,Wand,Shield=24,Staff=26,Sword,
		Kit=29,Trophy,Scroll,Daggers,Minipet,Scythe,Spear,Costume=45
	};

	enum HeroID : DWORD {
		Norgu = 1,Goren,Tahlkora,MasterOfWhispers,AcolyteJin,Koss,
		Dunkoro,AcolyteSousuke,Melonni,ZhedShadowhoof,GeneralMorgahn,
		MargridTheSly,Zenmai,Olias,Razah,MOX,Jora = 18,PyreFierceshot,
		Anton,Livia,Hayda,Kahmu,Gwen,Xandra,Vekk,Ogden,Merc1,Merc2,Merc3,
		Merc4,Merc5,Merc6,Merc7,Merc8,Miku,ZeiRi
	};

	enum HeroState : DWORD {
		Fight,Guard,AvoidCombat
	};

	enum TitleID : DWORD {
		Hero,TyrianCarto,CanthanCarto,Gladiator,Champion,Kurzick,
		Luxon,Drunkard,Survivor = 0x9,KoaBD,ProtectorTyria = 0xD,
		ProtectorCantha,Lucky,Unlucky,Sunspear,ElonianCarto,
		ProtectorElona,Lightbringer,LDoA,Commander,Gamer,
		SkillHunterTyria,VanquisherTyria,SkillHunterCantha,
		VanquisherCantha,SkillHunterElona,VanquisherElona,
		LegendaryCarto,LegendaryGuardian,LegendarySkillHunter,
		LegendaryVanquisher,Sweets,GuardianTyria,GuardianCantha,
		GuardianElona,Asuran,Deldrimor,Vanguard,Norn,MasterOfTheNorth,
		Party,Zaishen,TreasureHunter,Wisdom,Codex
	};

	enum class Tick { NOT_READY, READY };

	enum class InterfaceSize { SMALL = -1, NORMAL, LARGE, VERYLARGE };
	namespace HealthbarHeight {
		const int Small = 25;
		const int Normal = 22;
		const int Large = 28;
		const int VeryLarge = 31;
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
		const int Adjacent = 166;
		const int Nearby = 238;
		const int Area = 322;
		const int Earshot = 1010;	
		const int Spellcast = 1246;
		const int Spirit = 2500;
		const int Compass = 5000;
	};

	namespace SqrRange {
		const int Adjacent = Range::Adjacent * Range::Adjacent;
		const int Nearby = Range::Nearby * Range::Nearby;
		const int Area = Range::Area * Range::Area;
		const int Earshot = Range::Earshot * Range::Earshot;
		const int Spellcast = Range::Spellcast * Range::Spellcast;
		const int Spirit = Range::Spirit * Range::Spirit;
		const int Compass = Range::Compass * Range::Compass;
	};

	namespace ItemID { // aka item modelIDs
		// item that make agents
		const int GhostInTheBox = 6368;
		const int GhastlyStone = 32557;
		const int LegionnaireStone = 37810;

		// materials
		const int Bones = 921;
		const int Iron = 948;
		const int Granite = 955;
		const int Dust = 929;
		const int Fibers = 934;
		const int Feathers = 933;

		// pcons
		const int BRC = 31151;
		const int GRC = 31152;
		const int RRC = 31153;
		const int Pies = 28436;
		const int Cupcakes = 22269;
		const int Apples = 28431;
		const int Corns = 28432;
		const int Eggs = 22752;
		const int Kabobs = 17060;
		const int Warsupplies = 35121;
		const int LunarRat = 29425;
		const int LunarOx = 29426;
		const int LunarTiger = 29427;
		const int LunarRabbit = 29428;
		const int LunarDragon = 29429;
		const int LunarSnake = 29430;
		const int LunarHorse = 29431;
		const int LunarSheep = 29432;
		const int LunarMonkey = 29433;
		const int ConsEssence = 24859;
		const int ConsArmor = 24860;
		const int ConsGrail = 24861;
		const int ResScrolls = 26501;
		const int SkalefinSoup = 17061;
		const int PahnaiSalad = 17062;
		const int Mobstopper = 32558;
		const int Powerstone = 24862;

		const int CremeBrulee = 15528;
		const int Fruitcake = 21492;
		const int SugaryBlueDrink = 21812;
		const int RedBeanCake = 15479;
		const int JarOfHoney = 31150;
		const int ChocolateBunny = 22644;

		// level-1 alcohol
		const int Eggnog = 6375;
		const int DwarvenAle = 5585;
		const int HuntersAle = 910;
		const int Absinthe = 6367;
		const int WitchsBrew = 6049;
		const int Ricewine = 15477;
		const int ShamrockAle = 22190;
		const int Cider = 28435;

		// level-5 alcohol
		const int Grog = 30855;
		const int SpikedEggnog = 6366;
		const int AgedDwarvenAle = 24593;
		const int AgedHungersAle = 31145;
		const int Keg = 31146;
		const int FlaskOfFirewater = 2513;
		const int KrytanBrandy = 35124;
	}

	namespace ModelID { // this is actually agent->PlayerNumber for agents
		const int SkeletonOfDhuum = 2338;
		const int Boo = 7445;
		const int MiniatureGuildLord = 347;
		const int MiniatureHighPriestZhang = 348;
		const int MiniatureGhostlyPriest = 349;
		const int MiniatureRiftWarden = 350;

		// ==== SoO ====
		const int SoO_brigand = 7005;
		const int SoO_fendi = 7009;
		const int SoO_fendi_soul = 7010;

		// ==== DoA ====
		const int BlackBeastOfArgh = 5146;

		// stygian lords
		const int StygianLordNecro = 5141;
		const int StygianLordMesmer = 5142;
		const int StygianLordEle = 5143;
		const int StygianLordMonk = 5144;
		const int StygianLordDerv = 5159;
		const int StygianLordRanger = 5160;

		// margonites
		const int MargoniteAnurKaya = 5162;
		const int MargoniteAnurDabi = 5163;
		const int MargoniteAnurSu = 5164;
		const int MargoniteAnurKi = 5165;
		const int MargoniteAnurVi = 5166;
		const int MargoniteAnurTuk = 5167;
		const int MargoniteAnurRuk = 5168;
		const int MargoniteAnurRand = 5169;
		const int MargoniteAnurMank = 5170;

		// stygians
		const int StygianHunger = 5171;
		const int StygianBrute = 5172;
		const int StygianGolem = 5173;
		const int StygianHorror = 5174;
		const int StygianFiend = 5175;

		// tormentors
		const int MindTormentor = 5176;
		const int SoulTormentor = 5177;
		const int WaterTormentor = 5178;
		const int HeartTormentor = 5179;
		const int FleshTormentor = 5180;
		const int SpiritTormentor = 5181;
		const int EarthTormentor = 5182;
		const int SanityTormentor = 5183;
	}

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

	namespace QuestID {
		namespace UW {
			const int Chamber = 101;
			const int Wastes = 102;
			const int UWG = 103;
			const int Mnt = 104;
			const int Pits = 105;
			const int Planes = 106;
			const int Pools = 107;
			const int Escort = 108;
			const int Restore = 109;
			const int Vale = 110;
		}

		namespace Fow {
			const int Defend = 202;
			const int ArmyOfDarknesses = 203;
			const int WailingLord = 204;
			const int Griffons = 205;
			const int Slaves = 206;
			const int Restore = 207;
			const int Hunt = 208;
			const int Forgemaster = 209;
			const int Tos = 211;
			const int Toc = 212;
			const int Khobay = 224;
		}

		namespace Doa {
			// gloom
			const int DeathbringerCompany = 749;
			const int RiftBetweenUs = 752;
			const int ToTheRescue = 753;

			// city
			const int City = 751;

			// Veil
			const int BreachingStygianVeil = 742;
			const int BroodWars = 755;

			// Foundry
			const int FoundryBreakout = 743;
			const int FoundryOfFailedCreations = 744;
		}

		namespace Camera {
			const float FIRST_PERSON_DIST = 2.f;
			const float DEFAULT_DIST = 750.f;
		}
	}
}

