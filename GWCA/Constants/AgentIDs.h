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

            /* Birthday Minis */
            /* int name is what target shows as, comment is the inventory name. */
            /* unknowns are commented as a guess as inventory name but fit a pattern of id schema */
            // First Year
            const int BirthdayCharrDoll = 230;              // Miniature Charr Shaman (purple)
            const int BirthdayDragonDoll = 231;             // Miniature Bone Dragon (green)
            const int BirthdayRurikDoll = 232;              // Miniature Prince Rurik (gold)
            // const int Birthday233 = 233;                    // Miniature Shiro (gold) ??
            const int BirthdayTitanDoll = 234;              // Miniature Burning Titan (purple)
            const int BirthdayKirinDoll = 235;              // Miniature Kirin (purple)
            const int BirthdayNecridDoll = 236;             // Miniature Necrid Horseman (white)
            const int BirthdayJadeArmorDoll = 237;          // Miniature Jade Armor (white)
            const int BirthdayHydraDoll = 238;              // Miniature Hydra (white)
            const int BirthdayWallowDoll = 239;             // Miniature Fungal Wallow (white)
            const int BirthdayTurtleDoll = 240;             // Miniature Siege Turtle (white)
            const int BirthdayGuardianDoll = 241;           // Miniature Temple Guardian (white)
            const int BirthdayTrollDoll = 242;              // Miniature Jungle Troll (white)
            const int BirthdayDevourerDoll = 243;           // Miniature Whiptail Devourer (white)
            // Second Year
            const int MiniatureGwen = 244;                  // Miniature Gwen (green)
            // const int Miniature245 = 245;                   // ?? Wiki missing an entry ??
            const int MiniatureWaterDjinn = 246;            // Miniature Water Djinn (gold)
            const int MiniatureLich = 247;                  // Miniature Lich (gold)
            const int MiniatureElf = 248;                   // Miniature Elf (purple)
            const int MiniaturePalawaJoko = 249;            // Miniature Palawa Joko (purple)
            const int MiniatureKoss = 250;                  // Miniature Koss (purple)
            const int MiniatureMandragorImp = 251;          // Miniature Mandragor Imp (white)
            const int MiniatureHeket = 252;                 // Miniature Heket Warrior (white)
            const int MiniatureHarpy = 253;                 // Miniature Harpy Ranger (white)
            const int MiniatureJuggernaut = 254;            // Miniature Juggernaut (white)
            const int MiniatureWindRider = 255;             // Miniature Wind Rider (white)
            const int MiniatureFireImp = 256;               // Miniature Fire Imp (white)
            const int MiniatureAatxe = 257;                 // Miniature Aatxe (white)
            const int MiniatureThornWolf = 258;             // Miniature Thorn Wolf (white)
            // Third Year
            const int MiniatureAbyssal = 259;               // Miniature Abyssal (white)
            const int MiniatureBlackBeast = 260;            // Miniature Black Beast of Aaaaarrrrrrgghh (gold)
            const int MiniatureFreezie = 261;               // Miniature Freezie (purple)
            const int MiniatureIrukandji = 262;             // Miniature Irukandji (white)
            const int MiniatureMadKingThorn = 263;          // Miniature Mad King Thorn (green)
            const int MiniatureForestMinotaur = 264;        // Miniature Forest Minotaur (white)
            const int MiniatureMursaat = 265;               // Miniature Mursaat (white)
            const int MiniatureSvanir = 266;                // Miniature Nornbear (purple)
            const int MiniatureOoze = 267;                  // Miniature Ooze (purple)
            const int MiniatureRaptor = 268;                // Miniature Raptor (white)
            const int MiniatureRoaringEther = 269;          // Miniature Roaring Ether (white)
            const int MiniatureCloudtouchedSimian = 270;    // Miniature Cloudtouched Simian (white)
            const int MiniatureLavaSpider = 271;            // Miniature Cave Spider (white)
            const int WhiteRabbit = 272;                    // White Rabbit (gold)
            // Fourth Year
            const int MiniatureWordofMadness = 273;         // Miniature Word of Madness (white)
            const int MiniatureDredgeBrute = 274;           // Miniature Dredge Brute (white)
            const int MiniatureTerrorwebDryder = 275;       // Miniature Terrorweb Dryder (white)
            const int MiniatureAbomination = 276;           // Miniature Abomination (white)
            const int MiniatureKraitNeoss = 277;            // Miniature Krait Neoss (white)
            //const int Miniature278 = 278;                 // Miniature Desert Griffon (white) ??
            const int MiniatureKveldulf = 279;              // Miniature Kveldulf (white)
            const int MiniatureQuetzalSly = 280;            // Miniature Quetzal Sly (white)
            const int MiniatureJora = 281;                  // Miniature Jora (purple)
            const int MiniatureFlowstoneElemental = 282;    // Miniature Flowstone Elemental (purple)
            const int MiniatureNian = 283;                  // Miniature Nian (purple)
            const int MiniatureDagnarStonepate = 284;       // Miniature Dagnar Stonepate (gold)
            const int MiniatureFlameDjinn = 285;            // Miniature Flame Djinn (gold)
            // const int Miniature286 = 286;                   // Miniature Eye of Janthir (green) ??
            // Fifth Year
            const int MiniatureSeer = 287;                  // Miniature Seer (white)
            const int MiniatureSiegeDevourer = 288;         // Miniature Siege Devourer (white)
            const int MiniatureShardWolf = 289;             // Miniature Shard Wolf (white)
            const int MiniatureFireDrake = 290;             // Miniature Fire Drake (white)
            const int MiniatureSummitGiantHerder = 291;     // Miniature Summit Giant Herder (white)
            const int MiniatureOphilNahualli = 292;         // Miniature Ophil Nahualli (white)
            const int MiniatureCobaltScabara = 293;         // Miniature Cobalt Scabara (white)
            const int MiniatureScourgeManta = 294;          // Miniature Scourge Manta (white)
            const int MiniatureVentari = 295;               // Miniature Ventari (purple)
            const int MiniatureOola = 296;                  // Miniature Oola (purple)
            const int MiniatureCandysmithMarley = 297;      // Miniature CandysmithMarley (purple)
            const int MiniatureZhuHanuku = 298;             // Miniature Zhu Hanuku (gold)
            const int MiniatureKingAdelbern = 299;          // Miniature King Adelbern (gold)
            // const int Miniature300 = 300;                   // ?? Wiki missing an entry ??
            // const int Miniature301 = 301;                   // ?? Wiki missing an entry ??
            const int MiniatureMOX = 302;                   // Miniature M.O.X. (green)

            // const int Unknown303 = 303;
            // const int Unknown304 = 304;
            // const int Unknown305 = 305;
            // const int Unknown306 = 306;
            // const int Unknown307 = 307;
            // const int Unknown308 = 308;
            const int CollectorsEditionKuunavangDoll = 309; // Miniature Kuunavang (green)
            // const int Unknown310 = 310;
            // const int Unknown311 = 311;
            // const int Unknown312 = 312;
            // const int Unknown313 = 313;
            const int CollectorsEditionVareshDoll = 314;    // Miniature Varesh (green)

            const int MiniatureGuildLord = 347;
            const int MiniatureHighPriestZhang = 348;
            const int MiniatureGhostlyPriest = 349;
            const int MiniatureRiftWarden = 350;

            const int LockedChest = 8141; // this is actually ->ExtraType

            // ==== SoO ====
            namespace SoO {
                const int Brigand = 7005;
                const int Fendi = 7009;
                const int Fendi_soul = 7010;
            }

            // ==== DoA ====
            namespace DoA {
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

                // tormentors
                const int MindTormentor = 5200;
                const int SoulTormentor = 5201;
                const int WaterTormentor = 5202;
                const int HeartTormentor = 5203;
                const int FleshTormentor = 5204;
                const int SpiritTormentor = 5205;
                const int EarthTormentor = 5206;
                const int SanityTormentor = 5208;

                // termentors in veil
                const int VeilMindTormentor = 5176;
                const int VeilSoulTormentor = 5177;
                const int VeilWaterTormentor = 5178;
                const int VeilHeartTormentor = 5179;
                const int VeilFleshTormentor = 5180;
                const int VeilSpiritTormentor = 5181;
                const int VeilEarthTormentor = 5182;
                const int VeilSanityTormentor = 5183;

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
                const int KeeperOfSouls = 2319;
                const int Slayer = 2337;
                const int SkeletonOfDhuum = 2338;
                const int FourHorseman = 2325; // all four share the same id
            }

            namespace FoW {
                const int ShardWolf = 2831;
                const int Abyssal = 2806;
                const int SeedOfCorruption = 2807;
            }
        }
    }
}
