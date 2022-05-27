#pragma once
#include <GWCA/Constants/Types.h>

namespace GW {
    namespace Constants {
        enum class Bag {
            None, Backpack, Belt_Pouch, Bag_1, Bag_2, Equipment_Pack,
            Material_Storage, Unclaimed_Items, Storage_1, Storage_2,
            Storage_3, Storage_4, Storage_5, Storage_6, Storage_7,
            Storage_8, Storage_9, Storage_10, Storage_11, Storage_12,
            Storage_13, Storage_14, Equipped_Items, Max
        };
        enum class ItemType {
            Salvage, Axe = 2, Bag, Boots, Bow, Chestpiece = 7, Rune_Mod, Usable, Dye,
            Materials_Zcoins, Offhand, Gloves, Hammer = 15, Headpiece, CC_Shards,
            Key, Leggings, Gold_Coin, Quest_Item, Wand, Shield = 24, Staff = 26, Sword,
            Kit = 29, Trophy, Scroll, Daggers, Present, Minipet, Scythe, Spear, Costume_Headpiece = 44, Costume
        };
        enum class EquipmentStatus {
            AlwaysHide, HideInTownsAndOutposts, HideInCombatAreas, AlwaysShow
        };
        enum class MaterialSlot {
            Bone, IronIngot, TannedHideSquare, Scale, ChitinFragment,
            BoltofCloth, WoodPlank, GraniteSlab = 8,
            PileofGlitteringDust, PlantFiber, Feather,

            FurSquare, BoltofLinen, BoltofDamask, BoltofSilk,
            GlobofEctoplasm, SteelIngot, DeldrimorSteelIngot,
            MonstrousClaw, MonstrousEye, MonstrousFang, Ruby,
            Sapphire, Diamond, OnyxGemstone, LumpofCharcoal,
            ObsidianShard, TemperedGlassVial = 29, LeatherSquare,
            ElonianLeatherSquare, VialofInk, RollofParchment,
            RollofVellum, SpiritwoodPlank, AmberChunk, JadeiteShard,

            N_MATS
        };
        enum class ModelID { // aka item modelIDs
            None = 0,
            Dye = 146,
            
            // items that make agents
            GhostInTheBox = 6368,
            GhastlyStone = 32557,
            LegionnaireStone = 37810,

            // materials
            Bone = 921,
            BoltofCloth = 925,
            PileofGlitteringDust = 929,
            Feather = 933,
            PlantFiber = 934,
            TannedHideSquare = 940,
            WoodPlank = 946,
            IronIngot = 948,
            Scale = 953,
            ChitinFragment = 954,
            GraniteSlab = 955,

            AmberChunk = 6532,
            BoltofDamask = 927,
            BoltofLinen = 926,
            BoltofSilk = 928,
            DeldrimorSteelIngot = 950,
            Diamond = 935,
            ElonianLeatherSquare = 943,
            FurSquare = 941,
            GlobofEctoplasm = 930,
            JadeiteShard = 6533,
            LeatherSquare = 942,
            LumpofCharcoal = 922,
            MonstrousClaw = 923,
            MonstrousEye = 931,
            MonstrousFang = 932,
            ObsidianShard = 945,
            OnyxGemstone = 936,
            RollofParchment = 951,
            RollofVellum = 952,
            Ruby = 937,
            Sapphire = 938,
            SpiritwoodPlank = 956,
            SteelIngot = 949,
            TemperedGlassVial = 939,
            VialofInk = 944,

            // pcons
            SkalefinSoup = 17061,
            PahnaiSalad = 17062,
            MandragorRootCake = 19170,
            Pies = 28436,
            Cupcakes = 22269,
            Apples = 28431,
            Corns = 28432,
            Eggs = 22752,
            Kabobs = 17060,
            Warsupplies = 35121,
            LunarPig = 29424,
            LunarRat = 29425,
            LunarOx = 29426,
            LunarTiger = 29427,
            LunarRabbit = 29428,
            LunarDragon = 29429,
            LunarSnake = 29430,
            LunarHorse = 29431,
            LunarSheep = 29432,
            LunarMonkey = 29433,
            LunarRooster = 29434,
            LunarDog = 29435,
            ConsEssence = 24859,
            ConsArmor = 24860,
            ConsGrail = 24861,
            ResScrolls = 26501,
            Mobstopper = 32558,
            Powerstone = 24862,

            Fruitcake = 21492,
            RedBeanCake = 15479,
            CremeBrulee = 15528,
            SugaryBlueDrink = 21812,
            ChocolateBunny = 22644,
            JarOfHoney = 31150,
            BRC = 31151,
            GRC = 31152,
            RRC = 31153,
            KrytalLokum = 35125,
            MinistreatOfPurity = 30208,

            // level-1 alcohol
            Eggnog = 6375,
            DwarvenAle = 5585,
            HuntersAle = 910,
            Absinthe = 6367,
            WitchsBrew = 6049,
            Ricewine = 15477,
            ShamrockAle = 22190,
            Cider = 28435,

            // level-5 alcohol
            Grog = 30855,
            SpikedEggnog = 6366,
            AgedDwarvenAle = 24593,
            AgedHuntersAle = 31145,
            Keg = 31146,
            FlaskOfFirewater = 2513,
            KrytanBrandy = 35124,

            // DoA Gemstones
            MargoniteGem = 21128,
            StygianGem = 21129,
            TitanGem = 21130,
            TormentGem = 21131,

            // Holiday Drops
            LunarToken = 21833
        };
        ENUM_CLASS_OPERATORS(ModelID, 0)
    }
}
