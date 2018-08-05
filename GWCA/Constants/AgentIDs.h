#pragma once

namespace GW {
    namespace Constants {

        namespace ModelID { // this is actually agent->PlayerNumber for agents
            const int EoE = 2872;
            const int QZ = 2882;
            const int Winnowing = 2871;
            const int InfuriatingHeat = 5711;
            const int Equinox = 4232;
            const int Famine = 4234;
            const int FrozenSoil = 2878;
            const int Quicksand = 5714;
            const int Lacerate = 4228;

            const int Boo = 7445;

			const int LockedChest = 8141; // this is actually ->ExtraType

			/* Birthday Minis */
			/* unknowns are commented as a guess as inventory name but fit a pattern of id schema */
			namespace Minipet {
				// First Year
				const int Charr = 230;					// Miniature Charr Shaman (purple)
				const int Dragon = 231;					// Miniature Bone Dragon (green)
				const int Rurik = 232;					// Miniature Prince Rurik (gold)
				const int Shiro = 233;					// Miniature Shiro (gold)
				const int Titan = 234;					// Miniature Burning Titan (purple)
				const int Kirin = 235;					// Miniature Kirin (purple)
				const int NecridHorseman = 236;			// Miniature Necrid Horseman (white)
				const int JadeArmor = 237;				// Miniature Jade Armor (white)
				const int Hydra = 238;					// Miniature Hydra (white)
				const int FungalWallow = 239;			// Miniature Fungal Wallow (white)
				const int SiegeTurtle = 240;			// Miniature Siege Turtle (white)
				const int TempleGuardian = 241;			// Miniature Temple Guardian (white)
				const int JungleTroll = 242;			// Miniature Jungle Troll (white)
				const int WhiptailDevourer = 243;		// Miniature Whiptail Devourer (white)
				// Second Year
				const int Gwen = 244;                   // Miniature Gwen (green)
				const int GwenDoll = 245;               // Miniature Gwen Doll (??)
				const int WaterDjinn = 246;             // Miniature Water Djinn (gold)
				const int Lich = 247;                   // Miniature Lich (gold)
				const int Elf = 248;                    // Miniature Elf (purple)
				const int PalawaJoko = 249;             // Miniature Palawa Joko (purple)
				const int Koss = 250;                   // Miniature Koss (purple)
				const int MandragorImp = 251;           // Miniature Mandragor Imp (white)
				const int HeketWarrior = 252;			// Miniature Heket Warrior (white)
				const int HarpyRanger = 253;            // Miniature Harpy Ranger (white)
				const int Juggernaut = 254;             // Miniature Juggernaut (white)
				const int WindRider = 255;              // Miniature Wind Rider (white)
				const int FireImp = 256;                // Miniature Fire Imp (white)
				const int Aatxe = 257;                  // Miniature Aatxe (white)
				const int ThornWolf = 258;              // Miniature Thorn Wolf (white)
				// Third Year
				const int Abyssal = 259;                // Miniature Abyssal (white)
				const int BlackBeast = 260;             // Miniature Black Beast of Aaaaarrrrrrgghh (gold)
				const int Freezie = 261;                // Miniature Freezie (purple)
				const int Irukandji = 262;              // Miniature Irukandji (white)
				const int MadKingThorn = 263;           // Miniature Mad King Thorn (green)
				const int ForestMinotaur = 264;         // Miniature Forest Minotaur (white)
				const int Mursaat = 265;                // Miniature Mursaat (white)
				const int Nornbear = 266;               // Miniature Nornbear (purple)
				const int Ooze = 267;                   // Miniature Ooze (purple)
				const int Raptor = 268;                 // Miniature Raptor (white)
				const int RoaringEther = 269;           // Miniature Roaring Ether (white)
				const int CloudtouchedSimian = 270;     // Miniature Cloudtouched Simian (white)
				const int CaveSpider = 271;             // Miniature Cave Spider (white)
				const int WhiteRabbit = 272;			// White Rabbit (gold)
				// Fourth Year
				const int WordofMadness = 273;          // Miniature Word of Madness (white)
				const int DredgeBrute = 274;            // Miniature Dredge Brute (white)
				const int TerrorwebDryder = 275;        // Miniature Terrorweb Dryder (white)
				const int Abomination = 276;            // Miniature Abomination (white)
				const int KraitNeoss = 277;             // Miniature Krait Neoss (white)
				const int DesertGriffon = 278;          // Miniature Desert Griffon (white)
				const int Kveldulf = 279;               // Miniature Kveldulf (white)
				const int QuetzalSly = 280;             // Miniature Quetzal Sly (white)
				const int Jora = 281;                   // Miniature Jora (purple)
				const int FlowstoneElemental = 282;     // Miniature Flowstone Elemental (purple)
				const int Nian = 283;                   // Miniature Nian (purple)
				const int DagnarStonepate = 284;        // Miniature Dagnar Stonepate (gold)
				const int FlameDjinn = 285;             // Miniature Flame Djinn (gold)
				const int EyeOfJanthir = 286;           // Miniature Eye of Janthir (green)
				// Fifth Year
				const int Seer = 287;                   // Miniature Seer (white)
				const int SiegeDevourer = 288;          // Miniature Siege Devourer (white)
				const int ShardWolf = 289;              // Miniature Shard Wolf (white)
				const int FireDrake = 290;              // Miniature Fire Drake (white)
				const int SummitGiantHerder = 291;      // Miniature Summit Giant Herder (white)
				const int OphilNahualli = 292;          // Miniature Ophil Nahualli (white)
				const int CobaltScabara = 293;          // Miniature Cobalt Scabara (white)
				const int ScourgeManta = 294;           // Miniature Scourge Manta (white)
				const int Ventari = 295;                // Miniature Ventari (purple)
				const int Oola = 296;                   // Miniature Oola (purple)
				const int CandysmithMarley = 297;       // Miniature CandysmithMarley (purple)
				const int ZhuHanuku = 298;              // Miniature Zhu Hanuku (gold)
				const int KingAdelbern = 299;           // Miniature King Adelbern (gold)
				const int MOX1 = 300;                   // Miniature M.O.X. (color?)
				const int MOX2 = 301;                   // Miniature M.O.X. (color?)
				const int MOX3 = 302;                   // Miniature M.O.X. (green)
				const int MOX4 = 303;                   // Miniature M.O.X. (color?)
				const int MOX5 = 304;					// Miniature M.O.X. (color?)
				const int MOX6 = 305;					// Miniature M.O.X. (color?)

				// In-game rewards and promotionals
				const int BrownRabbit = 306;			// Miniature Brown Rabbit
				const int Yakkington = 307;				// Miniature Yakkington
				// const int Unknown308 = 308;
				const int CollectorsEditionKuunavang = 309; // Miniature Kuunavang (green)
				const int GrayGiant = 310;
				const int Asura = 311;
				const int DestroyerOfFlesh = 312;
				const int PolarBear = 313;
				const int CollectorsEditionVaresh = 314;
				const int Mallyx = 315;
				const int Ceratadon = 316;
				
				// Misc.
				const int Kanaxai = 317;
				const int Panda = 318;
				const int IslandGuardian = 319;
				const int NagaRaincaller = 320;
				const int LonghairYeti = 321;
				const int Oni = 322;
				const int ShirokenAssassin = 323;
				const int Vizu = 324;
				const int ZhedShadowhoof = 325;
				const int Grawl = 326;
				const int GhostlyHero = 327;

				// Special events
				const int Pig = 328;
				const int GreasedLightning = 329;
				const int WorldFamousRacingBeetle = 330;
				const int CelestialPig = 331;
				const int CelestialRat = 332;
				const int CelestialOx = 333;
				const int CelestialTiger = 334;
				const int CelestialRabbit = 335;
				const int CelestialDragon = 336;
				const int CelestialSnake = 337;
				const int CelestialHorse = 338;
				const int CelestialSheep = 339;
				const int CelestialMonkey = 340;
				const int CelestialRooster = 341;
				const int CelestialDog = 342;

				// In-game
				const int BlackMoaChick = 343;
				const int Dhuum = 344;
				const int MadKingsGuard = 345;
				const int SmiteCrawler = 346;

				// Zaishen strongboxes, and targetable minipets
				const int GuildLord = 347;
				const int HighPriestZhang = 348;
				const int GhostlyPriest = 349;
				const int RiftWarden = 350;

				// More in-game drops and rewards
				const int MiniatureLegionnaire = 7980;
				const int MiniatureConfessorDorian = 8289;
				const int MiniaturePrincessSalma = 8294;
				const int MiniatureLivia = 8295;
				const int MiniatureEvennia = 8296;
				const int MiniatureConfessorIsaiah = 8297;
				// missing miniature 8298 ?
				const int MiniaturePeacekeeperEnforcer = 8299;
				const int MiniatureMinisterReiko = 8983;
				const int MiniatureEcclesiateXunRao = 8984;
			}

            namespace DoA {
				// Friendly
				const int FoundrySnakes = 5217;

                const int BlackBeastOfArgh = 5146;
                const int SmotheringTendril = 5210;
                const int Fury = 5145;
                const int LordJadoth = 5140;

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
                const int MargoniteAnurVu = 5166;
                const int MargoniteAnurTuk = 5167;
                const int MargoniteAnurRuk = 5168;
                const int MargoniteAnurRund = 5169;
                const int MargoniteAnurMank = 5170;

                // stygians
                const int StygianHunger = 5171;
                const int StygianBrute = 5172;
                const int StygianGolem = 5173;
                const int StygianHorror = 5174;
                const int StygianFiend = 5175;

				// tormentors in veil
				const int VeilMindTormentor = 5176;
				const int VeilSoulTormentor = 5177;
				const int VeilWaterTormentor = 5178;
				const int VeilHeartTormentor = 5179;
				const int VeilFleshTormentor = 5180;
				const int VeilSpiritTormentor = 5181;
				const int VeilEarthTormentor = 5182;
				const int VeilSanityTormentor = 5183;
				// tormentors
                const int MindTormentor = 5200;
                const int SoulTormentor = 5201;
                const int WaterTormentor = 5202;
                const int HeartTormentor = 5203;
                const int FleshTormentor = 5204;
                const int SpiritTormentor = 5205;
                const int EarthTormentor = 5206;
                const int SanityTormentor = 5208;

                // titans
                const int MiseryTitan = 5191;
                const int RageTitan = 5192;
                const int DementiaTitan = 5193;
                const int AnguishTitan = 5194;
                const int DespairTitan = 5195;
                const int FuryTitan = 5196;
                const int RageTitan2 = 5197;
                const int DementiaTitan2 = 5198;

                const int TorturewebDryder = 5211;
                const int GreaterDreamRider = 5212;
            }

            namespace UW {
				const int ChainedSoul = 2313;
				const int DyingNightmare = 2314;
				const int ObsidianBehemoth = 2315;
				const int ObsidianGuardian = 2316;
				const int TerrorwebDryder = 2317;
				const int TerrorwebDryderSilver = 2318;
				const int KeeperOfSouls = 2319;
				const int TerrorwebQueen = 2320; // boss-like
				const int SmiteCrawler = 2321;
				const int WailingLord = 2322;		// Note: same as FoW::Banshee
				const int BanishedDreamRider = 2323;
				// 2324 ?
				const int FourHorseman = 2325; // all four share the same id
				const int MindbladeSpectre = 2326;
				
				const int DeadCollector = 2328;
				const int DeadThresher = 2329;
				const int ColdfireNight = 2330;
				const int StalkingNight = 2331;
				// 2332 ?
				const int ChargedBlackness = 2333;
				const int GraspingDarkness = 2334;
				const int BladedAatxe = 2335;
				// 2336 ?
                const int Slayer = 2337;
				const int SkeletonOfDhuum1 = 2338;
				const int SkeletonOfDhuum2 = 2339;
				const int ChampionOfDhuum = 2340;
				const int MinionOfDhuum = 2341;
				const int Dhuum = 2342;

				const int Reapers = 2344; // outside dhuum chamber
                const int ReapersAtDhuum = 2345; // in dhuum chamber
                const int IceElemental = 2346; // friendly, during waste quest near dhuum.
                const int KingFrozenwind = 2348;
                const int TorturedSpirit1 = 2349; // friendly, during quest
				const int Escort1 = 2352; // souls npc spawned by escort quest
				const int Escort2 = 2353;
				const int Escort3 = 2354;
				const int Escort4 = 2355;
				const int Escort5 = 2356;
				const int Escort6 = 2357;
				const int PitsSoul1 = 2358;
				const int PitsSoul2 = 2359;
				const int PitsSoul3 = 2360;
				const int PitsSoul4 = 2361;

				const int TorturedSpirit = 2367;
                const int MajorAlgheri = 2369;
            }

            namespace FoW {
				const int NimrosTheHunter = 1478;
				const int MikoTheUnchained = 1961;

				const int Banshee = 2322;			// Note: same as UW::WailingLord

				const int MahgoHydra = 2792;
				const int ArmoredCaveSpider = 2796;
				const int SmokeWalker = 2797;
				const int ObsidianFurnanceDrake = 2798;
				const int DoubtersDryder = 2799;
				const int ShadowMesmer = 2800;
				const int ShadowElemental = 2801;
				const int ShadowMonk = 2802;
				const int ShadowWarrior = 2803;
				const int ShadowRanger = 2804;
				const int ShadowBeast = 2805;
				const int Abyssal = 2806;			// Note: same as ShadowOverlord.
				const int ShadowOverlord = 2806;	// Note: same as Abyssal.
				const int SeedOfCorruption = 2807;
				const int SpiritWood = 2808;
				const int SpiritShepherd = 2809;
				const int AncientSkale = 2810;
				const int SnarlingDriftwood = 2811;
				const int SkeletalEtherBreaker = 2812;
				const int SkeletalIcehand = 2813;
				const int SkeletalBond = 2814;
				const int SkeletalBerserker = 2815;
				const int SkeletalImpaler = 2816;
				const int RockBorerWorm = 2817;
				const int InfernalWurm = 2818;
				const int DragonLich = 2819;
				const int Menzies = 2820;
				// 2821 ?
				const int Rastigan = 2822;			// Friendly NPC
				const int Griffons = 2823;			// Friendly NPC
				const int LordKhobay = 2824;		// Unfriendly NPC
				const int Forgemaster = 2825;		// Friendly NPC
				// 2826 ?
				const int TraitorousTempleGuard1 = 2827;
				const int TraitorousTempleGuard2 = 2828;
				const int TraitorousTempleGuard3 = 2829;
				// 2830 ?
				const int ShardWolf = 2831;
            }

			namespace Urgoz {
				const int HoppingVampire = 3741;
				const int Urgoz = 3750;
			}

			namespace Deep {
				const int Kanaxai = 4055;
				const int KanaxaiAspect = 4056;
			}

			namespace EotnDungeons {
				const int StormcloudIncubus = 6847;
				const int ZhimMonns = 6242;
				const int Khabuus = 6244;
				const int DuncanTheBlack = 6401;
				const int JusticiarThommis = 6402;
				const int RandStormweaver = 6403;
				const int Selvetarm = 6404;
				const int Forgewright = 6405;
				const int Frostmaw = 6928;
				
			}
			
			namespace SoO {
				const int Brigand = 7005;
				const int Fendi = 7009;
				const int Fendi_soul = 7010;
			}
        }
    }
}
