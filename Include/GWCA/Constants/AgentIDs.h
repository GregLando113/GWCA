#pragma once

namespace GW {
    namespace Constants {

        namespace ModelID { // this is actually agent->PlayerNumber for agents
            constexpr int Rotscale = 2833;

            constexpr int EoE = 2872;
            constexpr int QZ = 2882;
            constexpr int Winnowing = 2871;
            constexpr int InfuriatingHeat = 5711;
            constexpr int Equinox = 4232;
            constexpr int Famine = 4234;
            constexpr int FrozenSoil = 2878;
            constexpr int Quicksand = 5714;
            constexpr int Lacerate = 4228;

            constexpr int ProphetVaresh = 5288;
            constexpr int CommanderVaresh = 5289;

            constexpr int Boo = 7445;

            constexpr int LockedChest = 8141; // this is actually ->ExtraType

            /* Birthday Minis */
            /* unknowns are commented as a guess as inventory name but fit a pattern of id schema */
            namespace Minipet {
                // First Year
                constexpr int Charr = 230;                  // Miniature Charr Shaman (purple)
                constexpr int Dragon = 231;                 // Miniature Bone Dragon (green)
                constexpr int Rurik = 232;                  // Miniature Prince Rurik (gold)
                constexpr int Shiro = 233;                  // Miniature Shiro (gold)
                constexpr int Titan = 234;                  // Miniature Burning Titan (purple)
                constexpr int Kirin = 235;                  // Miniature Kirin (purple)
                constexpr int NecridHorseman = 236;         // Miniature Necrid Horseman (white)
                constexpr int JadeArmor = 237;              // Miniature Jade Armor (white)
                constexpr int Hydra = 238;                  // Miniature Hydra (white)
                constexpr int FungalWallow = 239;           // Miniature Fungal Wallow (white)
                constexpr int SiegeTurtle = 240;            // Miniature Siege Turtle (white)
                constexpr int TempleGuardian = 241;         // Miniature Temple Guardian (white)
                constexpr int JungleTroll = 242;            // Miniature Jungle Troll (white)
                constexpr int WhiptailDevourer = 243;       // Miniature Whiptail Devourer (white)
                // Second Year
                constexpr int Gwen = 244;                   // Miniature Gwen (green)
                constexpr int GwenDoll = 245;               // Miniature Gwen Doll (??)
                constexpr int WaterDjinn = 246;             // Miniature Water Djinn (gold)
                constexpr int Lich = 247;                   // Miniature Lich (gold)
                constexpr int Elf = 248;                    // Miniature Elf (purple)
                constexpr int PalawaJoko = 249;             // Miniature Palawa Joko (purple)
                constexpr int Koss = 250;                   // Miniature Koss (purple)
                constexpr int MandragorImp = 251;           // Miniature Mandragor Imp (white)
                constexpr int HeketWarrior = 252;           // Miniature Heket Warrior (white)
                constexpr int HarpyRanger = 253;            // Miniature Harpy Ranger (white)
                constexpr int Juggernaut = 254;             // Miniature Juggernaut (white)
                constexpr int WindRider = 255;              // Miniature Wind Rider (white)
                constexpr int FireImp = 256;                // Miniature Fire Imp (white)
                constexpr int Aatxe = 257;                  // Miniature Aatxe (white)
                constexpr int ThornWolf = 258;              // Miniature Thorn Wolf (white)
                // Third Year
                constexpr int Abyssal = 259;                // Miniature Abyssal (white)
                constexpr int BlackBeast = 260;             // Miniature Black Beast of Aaaaarrrrrrgghh (gold)
                constexpr int Freezie = 261;                // Miniature Freezie (purple)
                constexpr int Irukandji = 262;              // Miniature Irukandji (white)
                constexpr int MadKingThorn = 263;           // Miniature Mad King Thorn (green)
                constexpr int ForestMinotaur = 264;         // Miniature Forest Minotaur (white)
                constexpr int Mursaat = 265;                // Miniature Mursaat (white)
                constexpr int Nornbear = 266;               // Miniature Nornbear (purple)
                constexpr int Ooze = 267;                   // Miniature Ooze (purple)
                constexpr int Raptor = 268;                 // Miniature Raptor (white)
                constexpr int RoaringEther = 269;           // Miniature Roaring Ether (white)
                constexpr int CloudtouchedSimian = 270;     // Miniature Cloudtouched Simian (white)
                constexpr int CaveSpider = 271;             // Miniature Cave Spider (white)
                constexpr int WhiteRabbit = 272;            // White Rabbit (gold)
                // Fourth Year
                constexpr int WordofMadness = 273;          // Miniature Word of Madness (white)
                constexpr int DredgeBrute = 274;            // Miniature Dredge Brute (white)
                constexpr int TerrorwebDryder = 275;        // Miniature Terrorweb Dryder (white)
                constexpr int Abomination = 276;            // Miniature Abomination (white)
                constexpr int KraitNeoss = 277;             // Miniature Krait Neoss (white)
                constexpr int DesertGriffon = 278;          // Miniature Desert Griffon (white)
                constexpr int Kveldulf = 279;               // Miniature Kveldulf (white)
                constexpr int QuetzalSly = 280;             // Miniature Quetzal Sly (white)
                constexpr int Jora = 281;                   // Miniature Jora (purple)
                constexpr int FlowstoneElemental = 282;     // Miniature Flowstone Elemental (purple)
                constexpr int Nian = 283;                   // Miniature Nian (purple)
                constexpr int DagnarStonepate = 284;        // Miniature Dagnar Stonepate (gold)
                constexpr int FlameDjinn = 285;             // Miniature Flame Djinn (gold)
                constexpr int EyeOfJanthir = 286;           // Miniature Eye of Janthir (green)
                // Fifth Year
                constexpr int Seer = 287;                   // Miniature Seer (white)
                constexpr int SiegeDevourer = 288;          // Miniature Siege Devourer (white)
                constexpr int ShardWolf = 289;              // Miniature Shard Wolf (white)
                constexpr int FireDrake = 290;              // Miniature Fire Drake (white)
                constexpr int SummitGiantHerder = 291;      // Miniature Summit Giant Herder (white)
                constexpr int OphilNahualli = 292;          // Miniature Ophil Nahualli (white)
                constexpr int CobaltScabara = 293;          // Miniature Cobalt Scabara (white)
                constexpr int ScourgeManta = 294;           // Miniature Scourge Manta (white)
                constexpr int Ventari = 295;                // Miniature Ventari (purple)
                constexpr int Oola = 296;                   // Miniature Oola (purple)
                constexpr int CandysmithMarley = 297;       // Miniature CandysmithMarley (purple)
                constexpr int ZhuHanuku = 298;              // Miniature Zhu Hanuku (gold)
                constexpr int KingAdelbern = 299;           // Miniature King Adelbern (gold)
                constexpr int MOX1 = 300;                   // Miniature M.O.X. (color?)
                constexpr int MOX2 = 301;                   // Miniature M.O.X. (color?)
                constexpr int MOX3 = 302;                   // Miniature M.O.X. (green)
                constexpr int MOX4 = 303;                   // Miniature M.O.X. (color?)
                constexpr int MOX5 = 304;                   // Miniature M.O.X. (color?)
                constexpr int MOX6 = 305;                   // Miniature M.O.X. (color?)

                // In-game rewards and promotionals
                constexpr int BrownRabbit = 306;            // Miniature Brown Rabbit
                constexpr int Yakkington = 307;             // Miniature Yakkington
                // constexpr int Unknown308 = 308;
                constexpr int CollectorsEditionKuunavang = 309; // Miniature Kuunavang (green)
                constexpr int GrayGiant = 310;
                constexpr int Asura = 311;
                constexpr int DestroyerOfFlesh = 312;
                constexpr int PolarBear = 313;
                constexpr int CollectorsEditionVaresh = 314;
                constexpr int Mallyx = 315;
                constexpr int Ceratadon = 316;
                
                // Misc.
                constexpr int Kanaxai = 317;
                constexpr int Panda = 318;
                constexpr int IslandGuardian = 319;
                constexpr int NagaRaincaller = 320;
                constexpr int LonghairYeti = 321;
                constexpr int Oni = 322;
                constexpr int ShirokenAssassin = 323;
                constexpr int Vizu = 324;
                constexpr int ZhedShadowhoof = 325;
                constexpr int Grawl = 326;
                constexpr int GhostlyHero = 327;

                // Special events
                constexpr int Pig = 328;
                constexpr int GreasedLightning = 329;
                constexpr int WorldFamousRacingBeetle = 330;
                constexpr int CelestialPig = 331;
                constexpr int CelestialRat = 332;
                constexpr int CelestialOx = 333;
                constexpr int CelestialTiger = 334;
                constexpr int CelestialRabbit = 335;
                constexpr int CelestialDragon = 336;
                constexpr int CelestialSnake = 337;
                constexpr int CelestialHorse = 338;
                constexpr int CelestialSheep = 339;
                constexpr int CelestialMonkey = 340;
                constexpr int CelestialRooster = 341;
                constexpr int CelestialDog = 342;

                // In-game
                constexpr int BlackMoaChick = 343;
                constexpr int Dhuum = 344;
                constexpr int MadKingsGuard = 345;
                constexpr int SmiteCrawler = 346;

                // Zaishen strongboxes, and targetable minipets
                constexpr int GuildLord = 347;
                constexpr int HighPriestZhang = 348;
                constexpr int GhostlyPriest = 349;
                constexpr int RiftWarden = 350;

                // More in-game drops and rewards
                constexpr int MiniatureLegionnaire = 7980;
                constexpr int MiniatureConfessorDorian = 8289;
                constexpr int MiniaturePrincessSalma = 8294;
                constexpr int MiniatureLivia = 8295;
                constexpr int MiniatureEvennia = 8296;
                constexpr int MiniatureConfessorIsaiah = 8297;
                // missing miniature 8298 ?
                constexpr int MiniaturePeacekeeperEnforcer = 8299;
                constexpr int MiniatureMinisterReiko = 8983;
                constexpr int MiniatureEcclesiateXunRao = 8984;
            }

            namespace DoA {
                // Friendly
                constexpr int FoundrySnakes = 5217;

                constexpr int BlackBeastOfArgh = 5146;
                constexpr int SmotheringTendril = 5210;
                constexpr int Fury = 5145;
                constexpr int LordJadoth = 5140;

                // stygian lords
                constexpr int StygianLordNecro = 5141;
                constexpr int StygianLordMesmer = 5142;
                constexpr int StygianLordEle = 5143;
                constexpr int StygianLordMonk = 5144;
                constexpr int StygianLordDerv = 5159;
                constexpr int StygianLordRanger = 5160;

                // margonites
                constexpr int MargoniteAnurKaya = 5162;
                constexpr int MargoniteAnurDabi = 5163;
                constexpr int MargoniteAnurSu = 5164;
                constexpr int MargoniteAnurKi = 5165;
                constexpr int MargoniteAnurVu = 5166;
                constexpr int MargoniteAnurTuk = 5167;
                constexpr int MargoniteAnurRuk = 5168;
                constexpr int MargoniteAnurRund = 5169;
                constexpr int MargoniteAnurMank = 5170;

                // stygians
                constexpr int StygianHunger = 5171;
                constexpr int StygianBrute = 5172;
                constexpr int StygianGolem = 5173;
                constexpr int StygianHorror = 5174;
                constexpr int StygianFiend = 5175;

                // tormentors in veil
                constexpr int VeilMindTormentor = 5176;
                constexpr int VeilSoulTormentor = 5177;
                constexpr int VeilWaterTormentor = 5178;
                constexpr int VeilHeartTormentor = 5179;
                constexpr int VeilFleshTormentor = 5180;
                constexpr int VeilSpiritTormentor = 5181;
                constexpr int VeilEarthTormentor = 5182;
                constexpr int VeilSanityTormentor = 5183;
                // tormentors
                constexpr int MindTormentor = 5200;
                constexpr int SoulTormentor = 5201;
                constexpr int WaterTormentor = 5202;
                constexpr int HeartTormentor = 5203;
                constexpr int FleshTormentor = 5204;
                constexpr int SpiritTormentor = 5205;
                constexpr int EarthTormentor = 5206;
                constexpr int SanityTormentor = 5208;

                // titans
                constexpr int MiseryTitan = 5191;
                constexpr int RageTitan = 5192;
                constexpr int DementiaTitan = 5193;
                constexpr int AnguishTitan = 5194;
                constexpr int DespairTitan = 5195;
                constexpr int FuryTitan = 5196;
                constexpr int RageTitan2 = 5197;
                constexpr int DementiaTitan2 = 5198;

                constexpr int TorturewebDryder = 5211;
                constexpr int GreaterDreamRider = 5212;
            }

            namespace UW {
                constexpr int ChainedSoul = 2313;
                constexpr int DyingNightmare = 2314;
                constexpr int ObsidianBehemoth = 2315;
                constexpr int ObsidianGuardian = 2316;
                constexpr int TerrorwebDryder = 2317;
                constexpr int TerrorwebDryderSilver = 2318;
                constexpr int KeeperOfSouls = 2319;
                constexpr int TerrorwebQueen = 2320; // boss-like
                constexpr int SmiteCrawler = 2321;
                constexpr int WailingLord = 2322;       // Note: same as FoW::Banshee
                constexpr int BanishedDreamRider = 2323;
                // 2324 ?
                constexpr int FourHorseman = 2325; // all four share the same id
                constexpr int MindbladeSpectre = 2326;
                
                constexpr int DeadCollector = 2328;
                constexpr int DeadThresher = 2329;
                constexpr int ColdfireNight = 2330;
                constexpr int StalkingNight = 2331;
                // 2332 ?
                constexpr int ChargedBlackness = 2333;
                constexpr int GraspingDarkness = 2334;
                constexpr int BladedAatxe = 2335;
                // 2336 ?
                constexpr int Slayer = 2337;
                constexpr int SkeletonOfDhuum1 = 2338;
                constexpr int SkeletonOfDhuum2 = 2339;
                constexpr int ChampionOfDhuum = 2340;
                constexpr int MinionOfDhuum = 2341;
                constexpr int Dhuum = 2342;

                constexpr int Reapers = 2344; // outside dhuum chamber
                constexpr int ReapersAtDhuum = 2345; // in dhuum chamber
                constexpr int IceElemental = 2346; // friendly, during waste quest near dhuum.
                constexpr int KingFrozenwind = 2348;
                constexpr int TorturedSpirit1 = 2349; // friendly, during quest
                constexpr int Escort1 = 2352; // souls npc spawned by escort quest
                constexpr int Escort2 = 2353;
                constexpr int Escort3 = 2354;
                constexpr int Escort4 = 2355;
                constexpr int Escort5 = 2356;
                constexpr int Escort6 = 2357;
                constexpr int PitsSoul1 = 2358;
                constexpr int PitsSoul2 = 2359;
                constexpr int PitsSoul3 = 2360;
                constexpr int PitsSoul4 = 2361;

                constexpr int TorturedSpirit = 2367;
                constexpr int MajorAlgheri = 2369;
            }

            namespace FoW {
                constexpr int NimrosTheHunter = 1478;
                constexpr int MikoTheUnchained = 1961;

                constexpr int Banshee = 2322;           // Note: same as UW::WailingLord

                constexpr int MahgoHydra = 2792;
                constexpr int ArmoredCaveSpider = 2796;
                constexpr int SmokeWalker = 2797;
                constexpr int ObsidianFurnanceDrake = 2798;
                constexpr int DoubtersDryder = 2799;
                constexpr int ShadowMesmer = 2800;
                constexpr int ShadowElemental = 2801;
                constexpr int ShadowMonk = 2802;
                constexpr int ShadowWarrior = 2803;
                constexpr int ShadowRanger = 2804;
                constexpr int ShadowBeast = 2805;
                constexpr int Abyssal = 2806;           // Note: same as ShadowOverlord.
                constexpr int ShadowOverlord = 2806;    // Note: same as Abyssal.
                constexpr int SeedOfCorruption = 2807;
                constexpr int SpiritWood = 2808;
                constexpr int SpiritShepherd = 2809;
                constexpr int AncientSkale = 2810;
                constexpr int SnarlingDriftwood = 2811;
                constexpr int SkeletalEtherBreaker = 2812;
                constexpr int SkeletalIcehand = 2813;
                constexpr int SkeletalBond = 2814;
                constexpr int SkeletalBerserker = 2815;
                constexpr int SkeletalImpaler = 2816;
                constexpr int RockBorerWorm = 2817;
                constexpr int InfernalWurm = 2818;
                constexpr int DragonLich = 2819;
                constexpr int Menzies = 2820;
                // 2821 ?
                constexpr int Rastigan = 2822;          // Friendly NPC
                constexpr int Griffons = 2823;          // Friendly NPC
                constexpr int LordKhobay = 2824;        // Unfriendly NPC
                constexpr int Forgemaster = 2825;       // Friendly NPC
                // 2826 ?
                constexpr int TraitorousTempleGuard1 = 2827;
                constexpr int TraitorousTempleGuard2 = 2828;
                constexpr int TraitorousTempleGuard3 = 2829;
                // 2830 ?
                constexpr int ShardWolf = 2831;
            }

            namespace Urgoz {
                constexpr int HoppingVampire = 3741;
                constexpr int Urgoz = 3750;
            }

            namespace Deep {
                constexpr int Kanaxai = 4055;
                constexpr int KanaxaiAspect = 4056;
            }

            namespace EotnDungeons {
                constexpr int DiscOfChaos = 6067;
                constexpr int PlagueOfDestruction = 6079;
                constexpr int ZhimMonns = 6242;
                constexpr int Khabuus = 6244;
                constexpr int DuncanTheBlack = 6401;
                constexpr int JusticiarThommis = 6402;
                constexpr int RandStormweaver = 6403;
                constexpr int Selvetarm = 6404;
                constexpr int Forgewright = 6405;
                constexpr int HavokSoulwail = 6423;
                constexpr int RragarManeater3 = 6554; // lvl 3
                constexpr int RragarManeater12 = 6555; // lvl 1 and 2
                constexpr int Arachni = 6789;
                constexpr int Hidesplitter = 6797;
                constexpr int PrismaticOoze = 6802;
                constexpr int IlsundurLordofFire = 6810;
                constexpr int EldritchEttin = 6817;
                constexpr int TPSRegulartorGolem = 6828;
                constexpr int MalfunctioningEnduringGolem = 6830;
                constexpr int StormcloudIncubus = 6847;
                constexpr int CyndrTheMountainHeart = 6910;
                constexpr int InfernalSiegeWurm = 6923; // kath lvl1 boss
                constexpr int Frostmaw = 6928;
                constexpr int RemnantOfAntiquities = 6930;
                constexpr int MurakaiLadyOfTheNight = 7004;
                constexpr int ZoldarkTheUnholy = 7006;

                constexpr int Brigand = 7005; // soo
                constexpr int FendiNin = 7009;
                constexpr int SoulOfFendiNin = 7010;

                constexpr int KeymasterOfMurakai = 7014;
                constexpr int AngrySnowman = 7407;
            }

            namespace BonusMissionPack {
                constexpr int WarAshenskull = 7075;
                constexpr int RoxAshreign = 7076;
                constexpr int AnrakTindershot = 7077;
                constexpr int DettMortash = 7078;
                constexpr int AkinCinderspire = 7079;
                constexpr int TwangSootpaws = 7080;
                constexpr int MagisEmberglow = 7081;
                constexpr int MerciaTheSmug = 7110;
                constexpr int OptimusCaliph = 7111;
                constexpr int LazarusTheDire = 7112;
                constexpr int AdmiralJakman = 7175;
                constexpr int PalawaJoko = 7194;
                constexpr int YuriTheHand = 7142;
                constexpr int MasterRiyo = 7143;
                constexpr int CaptainSunpu = 7145;
                constexpr int MinisterWona = 7146;
            }
        }
    }
}
