#pragma once

namespace GW {
    namespace Constants {
        enum class QuestID {
            None = 0,
            UW_Chamber = 101,
            UW_Wastes = 102,
            UW_UWG = 103,
            UW_Mnt = 104,
            UW_Pits = 105,
            UW_Planes = 106,
            UW_Pools = 107,
            UW_Escort = 108,
            UW_Restore = 109,
            UW_Vale = 110,

            Fow_Defend = 202,
            Fow_ArmyOfDarknesses = 203,
            Fow_WailingLord = 204,
            Fow_Griffons = 205,
            Fow_Slaves = 206,
            Fow_Restore = 207,
            Fow_Hunt = 208,
            Fow_Forgemaster = 209,
            Fow_Tos = 211,
            Fow_Toc = 212,
            Fow_Khobay = 224,

            // gloom
            Doa_DeathbringerCompany = 749,
            Doa_RiftBetweenUs = 752,
            Doa_ToTheRescue = 753,

            // city
            Doa_City = 751,

            // Veil
            Doa_BreachingStygianVeil = 742,
            Doa_BroodWars = 755,

            // Foundry
            Doa_FoundryBreakout = 743,
            Doa_FoundryOfFailedCreations = 744,

            // Zaishen Missions
            // Prophecies
            ZM_The_Great_Northern_Wall = 0, //UNKNOWN
            ZM_Fort_Ranik = 937,
            ZM_Ruins_of_Surmia = 938,
            ZM_Nolani_Academy = 0, //UNKNOWN
            ZM_Borlis_Pass = 940,
            ZM_The_Frost_Gate = 941,
            ZM_Gates_of_Kryta = 942,
            ZM_DAlessio_Seaboard = 943,
            ZM_Divinity_Coast = 944,
            ZM_The_Wilds = 945,
            ZM_Bloodstone_Fen = 946,
            ZM_Aurora_Glade = 0, //UNKNOWN
            ZM_Riverside_Province = 948,
            ZM_Sanctum_Cay = 949,
            ZM_Dunes_of_Despair = 950,
            ZM_Thirsty_River = 951,
            ZM_Elona_Reach = 952,
            ZM_Augury_Rock = 953,
            ZM_The_Dragons_Lair = 954,
            ZM_Ice_Caves_of_Sorrow = 0, //UNKNOWN
            ZM_Iron_Mines_of_Moladune = 956,
            ZM_Thunderhead_Keep = 957,
            ZM_Ring_of_Fire = 958,
            ZM_Abaddons_Mouth = 959,
            ZM_Hells_Precipice = 960,

            // Factions
            ZM_Minister_Chos_Estate = 0, //UNKNOWN
            ZM_Zen_Daijun = 961,
            ZM_Vizunah_Square = 0, //UNKNOWN
            ZM_Nahpui_Quarter = 963,
            ZM_Tahnnakai_Temple = 0x3c4, //UNKNOWN
            ZM_Arborstone = 0, //UNKNOWN
            ZM_Boreas_Seabed = 966,
            ZM_Sunjiang_District = 967,
            ZM_The_Eternal_Grove = 968,
            ZM_Gyala_Hatchery = 970,
            ZM_Unwaking_Waters = 969,
            ZM_Raisu_Palace = 971,
            ZM_Imperial_Sanctum = 972,

            // Nightfall
            ZM_Chahbek_Village = 978,
            ZM_Jokanur_Diggings = 979,
            ZM_Blacktide_Den = 0, //UNKNOWN
            ZM_Consulate_Docks = 981,
            ZM_Venta_Cemetery = 982,
            ZM_Kodonur_Crossroads = 983,
            ZM_Pogahn_Passage = 1181,
            ZM_Rilohn_Refuge = 0, //UNKNOWN
            ZM_Moddok_Crevice = 985,
            ZM_Tihark_Orchard = 0, //UNKNOWN
            ZM_Dasha_Vestibule = 0, //UNKNOWN
            ZM_Dzagonur_Bastion = 987,
            ZM_Grand_Court_of_Sebelkeh = 989,
            ZM_Jennurs_Horde = 990,
            ZM_Nundu_Bay = 991,
            ZM_Gate_of_Desolation = 0, //UNKNOWN
            ZM_Ruins_of_Morah = 993,
            ZM_Gate_of_Pain = 0, //UNKNOWN
            ZM_Gate_of_Madness = 0, //UNKNOWN
            ZM_Abaddons_Gate = 996,

            // Eye of the North
            ZM_Finding_the_Bloodstone = 0, //UNKNOWN
            ZM_The_Elusive_Golemancer = 0, //UNKNOWN
            ZM_G_O_L_E_M = 1002,
            ZM_Against_the_Charr = 0, //UNKNOWN
            ZM_Warband_of_Brothers = 0, //UNKNOWN
            ZM_Assault_on_the_Stronghold = 1005,
            ZM_Curse_of_the_Nornbear = 1006,
            ZM_A_Gate_Too_Far = 1008,
            ZM_Blood_Washes_Blood = 0, //UNKNOWN
            ZM_Destructions_Depths = 1009,
            ZM_A_Time_for_Heroes = 1010,

            // Zaishen Bounties
            ZB_Urgoz = 1025,
            ZB_Ilsundur_Lord_of_Fire = 0, //UNKNOWN
            ZB_Chung_The_Attuned = 1026,
            ZB_Mungri_Magicbox = 1029,
            ZB_The_Stygian_Lords = 1046,
            ZB_Rragar_Maneater = 1049,
            ZB_Murakai_Lady_of_the_Night = 1050,
            ZB_Prismatic_Ooze = 0, //UNKNOWN
            ZB_Havok_Soulwail = 0, //UNKNOWN
            ZB_Frostmaw_the_Kinslayer = 0, //UNKNOWN
            ZB_Remnant_of_Antiquities = 1054,
            ZB_Plague_of_Destruction = 1055,
            ZB_Zoldark_the_Unholy = 1056,
            ZB_Khabuus = 0, //UNKNOWN
            ZB_Zhim_Monns = 0, //UNKNOWN
            ZB_Eldritch_Ettin = 0, //UNKNOWN
            ZB_Fendi_Nin = 1060,
            ZB_TPS_Regulator_Golem = 1061,
            ZB_Arachni = 1062,
            ZB_Forgewight = 1063,
            ZB_Selvetarm = 1064,
            ZB_Justiciar_Thommis = 0, //UNKNOWN
            ZB_Rand_Stormweaver = 1066,
            ZB_Duncan_the_Black = 1067,
            ZB_Fronis_Irontoe = 1068,
            ZB_Magmus = 1070,
            ZB_Lord_Khobay = 1086,

            // Zaishen Vanquishes
            ZV_Dejarin_Estate = 1201,
            ZV_Watchtower_Coast = 1202,
            ZV_Arbor_Bay = 1203,
            ZV_Barbarous_Shore = 1204,
            ZV_Deldrimor_Bowl = 1205,
            ZV_Boreas_Seabed = 1206,
            ZV_Cliffs_of_Dohjok = 1207,
            ZV_Diessa_Lowlands = 1208,
            ZV_Bukdek_Byway = 1209,
            ZV_Bjora_Marches = 1210,
            ZV_Crystal_Overlook = 1211,
            ZV_Diviners_Ascent = 1212,
            ZV_Dalada_Uplands = 1213,
            ZV_Drazach_Thicket = 1214,
            ZV_Fahranur_the_First_City = 1215,
            ZV_Dragons_Gullet = 1216,
            ZV_Ferndale = 1217,
            ZV_Forum_Highlands = 1218,
            ZV_Dreadnoughts_Drift = 1219,
            ZV_Drakkar_Lake = 1220,
            ZV_Dry_Top = 1221,
            ZV_Tears_of_the_Fallen = 1222,
            ZV_Gyala_Hatchery = 1223,
            ZV_Ettins_Back = 1224,
            ZV_Gandara_the_Moon_Fortress = 1225,
            ZV_Grothmar_Wardowns = 1226,
            ZV_Flame_Temple_Corridor = 1227,
            ZV_Haiju_Lagoon = 1228,
            ZV_Frozen_Forest = 1229,
            ZV_Garden_of_Seborhin = 1230,
            ZV_Grenths_Footprint = 1231,
            ZV_Jaya_Bluffs = 1232,
            ZV_Holdings_of_Chokhin = 1233,
            ZV_Ice_Cliff_Chasms = 1234,
            ZV_Griffons_Mouth = 1235,
            ZV_Kinya_Province = 1236,
            ZV_Issnur_Isles = 1237,
            ZV_Jaga_Moraine = 1238,
            ZV_Ice_Floe = 1239,
            ZV_Maishang_Hills = 1240,
            ZV_Jahai_Bluffs = 1241,
            ZV_Riven_Earth = 1242,
            ZV_Icedome = 1243,
            ZV_Minister_Chos_Estate = 1244,
            ZV_Mehtani_Keys = 1245,
            ZV_Sacnoth_Valley = 1246,
            ZV_Iron_Horse_Mine = 1247,
            ZV_Morostav_Trail = 1248,
            ZV_Plains_of_Jarin = 1249,
            ZV_Sparkfly_Swamp = 1250,
            ZV_Kessex_Peak = 1251,
            ZV_Mourning_Veil_Falls = 1252,
            ZV_The_Alkali_Pan = 1253,
            ZV_Varajar_Fells = 1254,
            ZV_Lornars_Pass = 1255,
            ZV_Pongmei_Valley = 1256,
            ZV_The_Floodplain_of_Mahnkelon = 1257,
            ZV_Verdant_Cascades = 1258,
            ZV_Majestys_Rest = 1259,
            ZV_Raisu_Palace = 1260,
            ZV_The_Hidden_City_of_Ahdashim = 1261,
            ZV_Rheas_Crater = 1262,
            ZV_Mamnoon_Lagoon = 1263,
            ZV_Shadows_Passage = 1264,
            ZV_The_Mirror_of_Lyss = 1265,
            ZV_Saoshang_Trail = 1266,
            ZV_Nebo_Terrace = 1267,
            ZV_Shenzun_Tunnels = 1268,
            ZV_The_Ruptured_Heart = 1269,
            ZV_Salt_Flats = 1270,
            ZV_North_Kryta_Province = 1271,
            ZV_Silent_Surf = 1272,
            ZV_The_Shattered_Ravines = 1273,
            ZV_Scoundrels_Rise = 1274,
            ZV_Old_Ascalon = 1275,
            ZV_Sunjiang_District = 1276,
            ZV_The_Sulphurous_Wastes = 1277,
            ZV_Magus_Stones = 1278,
            ZV_Perdition_Rock = 1279,
            ZV_Sunqua_Vale = 1280,
            ZV_Turais_Procession = 1281,
            ZV_Norrhart_Domains = 1282,
            ZV_Pockmark_Flats = 1283,
            ZV_Tahnnakai_Temple = 1284,
            ZV_Vehjin_Mines = 1285,
            ZV_Poisoned_Outcrops = 1286,
            ZV_Prophets_Path = 1287,
            ZV_The_Eternal_Grove = 1288,
            ZV_Tascas_Demise = 1289,
            ZV_Respendent_Makuun = 1290,
            ZV_Reed_Bog = 1291,
            ZV_Unwaking_Waters = 1292,
            ZV_Stingray_Strand = 1293,
            ZV_Sunward_Marches = 1294,
            ZV_Regent_Valley = 1295,
            ZV_Wajjun_Bazaar = 1296,
            ZV_Yatendi_Canyons = 1297,
            ZV_Twin_Serpent_Lakes = 1298,
            ZV_Sage_Lands = 1299,
            ZV_Xaquang_Skyway = 1300,
            ZV_Zehlon_Reach = 1301,
            ZV_Tangle_Root = 1302,
            ZV_Silverwood = 1303,
            ZV_Zen_Daijun = 1304,
            ZV_The_Arid_Sea = 1305,
            ZV_Nahpui_Quarter = 1306,
            ZV_Skyward_Reach = 1307,
            ZV_The_Scar = 1308,
            ZV_The_Black_Curtain = 1309,
            ZV_Panjiang_Peninsula = 1310,
            ZV_Snake_Dance = 1311,
            ZV_Travelers_Vale = 1312,
            ZV_The_Breach = 1313,
            ZV_Lahtenda_Bog = 1314,
            ZV_Spearhead_Peak = 1315
        };
        ENUM_CLASS_OPERATORS(QuestID, 0)
    }
}
