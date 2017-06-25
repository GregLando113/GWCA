#ifndef _ENTITIE_AGENT_INC
#define _ENTITIE_AGENT_INC

#include <Windows.h>
#include <GWCA\GameEntities\Position.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    using AgentID = DWORD;
	using Vec2f = Vector2f;
	using Vec3f = Vector3f;

    struct Agent {
        /* +h0000 */ DWORD* vtable;
        /* +h0004 */ BYTE h0004[24];
        /* +h001C */ BYTE h001C[4]; // This actually points to the agent before but with a small offset
        /* +h0020 */ Agent *NextAgent; // Pointer to the next agent (by id)
        /* +h0024 */ BYTE h0024[8];
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
        /* +h005C */ BYTE  h005C[8];
        /* +h0064 */ float h0064;   // weird values, change with movement, always between -1 and 1
        /* +h0068 */ float h0068;
        /* +h006C */ float h006C;
        /* +h0070 */ BYTE  h0070[4];
        /* +h0074 */ Vec2f pos;
        /* +h007C */ DWORD plane;
        /* +h0080 */ BYTE  h0080[4];
        /* +h0084 */ float NameTagX; // Exactly the same as X above
        /* +h0088 */ float NameTagY; // Exactly the same as Y above
        /* +h008C */ float NameTagZ; // Z coord in float
        /* +h0090 */ BYTE  h0090[12];
        /* +h009C */ DWORD Type; // Livings = 0xDB, Gadgets = 0x200, Items = 0x400.
        /* +h00A0 */ float MoveX; //If moving, how much on the X axis per second
        /* +h00A4 */ float MoveY; //If moving, how much on the Y axis per second
        /* +h00A8 */ BYTE  h00A8[4]; // always 0?
        /* +h00AC */ float Rotation_cos2; // same as cosine above
        /* +h00B0 */ float Rotation_sin2; // same as sine above
        /* +h00B4 */ BYTE  h00B4[16];
        /* +h00C4 */ DWORD Owner;
        /* +h00C8 */ DWORD ItemID; // Only valid if agent is type 0x400 (item)
        /* +h00CC */ BYTE  h00CC[4];
        /* +h00D0 */ DWORD ExtraType;
        /* +h00D4 */ BYTE  h00D4[0xC];
        /* +h00E0 */ float AnimationType;
        /* +h00E4 */ BYTE  h00E4[0x8];
        /* +h00EC */ float WeaponAttackSpeed; // The base attack speed in float of last attacks weapon. 1.33 = axe, sWORD, daggers etc.
        /* +h00F0 */ float AttackSpeedModifier; // Attack speed modifier of the last attack. 0.67 = 33% increase (1-.33)
        /* +h00F4 */ WORD  PlayerNumber; // Selfexplanatory. All non-players have identifiers for their type. Two of the same mob = same number
        //           WORD padding;
        /* +h00F8 */ BYTE  h00F8[4];
        /* +h00FC */ DWORD** Equip;
        /* +h0100 */ BYTE  h0100[10];
        /* +h010A */ BYTE  Primary; // Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h010B */ BYTE  Secondary; // Secondary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h010C */ BYTE  Level; // Duh!
        /* +h010D */ BYTE  TeamId; // 0=None, 1=Blue, 2=Red, 3=Yellow
        /* +h010E */ BYTE  h010E[14];
        /* +h011C */ float Energy; // Only works for yourself
        /* +h0120 */ DWORD MaxEnergy; // Only works for yourself
        /* +h0124 */ BYTE  h0124[4];
        /* +h0128 */ float HPPips; // Regen/degen as float
        /* +h012C */ BYTE  h012C[4];
        /* +h0130 */ float HP; // Health in % where 1=100% and 0=0%
        /* +h0134 */ DWORD MaxHP; // Only works for yourself
        /* +h0138 */ DWORD Effects; // Bitmap for effects to display when targetted. DOES include hexes
        /* +h013C */ BYTE  h013C[4];
        /* +h0140 */ BYTE  Hex; // Bitmap for the hex effect when targetted (apparently obsolete!) (yes)
        /* +h0141 */ BYTE  h0141[19];
        /* +h0154 */ DWORD ModelState; // Different values for different states of the model.
        /* +h0158 */ DWORD TypeMap; // Odd variable! 0x08 = dead, 0xC00 = boss, 0x40000 = spirit, 0x400000 = player
        /* +h015C */ BYTE  h015C[16];
        /* +h016C */ DWORD InSpiritRange; // Tells if agent is within spirit range of you. Doesn't work anymore?
        /* +h0170 */ BYTE  h0170[16];
        /* +h0180 */ DWORD LoginNumber; // Unique number in instance that only works for players
        /* +h0184 */ float AnimationSpeed;  // Speed of the current animation
        /* +h0188 */ BYTE  AnimationUnk[4]; // related to animations
        /* +h018C */ DWORD AnimationID;     // Id of the current animation
        /* +h0190 */ BYTE  h0190[32];
        /* +h01B0 */ BYTE  DaggerStatus; // 0x1 = used lead attack, 0x2 = used offhand attack, 0x3 = used dual attack
        /* +h01B1 */ BYTE  Allegiance; // 0x1 = ally/non-attackable, 0x2 = neutral, 0x3 = enemy, 0x4 = spirit/pet, 0x5 = minion, 0x6 = npc/minipet
        /* +h01B2 */ WORD  WeaponType; // 1=bow, 2=axe, 3=hammer, 4=daggers, 5=scythe, 6=spear, 7=sWORD, 10=wand, 12=staff, 14=staff
        /* +h01B4 */ WORD  Skill; // 0 = not using a skill. Anything else is the Id of that skill
        /* +h01B6 */ WORD  WeaponItemType;
        /* +h01B8 */ WORD  OffhandItemType;
        /* +h01BA */ WORD  WeaponItemId;
        /* +h01BC */ WORD  OffhandItemId;

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

        // Agent Type Bitmasks.
        inline bool GetIsCharacterType()    { return (Type & 0xDB)  != 0; }
        inline bool GetIsGadgetType()       { return (Type & 0x200) != 0; }
        inline bool GetIsItemType()         { return (Type & 0x400) != 0; }

        // Agent TypeMap Bitmasks.
        inline bool GetInCombatStance()     { return (TypeMap & 0x000001) != 0; }
        inline bool GetHasQuest()           { return (TypeMap & 0x000002) != 0; } // if agent has quest marker
        inline bool GetIsDeadByTypeMap()    { return (TypeMap & 0x000008) != 0; }
        inline bool GetIsFemale()           { return (TypeMap & 0x000200) != 0; }
        inline bool GetHasBossGlow()        { return (TypeMap & 0x000400) != 0; }
        inline bool GetIsHidingCape()       { return (TypeMap & 0x001000) != 0; }
        inline bool GetCanBeViewedInPartyWindow() { return (TypeMap & 0x20000) != 0; }
        inline bool GetIsSpawned()          { return (TypeMap & 0x040000) != 0; }
        inline bool GetIsBeingObserved()    { return (TypeMap & 0x400000) != 0; }

        // Modelstates.
        inline bool GetIsKnockedDown()      { return ModelState == 1104; }
        inline bool GetIsMoving()           { return ModelState == 12 || ModelState == 76   || ModelState == 204; }
        inline bool GetIsAttacking()        { return ModelState == 96 || ModelState == 1088 || ModelState == 1120; }

        inline bool IsPlayer()              { return LoginNumber != 0; }
        inline bool IsNPC()                 { return LoginNumber == 0; }
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
        /* +h0000 */ BYTE   h0000[12];
        /* +h000C */ DWORD  AgentId;
        /* +h0010 */ BYTE   h0010[40];
        /* +h0038 */ DWORD  Moving1; //tells if you are stuck even if your client doesn't know
        /* +h003C */ BYTE   h003C[8];
        /* +h0044 */ DWORD  Moving2; //exactly same as Moving1
        /* +h0048 */ BYTE   h0048[28];
        /* +h0064 */ Vec3f  h0064;
        /* +h0070 */ BYTE   h0070[4];
        /* +h0074 */ Vec3f  h0074;
    };

    struct AgentInfo {
        BYTE h0000[52];
        wchar_t *NameString;
    };

    using AgentArray = Array<Agent*>;
    using MapAgentArray = Array<MapAgent>;
    using AgentMovementArray = Array<AgentMovement*>;
    using AgentInfoArray = Array<AgentInfo>;
}

#endif // _ENTITIE_AGENT_INC
