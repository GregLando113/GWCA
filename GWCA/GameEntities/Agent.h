#pragma once

#include <GWCA\GameContainers\gw_array.h>

#include "Position.h"

namespace GW {
	using AgentID = DWORD;

	struct Agent {
		DWORD* vtable;
		BYTE unknown1[24];
		BYTE unknown2[4]; //This actually points to the agent before but with a small offset
		Agent* NextAgent; //Pointer to the next agent (by id)
		BYTE unknown3[8];
		AgentID Id; //AgentId
		float Z; //Z coord in float
		float Width1;	//Width of the model's box
		float Height1;	//Height of the model's box
		float Width2;	//Width of the model's box (same as 1)
		float Height2;	//Height of the model's box (same as 1)
		float Width3;	//Width of the model's box (usually same as 1)
		float Height3;	//Height of the model's box (usually same as 1)
		float Rotation_angle; //Rotation in radians from East (-pi to pi)
		float Rotation_cos; // cosine of rotation
		float Rotation_sin; // sine of rotation
		long NameProperties; //Bitmap basically telling what the agent is
		BYTE unknown4[8];
		float unkfloat1;	// weird values, change with movement, always between -1 and 1
		float unkfloat2;
		float unkfloat3;
		BYTE unknown5[4];
		union {
			struct {
				float X; //X coord in float
				float Y; //Y coord in float
				DWORD Ground;
			};
			struct {
				GamePos pos;
			};
		};
		BYTE unknown6[4];
		float NameTagX; //Exactly the same as X above
		float NameTagY; //Exactly the same as Y above
		float NameTagZ; //Z coord in float
		BYTE unknown7[12];
		long Type; //0xDB = players, npc's, monsters etc. 0x200 = signpost/chest/object (unclickable). 0x400 = item to pick up
		float MoveX; //If moving, how much on the X axis per second
		float MoveY; //If moving, how much on the Y axis per second
		BYTE unknown8[4]; // always 0?
		float Rotation_cos2; // same as cosine above
		float Rotation_sin2; // same as sine above
		BYTE unknown10[16];
		long Owner;
		DWORD itemid; // Only valid if agent is type 0x400 (item)
		BYTE unknown24[4];
		long ExtraType;
		BYTE unknown11[24];
		float WeaponAttackSpeed; //The base attack speed in float of last attacks weapon. 1.33 = axe, sWORD, daggers etc.
		float AttackSpeedModifier; //Attack speed modifier of the last attack. 0.67 = 33% increase (1-.33)
		WORD PlayerNumber; //Selfexplanatory. All non-players have identifiers for their type. Two of the same mob = same number
		BYTE unknown12[6];
		DWORD** Equip;
		BYTE unknown13[10];
		BYTE Primary; //Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
		BYTE Secondary; //Secondary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
		BYTE Level; //Duh!
		BYTE TeamId; //0=None, 1=Blue, 2=Red, 3=Yellow
		BYTE unknown14[14];
		float Energy; //Only works for yourself
		long MaxEnergy; //Only works for yourself
		BYTE unknown15[4];
		float HPPips; //Regen/degen as float
		BYTE unknown16[4];
		float HP; //Health in % where 1=100% and 0=0%
		long MaxHP; //Only works for yourself
		long Effects; //Bitmap for effects to display when targetted. DOES include hexes
		BYTE unknown17[4];
		BYTE Hex; //Bitmap for the hex effect when targetted (apparently obsolete!) (yes)
		BYTE unknown18[18];
		long ModelState; //Different values for different states of the model.
		long TypeMap; //Odd variable! 0x08 = dead, 0xC00 = boss, 0x40000 = spirit, 0x400000 = player
		BYTE unknown19[16];
		long InSpiritRange; //Tells if agent is within spirit range of you. Doesn't work anymore?
		BYTE unknown20[16];
		DWORD LoginNumber; //Unique number in instance that only works for players
		float AnimationSpeed;	// Speed of the current animation
		BYTE AnimationUnk[4];	// related to animations
		long AnimationID;		// Id of the current animation
		BYTE unknown22[32];
		BYTE DaggerStatus; // 0x1 = used lead attack, 0x2 = used offhand attack, 0x3 = used dual attack
		BYTE Allegiance;  // 0x1 = ally/non-attackable, 0x2 = neutral, 0x3 = enemy, 0x4 = spirit/pet, 0x5 = minion, 0x6 = npc/minipet
		WORD WeaponType; //1=bow, 2=axe, 3=hammer, 4=daggers, 5=scythe, 6=spear, 7=sWORD, 10=wand, 12=staff, 14=staff
		WORD Skill;		//0 = not using a skill. Anything else is the Id of that skill
		WORD WeaponItemType;
		WORD OffhandItemType;
		WORD WeaponItemId;
		WORD OffhandItemId;

		// Health Bar Effect Bitmasks.
		inline bool GetIsBleeding() { return (Effects & 1) != 0; }
		inline bool GetIsConditioned() { return (Effects & 2) != 0; }
		inline bool GetIsDead() { return (Effects & 16) != 0; }
		inline bool GetIsDeepWounded() { return (Effects & 32) != 0; }
		inline bool GetIsPoisoned() { return (Effects & 64) != 0; }
		inline bool GetIsEnchanted() { return (Effects & 128) != 0; }
		inline bool GetIsDegenHexed() { return (Effects & 1024) != 0; }
		inline bool GetIsHexed() { return (Effects & 2048) != 0; }
		inline bool GetIsWeaponSpelled() { return (Effects & 32768) != 0; }

		// Agent Type Bitmasks.
		inline bool GetIsLivingType() { return (Type & 0xDB) != 0; }
		inline bool GetIsSignpostType() { return (Type & 0x200) != 0; }
		inline bool GetIsItemType() { return (Type & 0x400) != 0; }

		// Agent TypeMap Bitmasks.
		inline bool GetInCombatStance() { return (TypeMap & 0x1) != 0; }
		inline bool GetHasQuest() { return (TypeMap & 0x2) != 0; } // if agent has quest marker
		inline bool GetIsDeadByTypeMap() { return (TypeMap & 0x8) != 0; }
		inline bool GetIsFemale() { return (TypeMap & 0x200) != 0; }
		inline bool GetHasBossGlow() { return (TypeMap & 0x400) != 0; }
		inline bool GetIsHidingCape() { return (TypeMap & 0x1000) != 0; }
		inline bool GetCanBeViewedInPartyWindow() { return (TypeMap & 0x20000) != 0; }
		inline bool GetIsSpawned() { return (TypeMap & 0x40000) != 0; }
		inline bool GetIsBeingObserved() { return (TypeMap & 0x400000) != 0; }

		// Modelstates.
		inline bool GetIsKnockedDown() { return ModelState == 1104; }
		inline bool GetIsMoving() { return ModelState == 12 || ModelState == 76 || ModelState == 204; }
		inline bool GetIsAttacking() { return ModelState == 96 || ModelState == 1088 || ModelState == 1120; }

		inline bool IsPlayer() { return LoginNumber != 0; }
		inline bool IsNPC() { return LoginNumber == 0; }
	};

	struct MapAgent {
		float curenergy; //?
		float maxenergy; //?
		float energyregen;
		DWORD skilltimestamp; //?
		float unk2;
		float MaxEnergy2; // again?
		float unk3;
		DWORD unk4;
		float curHealth;
		float maxHealth;
		float healthregen;
		DWORD unk5;
		DWORD Effects;

		// Health Bar Effect Bitmasks.
		inline bool GetIsBleeding() { return (Effects & 1) != 0; }
		inline bool GetIsConditioned() { return (Effects & 2) != 0; }
		inline bool GetIsDead() { return (Effects & 16) != 0; }
		inline bool GetIsDeepWounded() { return (Effects & 32) != 0; }
		inline bool GetIsPoisoned() { return (Effects & 64) != 0; }
		inline bool GetIsEnchanted() { return (Effects & 128) != 0; }
		inline bool GetIsDegenHexed() { return (Effects & 1024) != 0; }
		inline bool GetIsHexed() { return (Effects & 2048) != 0; }
		inline bool GetIsWeaponSpelled() { return (Effects & 32768) != 0; }
	};

	struct AgentMovement {
		BYTE unknown1[12];
		long AgentId;
		BYTE unknown2[40];
		long Moving1; //tells if you are stuck even if your client doesn't know
		BYTE unknown3[8];
		long Moving2; //exactly same as Moving1
		BYTE unknown4[28];
		float X2;
		float Y2;
		byte unknown5[8];
		float X;
		float Y;
	};

	using AgentArray = gw_array<Agent*>;
	using MapAgentArray = gw_array<MapAgent>;
	using AgentMovementArray = gw_array<AgentMovement*>;
}
