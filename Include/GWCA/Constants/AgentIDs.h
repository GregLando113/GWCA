#pragma once

namespace GW {
    namespace Constants {

        namespace ModelID { // this is actually agent->PlayerNumber for agents

            // New bosses 2020-04-22
            enum {
                AbaddonsCursed = 1338,
                BladeAncientSyuShai,
                YoannhTheRebuilber,
                FureystSharpsight
            };

            constexpr int Rotscale = 2837;

            enum {
                Winnowing = 2875,
                EoE, 
                FrozenSoil = EoE + 6,
                QZ = FrozenSoil + 4,
                Lacerate = 4232,
                Equinox = Lacerate + 4,
                Famine = Equinox + 2,
                InfuriatingHeat = 5715,
                Quicksand = InfuriatingHeat + 4
            };
            
            enum {
                ProphetVaresh = 5296,
                CommanderVaresh
            };

            constexpr int Boo = 7453;

            constexpr int LockedChest = 8141; // this is actually ->ExtraType

            /* Birthday Minis */
            /* unknowns are commented as a guess as inventory name but fit a pattern of id schema */
            namespace Minipet {
                enum {
                    // First Year
                    Charr = 230,                  // Miniature Charr Shaman (purple)
                    Dragon,                 // Miniature Bone Dragon (green)
                    Rurik,                  // Miniature Prince Rurik (gold)
                    Shiro,                  // Miniature Shiro (gold)
                    Titan,                  // Miniature Burning Titan (purple)
                    Kirin,                  // Miniature Kirin (purple)
                    NecridHorseman,         // Miniature Necrid Horseman (white)
                    JadeArmor,              // Miniature Jade Armor (white)
                    Hydra,                  // Miniature Hydra (white)
                    FungalWallow,           // Miniature Fungal Wallow (white)
                    SiegeTurtle,            // Miniature Siege Turtle (white)
                    TempleGuardian,         // Miniature Temple Guardian (white)
                    JungleTroll,            // Miniature Jungle Troll (white)
                    WhiptailDevourer,       // Miniature Whiptail Devourer (white)
                    // Second Year
                    Gwen,                   // Miniature Gwen (green)
                    GwenDoll,               // Miniature Gwen Doll (??)
                    WaterDjinn,             // Miniature Water Djinn (gold)
                    Lich,                  // Miniature Lich (gold)
                    Elf,                    // Miniature Elf (purple)
                    PalawaJoko,             // Miniature Palawa Joko (purple)
                    Koss,                   // Miniature Koss (purple)
                    MandragorImp,           // Miniature Mandragor Imp (white)
                    HeketWarrior,           // Miniature Heket Warrior (white)
                    HarpyRanger,            // Miniature Harpy Ranger (white)
                    Juggernaut,             // Miniature Juggernaut (white)
                    WindRider,              // Miniature Wind Rider (white)
                    FireImp,               // Miniature Fire Imp (white)
                    Aatxe,                  // Miniature Aatxe (white)
                    ThornWolf,              // Miniature Thorn Wolf (white)
                    // Third Year
                    Abyssal,                // Miniature Abyssal (white)
                    BlackBeast,             // Miniature Black Beast of Aaaaarrrrrrgghh (gold)
                    Freezie,                // Miniature Freezie (purple)
                    Irukandji,              // Miniature Irukandji (white)
                    MadKingThorn,           // Miniature Mad King Thorn (green)
                    ForestMinotaur,         // Miniature Forest Minotaur (white)
                    Mursaat,                // Miniature Mursaat (white)
                    Nornbear,               // Miniature Nornbear (purple)
                    Ooze,                   // Miniature Ooze (purple)
                    Raptor,                 // Miniature Raptor (white)
                    RoaringEther,           // Miniature Roaring Ether (white)
                    CloudtouchedSimian,     // Miniature Cloudtouched Simian (white)
                    CaveSpider,            // Miniature Cave Spider (white)
                    WhiteRabbit,            // White Rabbit (gold)
                    // Fourth Year
                    WordofMadness,          // Miniature Word of Madness (white)
                    DredgeBrute,            // Miniature Dredge Brute (white)
                    TerrorwebDryder,        // Miniature Terrorweb Dryder (white)
                    Abomination,           // Miniature Abomination (white)
                    KraitNeoss,             // Miniature Krait Neoss (white)
                    DesertGriffon,          // Miniature Desert Griffon (white)
                    Kveldulf,               // Miniature Kveldulf (white)
                    QuetzalSly,             // Miniature Quetzal Sly (white)
                    Jora,                  // Miniature Jora (purple)
                    FlowstoneElemental,     // Miniature Flowstone Elemental (purple)
                    Nian,                   // Miniature Nian (purple)
                    DagnarStonepate,        // Miniature Dagnar Stonepate (gold)
                    FlameDjinn,             // Miniature Flame Djinn (gold)
                    EyeOfJanthir,           // Miniature Eye of Janthir (green)
                    // Fifth Year
                    Seer,                   // Miniature Seer (white)
                    SiegeDevourer,          // Miniature Siege Devourer (white)
                    ShardWolf,              // Miniature Shard Wolf (white)
                    FireDrake,              // Miniature Fire Drake (white)
                    SummitGiantHerder,      // Miniature Summit Giant Herder (white)
                    OphilNahualli,          // Miniature Ophil Nahualli (white)
                    CobaltScabara,          // Miniature Cobalt Scabara (white)
                    ScourgeManta,          // Miniature Scourge Manta (white)
                    Ventari,                // Miniature Ventari (purple)
                    Oola,                   // Miniature Oola (purple)
                    CandysmithMarley,       // Miniature CandysmithMarley (purple)
                    ZhuHanuku,              // Miniature Zhu Hanuku (gold)
                    KingAdelbern,           // Miniature King Adelbern (gold)
                    MOX1,                   // Miniature M.O.X. (color?)
                    MOX2,                   // Miniature M.O.X. (color?)
                    MOX3,                   // Miniature M.O.X. (green)
                    MOX4,                   // Miniature M.O.X. (color?)
                    MOX5,                   // Miniature M.O.X. (color?)
                    MOX6,                   // Miniature M.O.X. (color?)

                    // In-game rewards and promotionals
                    BrownRabbit,           // Miniature Brown Rabbit
                    Yakkington,             // Miniature Yakkington
                    Unk308,
                    CollectorsEditionKuunavang, // Miniature Kuunavang (green)
                    GrayGiant,
                    Asura,
                    DestroyerOfFlesh,
                    PolarBear,
                    CollectorsEditionVaresh,
                    Mallyx,
                    Ceratadon,

                    // Misc.
                    Kanaxai,
                    Panda,
                    IslandGuardian,
                    NagaRaincaller,
                    LonghairYeti,
                    Oni,
                    ShirokenAssassin,
                    Vizu,
                    ZhedShadowhoof,
                    Grawl,
                    GhostlyHero,

                    // Special events
                    Pig,
                    GreasedLightning,
                    WorldFamousRacingBeetle,
                    CelestialPig,
                    CelestialRat,
                    CelestialOx,
                    CelestialTiger,
                    CelestialRabbit,
                    CelestialDragon,
                    CelestialSnake,
                    CelestialHorse,
                    CelestialSheep,
                    CelestialMonkey,
                    CelestialRooster,
                    CelestialDog,

                    // In-game
                    BlackMoaChick,
                    Dhuum,
                    MadKingsGuard,
                    SmiteCrawler,

                    // Zaishen strongboxes, and targetable minipets
                    GuildLord,
                    HighPriestZhang,
                    GhostlyPriest,
                    RiftWarden,

                    // More in-game drops and rewards
                    MiniatureLegionnaire = 7984,
                    MiniatureConfessorDorian = 8293,
                    MiniaturePrincessSalma = MiniatureConfessorDorian + 5,
                    MiniatureLivia,
                    MiniatureEvennia,
                    MiniatureConfessorIsaiah,
                    MiniaturePeacekeeperEnforcer = MiniatureConfessorIsaiah + 2,
                    MiniatureMinisterReiko = 8987,
                    MiniatureEcclesiateXunRao
                };
            }
            namespace DoA {
                enum {
                    OtherSnakes = 5002,
                    // doa bosses
                    LordJadoth = 5144,
                    StygianLordNecro,
                    StygianLordMesmer,
                    StygianLordEle,
                    StygianLordMonk,
                    Fury,
                    BlackBeastOfArgh,
                    StygianLordDerv = BlackBeastOfArgh + 13,
                    StygianLordRanger,
                    // margonites
                    MargoniteAnurKaya = StygianLordRanger + 2,
                    MargoniteAnurDabi,
                    MargoniteAnurSu,
                    MargoniteAnurKi,
                    MargoniteAnurVu,
                    MargoniteAnurTuk,
                    MargoniteAnurRuk,
                    MargoniteAnurRund,
                    MargoniteAnurMank,

                    // stygians
                    StygianHunger,
                    StygianBrute,
                    StygianGolem,
                    StygianHorror,
                    StygianFiend,

                    // tormentors in veil
                    VeilMindTormentor,
                    VeilSoulTormentor,
                    VeilWaterTormentor,
                    VeilHeartTormentor,
                    VeilFleshTormentor,
                    VeilSpiritTormentor,
                    VeilEarthTormentor,
                    VeilSanityTormentor,
                    // titans
                    MiseryTitan = VeilSanityTormentor + 8,
                    RageTitan,
                    DementiaTitan,
                    AnguishTitan,
                    DespairTitan,
                    FuryTitan,
                    RageTitan2,
                    DementiaTitan2,
                    // tormentors
                    MindTormentor = DementiaTitan2 + 2,
                    SoulTormentor,
                    WaterTormentor,
                    HeartTormentor,
                    FleshTormentor,
                    SpiritTormentor,
                    EarthTormentor,
                    SanityTormentor = EarthTormentor + 2,
                    SmotheringTendril = SanityTormentor + 2,
                    TorturewebDryder,
                    GreaterDreamRider,
                    FoundrySnakes = GreaterDreamRider + 5 // Friendly
                };
            }
            namespace UW {
                enum {
                    ChainedSoul = 2317,
                    DyingNightmare,
                    ObsidianBehemoth,
                    ObsidianGuardian,
                    TerrorwebDryder,
                    TerrorwebDryderSilver,
                    KeeperOfSouls,
                    TerrorwebQueen, // boss-like
                    SmiteCrawler,
                    WailingLord,       // Note: same as FoW::Banshee
                    BanishedDreamRider,
                    FourHorseman = BanishedDreamRider + 2, // all four share the same id
                    MindbladeSpectre,
                    DeadCollector = MindbladeSpectre + 2,
                    DeadThresher,
                    ColdfireNight,
                    StalkingNight,
                    ChargedBlackness = StalkingNight + 2,
                    GraspingDarkness,
                    BladedAatxe,
                    Slayer = BladedAatxe + 2,
                    SkeletonOfDhuum1,
                    SkeletonOfDhuum2,
                    ChampionOfDhuum,
                    MinionOfDhuum,
                    Dhuum,
                    Reapers = Dhuum + 2, // outside dhuum chamber
                    ReapersAtDhuum, // in dhuum chamber
                    IceElemental, // friendly, during waste quest near dhuum.
                    KingFrozenwind = IceElemental + 2,
                    TorturedSpirit1, // friendly, during quest
                    Escort1 = TorturedSpirit1 + 3, // souls npc spawned by escort quest
                    Escort2,
                    Escort3,
                    Escort4,
                    Escort5,
                    Escort6,
                    PitsSoul1,
                    PitsSoul2,
                    PitsSoul3,
                    PitsSoul4,
                    TorturedSpirit = PitsSoul4 + 6,
                    MajorAlgheri = TorturedSpirit + 2
                };
            }
            
            namespace FoW {
                enum {
                    Banshee = (int)UW::WailingLord,
                    NimrosTheHunter = 1464,
                    MikoTheUnchained = 1965,

                    MahgoHydra = 2796,
                    ArmoredCaveSpider = MahgoHydra + 4,
                    SmokeWalker,
                    ObsidianFurnanceDrake,
                    DoubtersDryder,
                    ShadowMesmer,
                    ShadowElemental,
                    ShadowMonk,
                    ShadowWarrior,
                    ShadowRanger,
                    ShadowBeast,
                    Abyssal,           // Note: same as ShadowOverlord.
                    ShadowOverlord = Abyssal,    // Note: same as Abyssal.
                    SeedOfCorruption,
                    SpiritWood,
                    SpiritShepherd,
                    AncientSkale,
                    SnarlingDriftwood,
                    SkeletalEtherBreaker,
                    SkeletalIcehand,
                    SkeletalBond,
                    SkeletalBerserker,
                    SkeletalImpaler,
                    RockBorerWorm,
                    InfernalWurm,
                    DragonLich,
                    Menzies,
                    // 2821 ?
                    Rastigan = Menzies + 2,         // Friendly NPC
                    Griffons,          // Friendly NPC
                    LordKhobay,        // Unfriendly NPC
                    Forgemaster,       // Friendly NPC
                    TraitorousTempleGuard1 = Forgemaster + 2,
                    TraitorousTempleGuard2,
                    TraitorousTempleGuard3,
                    ShardWolf = TraitorousTempleGuard3 + 2
                };
            }
            
            namespace Urgoz {
                enum {
                    HoppingVampire = 3745,
                    Urgoz = HoppingVampire + 9
                };
            }
            
            namespace Deep {
                enum {
                    Kanaxai = 4055,
                    KanaxaiAspect
                };
            }
            namespace EotnDungeons {
                enum {
                    DiscOfChaos = 6071,
                    PlagueOfDestruction = DiscOfChaos + 12,
                    ZhimMonns = PlagueOfDestruction + 163,
                    Khabuus = ZhimMonns + 2,
                    DuncanTheBlack = Khabuus + 157,
                    JusticiarThommis,
                    RandStormweaver,
                    Selvetarm,
                    Forgewight,
                    HavokSoulwail = Forgewight + 18,
                    RragarManeater3 = HavokSoulwail + 131, // lvl 3
                    RragarManeater12, // lvl 1 and 2
                    Arachni = RragarManeater12 + 234,
                    Hidesplitter = Arachni + 8,
                    PrismaticOoze = Hidesplitter + 5,
                    IlsundurLordofFire = PrismaticOoze + 8,
                    EldritchEttin = IlsundurLordofFire + 7,
                    TPSRegulartorGolem = EldritchEttin + 11,
                    MalfunctioningEnduringGolem = TPSRegulartorGolem + 2,
                    StormcloudIncubus = MalfunctioningEnduringGolem + 17,
                    CyndrTheMountainHeart = StormcloudIncubus + 63,
                    InfernalSiegeWurm = CyndrTheMountainHeart + 13, // kath lvl1 boss
                    Frostmaw = InfernalSiegeWurm + 5,
                    RemnantOfAntiquities = Frostmaw + 2,
                    MurakaiLadyOfTheNight = RemnantOfAntiquities + 74,
                    Brigand,
                    ZoldarkTheUnholy,
                    FendiNin = ZoldarkTheUnholy + 3,
                    SoulOfFendiNin,
                    KeymasterOfMurakai = SoulOfFendiNin + 4,

                    AngrySnowman = 7411
                };
            }

            namespace BonusMissionPack {
                enum {
                    WarAshenskull = 7079,
                    RoxAshreign,
                    AnrakTindershot,
                    DettMortash,
                    AkinCinderspire,
                    TwangSootpaws,
                    MagisEmberglow,
                    MerciaTheSmug = MagisEmberglow + 29,
                    OptimusCaliph,
                    LazarusTheDire,
                    YuriTheHand = LazarusTheDire + 30,
                    MasterRiyo,
                    CaptainSunpu = MasterRiyo + 2,
                    MinisterWona,
                    AdmiralJakman = MinisterWona + 29,
                    PalawaJoko = AdmiralJakman + 19
                };
            }
            namespace PolymockSummon {
                enum {
                    // Polymocks
                    MursaatElementalist = 5847,
                    FlameDjinn,
                    IceImp,
                    NagaShaman
                };
            }

            constexpr int EbonVanguardAssassin = PolymockSummon::NagaShaman + 2;
        }
    }
}
