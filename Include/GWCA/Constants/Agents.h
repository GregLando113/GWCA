#pragma once
#include <GWCA/Constants/Types.h>

namespace GW {
    namespace Constants {
        enum class NpcID { // this is actually agent->PlayerNumber for agents
            None = 0,
            
            Rotscale = 2837,
            AbaddonsCursed = 1338,
            BladeAncientSyuShai,
            YoannhTheRebuilber,
            FureystSharpsight,
            EoE = 2876,
            QZ = 2886,
            Winnowing = 2875,
            InfuriatingHeat = 5715,
            Equinox = 4236,
            Famine = 4238,
            FrozenSoil = 2882,
            Quicksand = 5718,
            Lacerate = 4232,

            ProphetVaresh = 5292,
            CommanderVaresh = 5293,

            Boo = 7449,

            LockedChest = 8141, // this is actually ->ExtraType

            /* Birthday Minis */
            /* unknowns are commented as a guess as inventory name but fit a pattern of id schema */
            // First Year
            Minipet_Charr = 230,                  // Miniature Charr Shaman (purple)
            Minipet_Dragon = 231,                 // Miniature Bone Dragon (green)
            Minipet_Rurik = 232,                  // Miniature Prince Rurik (gold)
            Minipet_Shiro = 233,                  // Miniature Shiro (gold)
            Minipet_Titan = 234,                  // Miniature Burning Titan (purple)
            Minipet_Kirin = 235,                  // Miniature Kirin (purple)
            Minipet_NecridHorseman = 236,         // Miniature Necrid Horseman (white)
            Minipet_JadeArmor = 237,              // Miniature Jade Armor (white)
            Minipet_Hydra = 238,                  // Miniature Hydra (white)
            Minipet_FungalWallow = 239,           // Miniature Fungal Wallow (white)
            Minipet_SiegeTurtle = 240,            // Miniature Siege Turtle (white)
            Minipet_TempleGuardian = 241,         // Miniature Temple Guardian (white)
            Minipet_JungleTroll = 242,            // Miniature Jungle Troll (white)
            Minipet_WhiptailDevourer = 243,       // Miniature Whiptail Devourer (white)
            // Second Year
            Minipet_Gwen = 244,                   // Miniature Gwen (green)
            Minipet_GwenDoll = 245,               // Miniature Gwen Doll (??)
            Minipet_WaterDjinn = 246,             // Miniature Water Djinn (gold)
            Minipet_Lich = 247,                   // Miniature Lich (gold)
            Minipet_Elf = 248,                    // Miniature Elf (purple)
            Minipet_PalawaJoko = 249,             // Miniature Palawa Joko (purple)
            Minipet_Koss = 250,                   // Miniature Koss (purple)
            Minipet_MandragorImp = 251,           // Miniature Mandragor Imp (white)
            Minipet_HeketWarrior = 252,           // Miniature Heket Warrior (white)
            Minipet_HarpyRanger = 253,            // Miniature Harpy Ranger (white)
            Minipet_Juggernaut = 254,             // Miniature Juggernaut (white)
            Minipet_WindRider = 255,              // Miniature Wind Rider (white)
            Minipet_FireImp = 256,                // Miniature Fire Imp (white)
            Minipet_Aatxe = 257,                  // Miniature Aatxe (white)
            Minipet_ThornWolf = 258,              // Miniature Thorn Wolf (white)
            // Third Year
            Minipet_Abyssal = 259,                // Miniature Abyssal (white)
            Minipet_BlackBeast = 260,             // Miniature Black Beast of Aaaaarrrrrrgghh (gold)
            Minipet_Freezie = 261,                // Miniature Freezie (purple)
            Minipet_Irukandji = 262,              // Miniature Irukandji (white)
            Minipet_MadKingThorn = 263,           // Miniature Mad King Thorn (green)
            Minipet_ForestMinotaur = 264,         // Miniature Forest Minotaur (white)
            Minipet_Mursaat = 265,                // Miniature Mursaat (white)
            Minipet_Nornbear = 266,               // Miniature Nornbear (purple)
            Minipet_Ooze = 267,                   // Miniature Ooze (purple)
            Minipet_Raptor = 268,                 // Miniature Raptor (white)
            Minipet_RoaringEther = 269,           // Miniature Roaring Ether (white)
            Minipet_CloudtouchedSimian = 270,     // Miniature Cloudtouched Simian (white)
            Minipet_CaveSpider = 271,             // Miniature Cave Spider (white)
            Minipet_WhiteRabbit = 272,            // White Rabbit (gold)
            // Fourth Year
            Minipet_WordofMadness = 273,          // Miniature Word of Madness (white)
            Minipet_DredgeBrute = 274,            // Miniature Dredge Brute (white)
            Minipet_TerrorwebDryder = 275,        // Miniature Terrorweb Dryder (white)
            Minipet_Abomination = 276,            // Miniature Abomination (white)
            Minipet_KraitNeoss = 277,             // Miniature Krait Neoss (white)
            Minipet_DesertGriffon = 278,          // Miniature Desert Griffon (white)
            Minipet_Kveldulf = 279,               // Miniature Kveldulf (white)
            Minipet_QuetzalSly = 280,             // Miniature Quetzal Sly (white)
            Minipet_Jora = 281,                   // Miniature Jora (purple)
            Minipet_FlowstoneElemental = 282,     // Miniature Flowstone Elemental (purple)
            Minipet_Nian = 283,                   // Miniature Nian (purple)
            Minipet_DagnarStonepate = 284,        // Miniature Dagnar Stonepate (gold)
            Minipet_FlameDjinn = 285,             // Miniature Flame Djinn (gold)
            Minipet_EyeOfJanthir = 286,           // Miniature Eye of Janthir (green)
            // Fifth Year
            Minipet_Seer = 287,                   // Miniature Seer (white)
            Minipet_SiegeDevourer = 288,          // Miniature Siege Devourer (white)
            Minipet_ShardWolf = 289,              // Miniature Shard Wolf (white)
            Minipet_FireDrake = 290,              // Miniature Fire Drake (white)
            Minipet_SummitGiantHerder = 291,      // Miniature Summit Giant Herder (white)
            Minipet_OphilNahualli = 292,          // Miniature Ophil Nahualli (white)
            Minipet_CobaltScabara = 293,          // Miniature Cobalt Scabara (white)
            Minipet_ScourgeManta = 294,           // Miniature Scourge Manta (white)
            Minipet_Ventari = 295,                // Miniature Ventari (purple)
            Minipet_Oola = 296,                   // Miniature Oola (purple)
            Minipet_CandysmithMarley = 297,       // Miniature CandysmithMarley (purple)
            Minipet_ZhuHanuku = 298,              // Miniature Zhu Hanuku (gold)
            Minipet_KingAdelbern = 299,           // Miniature King Adelbern (gold)
            Minipet_MOX1 = 300,                   // Miniature M.O.X. (color?)
            Minipet_MOX2 = 301,                   // Miniature M.O.X. (color?)
            Minipet_MOX3 = 302,                   // Miniature M.O.X. (green)
            Minipet_MOX4 = 303,                   // Miniature M.O.X. (color?)
            Minipet_MOX5 = 304,                   // Miniature M.O.X. (color?)
            Minipet_MOX6 = 305,                   // Miniature M.O.X. (color?)

            // In-game rewards and promotionals
            Minipet_BrownRabbit = 306,            // Miniature Brown Rabbit
            Minipet_Yakkington = 307,             // Miniature Yakkington
            // Unknown308 = 308,
            Minipet_CollectorsEditionKuunavang = 309, // Miniature Kuunavang (green)
            Minipet_GrayGiant = 310,
            Minipet_Asura = 311,
            Minipet_DestroyerOfFlesh = 312,
            Minipet_PolarBear = 313,
            Minipet_CollectorsEditionVaresh = 314,
            Minipet_Mallyx = 315,
            Minipet_Ceratadon = 316,

            // Misc.
            Minipet_Kanaxai = 317,
            Minipet_Panda = 318,
            Minipet_IslandGuardian = 319,
            Minipet_NagaRaincaller = 320,
            Minipet_LonghairYeti = 321,
            Minipet_Oni = 322,
            Minipet_ShirokenAssassin = 323,
            Minipet_Vizu = 324,
            Minipet_ZhedShadowhoof = 325,
            Minipet_Grawl = 326,
            Minipet_GhostlyHero = 327,

            // Special events
            Minipet_Pig = 328,
            Minipet_GreasedLightning = 329,
            Minipet_WorldFamousRacingBeetle = 330,
            Minipet_CelestialPig = 331,
            Minipet_CelestialRat = 332,
            Minipet_CelestialOx = 333,
            Minipet_CelestialTiger = 334,
            Minipet_CelestialRabbit = 335,
            Minipet_CelestialDragon = 336,
            Minipet_CelestialSnake = 337,
            Minipet_CelestialHorse = 338,
            Minipet_CelestialSheep = 339,
            Minipet_CelestialMonkey = 340,
            Minipet_CelestialRooster = 341,
            Minipet_CelestialDog = 342,

            // In-game
            Minipet_BlackMoaChick = 343,
            Minipet_Dhuum = 344,
            Minipet_MadKingsGuard = 345,
            Minipet_SmiteCrawler = 346,

            // Zaishen strongboxes, and targetable minipets
            Minipet_GuildLord = 347,
            Minipet_HighPriestZhang = 348,
            Minipet_GhostlyPriest = 349,
            Minipet_RiftWarden = 350,

            // More in-game drops and rewards
            Minipet_MiniatureLegionnaire = 7984,
            Minipet_MiniatureConfessorDorian = 8293,
            Minipet_MiniaturePrincessSalma = 8298,
            Minipet_MiniatureLivia = 8299,
            Minipet_MiniatureEvennia = 8300,
            Minipet_MiniatureConfessorIsaiah = 8301,
            // missing miniature 8298 ?
            Minipet_MiniaturePeacekeeperEnforcer = 8303,
            Minipet_MiniatureMinisterReiko = 8987,
            Minipet_MiniatureEcclesiateXunRao = 8988,

            // DoA
            // Friendly
            FoundrySnakes = 5221,

            BlackBeastOfArgh = 5150,
            SmotheringTendril = 5214,
            Fury = 5149,
            LordJadoth = 5144,

            // stygian lords
            StygianLordNecro = 5145,
            StygianLordMesmer = 5146,
            StygianLordEle = 5147,
            StygianLordMonk = 5148,
            StygianLordDerv = 5163,
            StygianLordRanger = 5164,

            // margonites
            MargoniteAnurKaya = 5166,
            MargoniteAnurDabi = 5167,
            MargoniteAnurSu = 5168,
            MargoniteAnurKi = 5169,
            MargoniteAnurVu = 5170,
            MargoniteAnurTuk = 5171,
            MargoniteAnurRuk = 5172,
            MargoniteAnurRund = 5173,
            MargoniteAnurMank = 5174,

            // stygians
            StygianHunger = 5175,
            StygianBrute = 5176,
            StygianGolem = 5177,
            StygianHorror = 5178,
            StygianFiend = 5179,

            // tormentors in veil
            VeilMindTormentor = 5180,
            VeilSoulTormentor = 5181,
            VeilWaterTormentor = 5182,
            VeilHeartTormentor = 5183,
            VeilFleshTormentor = 5184,
            VeilSpiritTormentor = 5185,
            VeilEarthTormentor = 5186,
            VeilSanityTormentor = 5187,
            // tormentors
            MindTormentor = 5204,
            SoulTormentor = 5205,
            WaterTormentor = 5206,
            HeartTormentor = 5207,
            FleshTormentor = 5208,
            SpiritTormentor = 5209,
            EarthTormentor = 5210,
            SanityTormentor = 5212,

            // titans
            MiseryTitan = 5195,
            RageTitan = 5196,
            DementiaTitan = 5197,
            AnguishTitan = 5198,
            DespairTitan = 5199,
            FuryTitan = 5200,
            RageTitan2 = 5201,
            DementiaTitan2 = 5202,

            TorturewebDryder = 5215,
            GreaterDreamRider = 5216,

            // Underworld
            ChainedSoul = 2317,
            DyingNightmare = 2318,
            ObsidianBehemoth = 2319,
            ObsidianGuardian = 2320,
            TerrorwebDryder = 2321,
            TerrorwebDryderSilver = 2322,
            KeeperOfSouls = 2323,
            TerrorwebQueen = 2324, // boss-like
            SmiteCrawler = 2325,
            WailingLord = 2326,       // Note: same as FoW::Banshee
            BanishedDreamRider = 2327,
            // 2324 ?
            FourHorseman = 2329, // all four share the same id
            MindbladeSpectre = 2330,

            DeadCollector = 2332,
            DeadThresher = 2333,
            ColdfireNight = 2334,
            StalkingNight = 2335,
            // 2332 ?
            ChargedBlackness = 2337,
            GraspingDarkness = 2338,
            BladedAatxe = 2339,
            // 2336 ?
            Slayer = 2341,
            SkeletonOfDhuum1 = 2342,
            SkeletonOfDhuum2 = 2343,
            ChampionOfDhuum = 2344,
            MinionOfDhuum = 2345,
            Dhuum = 2346,

            Reapers = 2348, // outside dhuum chamber
            ReapersAtDhuum = 2349, // in dhuum chamber
            IceElemental = 2350, // friendly, during waste quest near dhuum.
            KingFrozenwind = 2352,
            TorturedSpirit1 = 2353, // friendly, during quest
            Escort1 = 2356, // souls npc spawned by escort quest
            Escort2 = 2357,
            Escort3 = 2358,
            Escort4 = 2359,
            Escort5 = 2360,
            Escort6 = 2361,
            PitsSoul1 = 2362,
            PitsSoul2 = 2363,
            PitsSoul3 = 2364,
            PitsSoul4 = 2365,

            TorturedSpirit = 2371,
            MajorAlgheri = 2373,

            // Fissure of Woe
            NimrosTheHunter = 1482,
            MikoTheUnchained = 1965,

            Banshee = 2326,           // Note: same as UW::WailingLord

            MahgoHydra = 2796,
            ArmoredCaveSpider = 2800,
            SmokeWalker = 2801,
            ObsidianFurnanceDrake = 2802,
            DoubtersDryder = 2803,
            ShadowMesmer = 2804,
            ShadowElemental = 2805,
            ShadowMonk = 2806,
            ShadowWarrior = 2807,
            ShadowRanger = 2808,
            ShadowBeast = 2809,
            Abyssal = 2810,           // Note: same as ShadowOverlord.
            ShadowOverlord = 2810,    // Note: same as Abyssal.
            SeedOfCorruption = 2811,
            SpiritWood = 2812,
            SpiritShepherd = 2813,
            AncientSkale = 2814,
            SnarlingDriftwood = 2815,
            SkeletalEtherBreaker = 2816,
            SkeletalIcehand = 2817,
            SkeletalBond = 2818,
            SkeletalBerserker = 2819,
            SkeletalImpaler = 2820,
            RockBorerWorm = 2821,
            InfernalWurm = 2822,
            DragonLich = 2823,
            Menzies = 2824,
            // 2821 ?
            Rastigan = 2826,          // Friendly NPC
            Griffons = 2827,          // Friendly NPC
            LordKhobay = 2828,        // Unfriendly NPC
            Forgemaster = 2829,       // Friendly NPC
            // 2826 ?
            TraitorousTempleGuard1 = 2831,
            TraitorousTempleGuard2 = 2832,
            TraitorousTempleGuard3 = 2833,
            // 2830 ?
            ShardWolf = 2835,

            // Urgoz' Warren
            HoppingVampire = 3745,
            Urgoz = 3754,

            // The Deep
            Kanaxai = 4059,
            KanaxaiAspect = 4060,

            // Eotn Dungeons
            DiscOfChaos = 6071,
            PlagueOfDestruction = 6083,
            ZhimMonns = 6246,
            Khabuus = 6248,
            DuncanTheBlack = 6405,
            JusticiarThommis = 6406,
            RandStormweaver = 6407,
            Selvetarm = 6408,
            Forgewright = 6409,
            HavokSoulwail = 6427,
            RragarManeater3 = 6558, // lvl 3
            RragarManeater12 = 6559, // lvl 1 and 2
            Arachni = 6793,
            Hidesplitter = 6801,
            PrismaticOoze = 6806,
            IlsundurLordofFire = 6814,
            EldritchEttin = 6821,
            TPSRegulartorGolem = 6832,
            MalfunctioningEnduringGolem = 6834,
            StormcloudIncubus = 6851,
            CyndrTheMountainHeart = 6914,
            InfernalSiegeWurm = 6927, // kath lvl1 boss
            Frostmaw = 6932,
            RemnantOfAntiquities = 6934,
            MurakaiLadyOfTheNight = 7008,
            ZoldarkTheUnholy = 7010,

            Brigand = 7009, // soo
            FendiNin = 7013,
            SoulOfFendiNin = 7014,

            KeymasterOfMurakai = 7018,
            AngrySnowman = 7411,

            // BonusMissionPack
            WarAshenskull = 7079,
            RoxAshreign = 7080,
            AnrakTindershot = 7081,
            DettMortash = 7082,
            AkinCinderspire = 7083,
            TwangSootpaws = 7084,
            MagisEmberglow = 7085,
            MerciaTheSmug = 7114,
            OptimusCaliph = 7115,
            LazarusTheDire = 7116,
            AdmiralJakman = 7179,
            PalawaJoko = 7198,
            YuriTheHand = 7146,
            MasterRiyo = 7147,
            CaptainSunpu = 7149,
            MinisterWona = 7150,

            // Polymock Summon
            MursaatElementalist = 5847,
            FlameDjinn,
            IceImp,
            NagaShaman,
            UnknownSummon,
            EbonVanguardAssassin
        };
        ENUM_CLASS_OPERATORS(NpcID, 0);
    }
}
