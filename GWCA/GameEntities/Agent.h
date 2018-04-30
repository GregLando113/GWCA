#ifndef _ENTITIE_AGENT_INC
#define _ENTITIE_AGENT_INC

#include <Windows.h>
#include <GWCA\GameEntities\Position.h>

#include <GWCA\GameContainers\List.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    using AgentID = DWORD;
    using Vec2f = Vector2f;
    using Vec3f = Vector3f;

    struct Agent {

        // Courtesy of DerMonech14
        struct Equipment {

            virtual void Unk1() = 0;
            virtual void Unk2() = 0;
            virtual void Update(unsigned index) = 0;

            struct DyeInfo {
                BYTE h0000;
                BYTE Shinyness;
                BYTE dye1 : 4;
                BYTE dye2 : 4;
                BYTE dye3 : 4;
                BYTE dye4 : 4;
            };
            struct ItemData {
                DWORD ModelFileID;
                DyeInfo Dye;
                DWORD Value;
                BYTE h000C[4];
            };

            DWORD h0004;                // 0x04     always 2 ?
            DWORD h0008;                // 0x08     Ptr PlayerModelFile?
            DWORD h000C;                // 0x0C
            DWORD *LeftHandPtr;         // 0x10     Ptr Bow, Hammer, Focus, Daggers, Scythe
            DWORD *RightHandPtr;            // 0x14     Ptr Sword, Spear, Staff, Daggers, Axe, Zepter, Bundle
            DWORD h0018;                // 0x18
            DWORD *ShieldPtr;           // 0x1C     Ptr Shield
            BYTE LeftHandMap;           // 0x20     Weapon1     None = 9, Bow = 0, Hammer = 0, Focus = 1, Daggers = 0, Scythe = 0
            BYTE RightHandMap;          // 0x21     Weapon2     None = 9, Sword = 0, Spear = 0, Staff = 0, Daggers = 0, Axe = 0, Zepter = 0, Bundle
            BYTE HeadMap;               // 0x22     Head        None = 9, Headpiece Ele = 4
            BYTE ShieldMap;             // 0x23     Shield      None = 9, Shield = 1
            union {
                ItemData Items[9];
                struct {
                    ItemData Weapon;        // 0x24
                    ItemData Offhand;       // 0x34
                    ItemData Chest;     // 0x44
                    ItemData Legs;          // 0x54
                    ItemData Head;          // 0x64
                    ItemData Feet;          // 0x74
                    ItemData Hands;     // 0x84 
                    ItemData CostumeBody;       // 0x94
                    ItemData CostumeHead;       // 0xA4
                };
            };
            DWORD ItemID_Weapon;            // 0xB4
            DWORD ItemID_Offhand;           // 0xB8
            DWORD ItemID_Chest;         // 0xBC
            DWORD ItemID_Legs;          // 0xC0
            DWORD ItemID_Head;          // 0xC4
            DWORD ItemID_Feet;          // 0xC8
            DWORD ItemID_Hands;         // 0xCC
            DWORD ItemID_CostumeBody;       // 0xD0
            DWORD ItemID_CostumeHead;       // 0xD4
        };

        struct TagInfo {
            WORD GuildId;
            BYTE Primary;
            BYTE Secondary;
            // ...
        };

        /* +h0000 */ DWORD* vtable;
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ DWORD h0008;
        /* +h000C */ DWORD h000C[2];
        /* +h0014 */ DWORD Timer; // Agent Instance Timer (in Frames)
        /* +h0018 */ DWORD Timer2;
        /* +h001C */ TLink<Agent> link;
        /* +h0024 */ TLink<Agent> link2; // Not used.
        /* +h002C */ AgentID Id; // AgentId
        /* +h0030 */ float Z; // Z coord in float
        /* +h0034 */ float Width1;  // Width of the model's box
        /* +h0038 */ float Height1; // Height of the model's box
        /* +h003C */ float Width2;  // Width of the model's box (same as 1)
        /* +h0040 */ float Height2; // Height of the model's box (same as 1)
        /* +h0044 */ float Width3;  // Width of the model's box (usually same as 1)
        /* +h0048 */ float Height3; // Height of the model's box (usually same as 1)
        /* +h004C */ float Rotation_angle; // Rotation in radians from East (-pi to pi)
        /* +h0050 */ float Rotation_cos; // cosine of rotation
        /* +h0054 */ float Rotation_sin; // sine of rotation
        /* +h0058 */ DWORD NameProperties; // Bitmap basically telling what the agent is
        /* +h005C */ DWORD Ground;
        /* +h0060 */ DWORD h0060;
        /* +h0064 */ float h0064;   // weird values, change with movement, always between -1 and 1
        /* +h0068 */ float h0068;
        /* +h006C */ float h006C;
        /* +h0070 */ BYTE  h0070[4];
        union {
            struct {
        /* +h0074 */ float X;
        /* +h0078 */ float Y;
        /* +h007C */ DWORD plane;
            };
        /* +h0074 */ GamePos pos;          
        };
        /* +h0080 */ BYTE  h0080[4];
        /* +h0084 */ float NameTagX; // Exactly the same as X above
        /* +h0088 */ float NameTagY; // Exactly the same as Y above
        /* +h008C */ float NameTagZ; // Z coord in float
        /* +h0090 */ WORD  VisualEffects; // Number of Visual Effects of Agent (Skills, Weapons); 1 = Always set;
        //           WORD  padding;
        /* +h0094 */ DWORD h0094[2];
        /* +h009C */ DWORD Type; // Livings = 0xDB, Gadgets = 0x200, Items = 0x400.
        /* +h00A0 */ float MoveX; //If moving, how much on the X axis per second
        /* +h00A4 */ float MoveY; //If moving, how much on the Y axis per second
        /* +h00A8 */ DWORD h00A8; // always 0?
        /* +h00AC */ float Rotation_cos2; // same as cosine above
        /* +h00B0 */ float Rotation_sin2; // same as sine above
        /* +h00B4 */ DWORD h00B4[4];
        /* +h00C4 */ DWORD Owner;
        /* +h00C8 */ DWORD ItemID; // Only valid if agent is type 0x400 (item)
        /* +h00CC */ DWORD h00CC;
        /* +h00D0 */ DWORD ExtraType; // same as GadgetId. Could be used as such.
        /* +h00D4 */ DWORD h00D4[3];
        /* +h00E0 */ float AnimationType;
        /* +h00E4 */ DWORD h00E4[2];
        /* +h00EC */ float WeaponAttackSpeed; // The base attack speed in float of last attacks weapon. 1.33 = axe, sWORD, daggers etc.
        /* +h00F0 */ float AttackSpeedModifier; // Attack speed modifier of the last attack. 0.67 = 33% increase (1-.33)
        /* +h00F4 */ WORD  PlayerNumber; // Selfexplanatory. All non-players have identifiers for their type. Two of the same mob = same number
        //           WORD  padding;
        /* +h00F8 */ DWORD TransmogNpcId; // Actually, it's 0x20000000 | npc_id, It's not defined for npc, minipet, etc...
        /* +h00FC */ Equipment** Equip;
        /* +h0100 */ DWORD h0100;
        /* +h0104 */ TagInfo *Tags; // struct { WORD guild_id, BYTE primary, BYTE secondary, WORD level
        /* +h0108 */ WORD  h0108;
        /* +h010A */ BYTE  Primary; // Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h010B */ BYTE  Secondary; // Secondary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h010C */ BYTE  Level; // Duh!
        /* +h010D */ BYTE  TeamId; // 0=None, 1=Blue, 2=Red, 3=Yellow
        /* +h010E */ BYTE  h010E[2];
        /* +h0110 */ DWORD h0110;
        /* +h0114 */ float EnergyRegen;
        /* +h0118 */ DWORD h0118;
        /* +h011C */ float Energy; // Only works for yourself
        /* +h0120 */ DWORD MaxEnergy; // Only works for yourself
        /* +h0124 */ DWORD h0124;
        /* +h0128 */ float HPPips; // Regen/degen as float
        /* +h012C */ DWORD h012C;
        /* +h0130 */ float HP; // Health in % where 1=100% and 0=0%
        /* +h0134 */ DWORD MaxHP; // Only works for yourself
        /* +h0138 */ DWORD Effects; // Bitmap for effects to display when targetted. DOES include hexes
        /* +h013C */ DWORD h013C;
        /* +h0140 */ BYTE  Hex; // Bitmap for the hex effect when targetted (apparently obsolete!) (yes)
        /* +h0141 */ BYTE  h0141[19];
        /* +h0154 */ DWORD ModelState; // Different values for different states of the model.
        /* +h0158 */ DWORD TypeMap; // Odd variable! 0x08 = dead, 0xC00 = boss, 0x40000 = spirit, 0x400000 = player
        /* +h015C */ DWORD h015C[4];
        /* +h016C */ DWORD InSpiritRange; // Tells if agent is within spirit range of you. Doesn't work anymore?
        /* +h0170 */ DWORD h0170[4];
        /* +h0180 */ DWORD LoginNumber; // Unique number in instance that only works for players
        /* +h0184 */ float AnimationSpeed;  // Speed of the current animation
        /* +h0188 */ DWORD AnimationCode; // related to animations
        /* +h018C */ DWORD AnimationID;     // Id of the current animation
        /* +h0190 */ BYTE  h0190[32];
        /* +h01B0 */ BYTE  DaggerStatus; // 0x1 = used lead attack, 0x2 = used offhand attack, 0x3 = used dual attack
        /* +h01B1 */ BYTE  Allegiance; // 0x1 = ally/non-attackable, 0x2 = neutral, 0x3 = enemy, 0x4 = spirit/pet, 0x5 = minion, 0x6 = npc/minipet
        /* +h01B2 */ WORD  WeaponType; // 1=bow, 2=axe, 3=hammer, 4=daggers, 5=scythe, 6=spear, 7=sWORD, 10=wand, 12=staff, 14=staff
        /* +h01B4 */ WORD  Skill; // 0 = not using a skill. Anything else is the Id of that skill
        /* +h01B6 */ WORD  h01B6;
        /* +h01B7 */ BYTE  WeaponItemType;
        /* +h01B8 */ BYTE  OffhandItemType;
        /* +h01BA */ WORD  WeaponItemId;
        /* +h01BC */ WORD  OffhandItemId;

        // Health Bar Effect Bitmasks.
        inline bool GetIsBleeding()        const { return (Effects & 0x0001) != 0; }
        inline bool GetIsConditioned()     const { return (Effects & 0x0002) != 0; }
        inline bool GetIsDead()            const { return (Effects & 0x0010) != 0; }
        inline bool GetIsDeepWounded()     const { return (Effects & 0x0020) != 0; }
        inline bool GetIsPoisoned()        const { return (Effects & 0x0040) != 0; }
        inline bool GetIsEnchanted()       const { return (Effects & 0x0080) != 0; }
        inline bool GetIsDegenHexed()      const { return (Effects & 0x0400) != 0; }
        inline bool GetIsHexed()           const { return (Effects & 0x0800) != 0; }
        inline bool GetIsWeaponSpelled()   const { return (Effects & 0x8000) != 0; }

        // Agent Type Bitmasks.
        inline bool GetIsCharacterType()   const { return (Type & 0xDB)  != 0; }
        inline bool GetIsGadgetType()      const { return (Type & 0x200) != 0; }
        inline bool GetIsItemType()        const { return (Type & 0x400) != 0; }

        // Agent TypeMap Bitmasks.
        inline bool GetInCombatStance()    const { return (TypeMap & 0x000001) != 0; }
        inline bool GetHasQuest()          const { return (TypeMap & 0x000002) != 0; } // if agent has quest marker
        inline bool GetIsDeadByTypeMap()   const { return (TypeMap & 0x000008) != 0; }
        inline bool GetIsFemale()          const { return (TypeMap & 0x000200) != 0; }
        inline bool GetHasBossGlow()       const { return (TypeMap & 0x000400) != 0; }
        inline bool GetIsHidingCape()      const { return (TypeMap & 0x001000) != 0; }
        inline bool GetCanBeViewedInPartyWindow() const { return (TypeMap & 0x20000) != 0; }
        inline bool GetIsSpawned()         const { return (TypeMap & 0x040000) != 0; }
        inline bool GetIsBeingObserved()   const { return (TypeMap & 0x400000) != 0; }

        // Modelstates.
        inline bool GetIsKnockedDown()     const { return ModelState == 1104; }
        inline bool GetIsMoving()          const { return ModelState == 12 || ModelState == 76   || ModelState == 204; }
        inline bool GetIsAttacking()       const { return ModelState == 96 || ModelState == 1088 || ModelState == 1120; }

        inline bool IsPlayer()             const { return LoginNumber != 0; }
        inline bool IsNPC()                const { return LoginNumber == 0; }
    };

    struct MapAgent {
        /* +h0000 */ float curenergy;
        /* +h0004 */ float maxenergy;
        /* +h0008 */ float energyregen;
        /* +h000C */ DWORD skilltimestamp;
        /* +h0010 */ float unk2;
        /* +h0014 */ float MaxEnergy2;
        /* +h0018 */ float unk3;
        /* +h001C */ DWORD unk4;
        /* +h0020 */ float curHealth;
        /* +h0024 */ float maxHealth;
        /* +h0028 */ float healthregen;
        /* +h002C */ DWORD unk5;
        /* +h0030 */ DWORD Effects;

        // Health Bar Effect Bitmasks.
        inline bool GetIsBleeding()         { return (Effects & 0x0001) != 0; }
        inline bool GetIsConditioned()      { return (Effects & 0x0002) != 0; }
        inline bool GetIsDead()             { return (Effects & 0x0010) != 0; }
        inline bool GetIsDeepWounded()      { return (Effects & 0x0020) != 0; }
        inline bool GetIsPoisoned()         { return (Effects & 0x0040) != 0; }
        inline bool GetIsEnchanted()        { return (Effects & 0x0080) != 0; }
        inline bool GetIsDegenHexed()       { return (Effects & 0x0400) != 0; }
        inline bool GetIsHexed()            { return (Effects & 0x0800) != 0; }
        inline bool GetIsWeaponSpelled()    { return (Effects & 0x8000) != 0; }
    };

    struct AgentMovement {
        /* +h0000 */ DWORD h0000[3];
        /* +h000C */ DWORD AgentId;
        /* +h0010 */ DWORD h0010[3];
        /* +h001C */ DWORD h001C;
        /* +h0020 */ DWORD h0020[6];
        /* +h0038 */ DWORD Moving1; //tells if you are stuck even if your client doesn't know
        /* +h003C */ DWORD h003C[2];
        /* +h0044 */ DWORD Moving2; //exactly same as Moving1
        /* +h0048 */ DWORD h0048[7];
        /* +h0064 */ Vec3f h0064;
        /* +h0070 */ DWORD h0070;
        /* +h0074 */ Vec3f h0074;
    };

    struct AgentInfo {
        BYTE h0000[52];
        wchar_t *NameString;
    };

    using AgentArray = Array<Agent*>;
    using AgentList  = TList<Agent>;

    using MapAgentArray = Array<MapAgent>;
    using AgentMovementArray = Array<AgentMovement*>;
    using AgentInfoArray = Array<AgentInfo>;
}

#endif // _ENTITIE_AGENT_INC
