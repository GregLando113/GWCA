#pragma once

#include <Windows.h>
#include <assert.h>

#include "MemoryMgr.h"

#include "GwConstants.h"
#include "GWStructPositions.h"

namespace GWCA {

	namespace GW {
		template <typename T>
		class gw_array {
		protected:
			T* array_;
			DWORD allocated_size_;
			DWORD current_size_;
			DWORD default_size_;
		public:
			typedef T* iterator;
			typedef const T* const_iterator;

			iterator begin() { return array_; }
			const_iterator begin() const { return array_; }
			iterator end() { return array_ + current_size_; }
			const_iterator end() const { return array_ + current_size_; }

			T& index(DWORD _index) {
				assert(_index >= 0 && _index < current_size_);
				return array_[_index];
			}

			T& operator[](DWORD _index) {
				assert(_index >= 0 && _index < current_size_);
				return array_[_index];
			}
			bool valid() {
				return array_ != NULL;
			}

			DWORD size() const { return current_size_; }
			DWORD size_allocated() const { return allocated_size_; }
		};


		using AgentID	= DWORD;
		using ItemID	= DWORD;
		using PlayerID	= DWORD;

		struct Agent {
			DWORD* vtable;
			BYTE unknown1[24];
			BYTE unknown2[4]; //This actually points to the agent before but with a small offset
			Agent* NextAgent; //Pointer to the next agent (by id)
			BYTE unknown3[8];
			AgentID Id; //AgentId
			float Z; //Z coord in float
			BYTE unknown4[8];
			float BoxHoverWidth; //Width of the model's box
			float BoxHoverHeight; //Height of the model's box
			BYTE unknown5[8];
			float Rotation; //Rotation in radians from East (-pi to pi)
			BYTE unknown6[8];
			long NameProperties; //Bitmap basically telling what the agent is
			BYTE unknown7[24];
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
			BYTE unknown8[4];
			float NameTagX; //Exactly the same as X above
			float NameTagY; //Exactly the same aswell
			float NameTagZ; //Z coord in float (actually negated)
			BYTE unknown9[12];
			long Type; //0xDB = players, npc's, monsters etc. 0x200 = signpost/chest/object (unclickable). 0x400 = item to pick up
			float MoveX; //If moving, how much on the X axis per second
			float MoveY; //If moving, how much on the Y axis per second
			//BYTE unknown10[68];
			BYTE unknown10[28];
			long Owner;
			ItemID itemid; // Only valid if agent is type 0x400 (item)
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
			//Offset +0x130
			float HP; //Health in % where 1=100% and 0=0%
			long MaxHP; //Only works for yourself
			long Effects; //Bitmap for effects to display when targetted. DOES include hexes
			BYTE unknown17[4];
			BYTE Hex; //Bitmap for the hex effect when targetted (apparently obsolete!)
			BYTE unknown18[18];
			long ModelState; //Different values for different states of the model.
			long TypeMap; //Odd variable! 0x08 = dead, 0xC00 = boss, 0x40000 = spirit, 0x400000 = player
			BYTE unknown19[16];
			long InSpiritRange; //Tells if agent is within spirit range of you. Doesn't work anymore?
			BYTE unknown20[16];
			PlayerID LoginNumber; //Unique number in instance that only works for players
			float ModelMode; //Float for the current mode the agent is in. Varies a lot
			BYTE unknown21[4];
			long ModelAnimation; //Id of the current animation
			BYTE unknown22[32];
			BYTE DaggerStatus; // 0x1 = used lead attack, 0x2 = used offhand attack, 0x3 = used dual attack
			BYTE Allegiance;  // 0x1 = ally/non-attackable, 0x2 = neutral, 0x3 = enemy, 0x4 = spirit/pet, 0x5 = minion, 0x6 = npc/minipet
			WORD WeaponType; //1=bow, 2=axe, 3=hammer, 4=daggers, 5=scythe, 6=spear, 7=sWORD, 10=wand, 12=staff, 14=staff
			//Offset +0x1B4
			WORD Skill; //0 = not using a skill. Anything else is the Id of that skill
			BYTE unknown23[4];
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
			inline bool GetInCombatStance() { return (TypeMap & 1) != 0; }
			inline bool GetHasQuest() { return (TypeMap & 2) != 0; }
			inline bool GetIsDeadByTypeMap() { return (TypeMap & 8) != 0; }
			inline bool GetIsFemale() { return (TypeMap & 512) != 0; }
			inline bool GetHasBossGlow() { return (TypeMap & 1024) != 0; }
			inline bool GetIsHidingCape() { return (TypeMap & 4096) != 0; }
			inline bool GetCanBeViewedInPartyWindow() { return (TypeMap & 131072) != 0; }
			inline bool GetIsSpawned() { return (TypeMap & 262144) != 0; }
			inline bool GetIsBeingObserved() { return (TypeMap & 4194304) != 0; }

			// Modelstates.
			inline bool GetIsKnockedDown() { return ModelState == 1104; }
			inline bool GetIsMoving() { return ModelState == 12 || ModelState == 76 || ModelState == 204; }
			inline bool GetIsAttacking() { return ModelState == 96 || ModelState == 1088 || ModelState == 1120; }
		};

		struct MapAgent{
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
			BYTE* vtable;
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

		struct PlayerPartyMember {
			PlayerID loginnumber;
			DWORD unk1;
			DWORD state;

			inline bool connected() { return (state & 1) > 0; }
			inline bool ticked() { return (state & 2) > 0; }
		};

		struct HeroPartyMember {
			AgentID id;
			PlayerID ownerplayerid;
			GwConstants::HeroID heroid;
			DWORD unk1;
			DWORD unk2;
			DWORD level;
		};

		struct HenchmanPartyMember {
			AgentID id;
			DWORD unk[11];
			DWORD profession;
			DWORD level;
		};

		using PlayerPartyMemberArray = gw_array<PlayerPartyMember>;
		using HeroPartyMemberArray = gw_array<HeroPartyMember>;
		using HenchmanPartyMemberArray = gw_array<HenchmanPartyMember>;

		class PartyInfo
		{
		public:
			char pad_0x0000[0x4]; //0x0000
			gw_array<PlayerPartyMember> players; //0x0004 
			gw_array<HenchmanPartyMember> henchmen; //0x0014 
			gw_array<HeroPartyMember> heroes; //0x0024 
			gw_array<void*> unk1_arr; //0x0034 
		};

		struct Player {
			DWORD AgentID; //0x0000 
			char pad_0x0004[0x14]; //0x0004
			DWORD Primary; //0x0018 
			DWORD Secondary; //0x001C 
			char pad_0x0020[0x4]; //0x0020
			struct NameMod {
				char pad_0x0000[0x4]; //0x0000
				wchar_t Name[20]; //0x0004 
			} *Name1; //Size=0x002C
			wchar_t* Name; //0x0028 
			char pad_0x002C[0x4]; //0x002C
			DWORD ActiveTitle; //0x0030 
			char pad_0x0034[0x4]; //0x0034
			DWORD PartySize; //0x0038 
			char pad_0x003C[0x10]; //0x003C
		};//Size=0x004C

		using PlayerArray = gw_array<Player>;
		using MapAgentArray = gw_array<MapAgent>;

		class AgentArray : public gw_array < Agent* > {
		public:
			Agent* GetPlayer() { return index(GetPlayerId()); }
			Agent* GetTarget() { return index(GetTargetId()); }
			inline DWORD GetPlayerId() { return *(DWORD*)MemoryMgr::PlayerAgentIDPtr; }
			inline DWORD GetTargetId() { return *(DWORD*)MemoryMgr::TargetAgentIDPtr; }
			Agent* operator[] (DWORD index) {
				if (!valid()) return nullptr;
				if (index < 0) return nullptr;
				if (index >= current_size_) return nullptr;
				return array_[index];
			}
		};


		struct Bag;
		struct Item;

		using ItemArray = gw_array<Item*>;

		struct Bag{							// total : 24 BYTEs
			BYTE unknown1[4];					// 0000	|--4 BYTEs--|
			long index;							// 0004
			DWORD BagId;						// 0008
			DWORD ContainerItem;				// 000C
			DWORD ItemsCount;					// 0010
			Bag* BagArray;						// 0014
			ItemArray Items;						// 0020
		};

		class ItemModifier {
		public:

			DWORD identifier() { return (mod_ & 0x3FF00000) >> 20; }
			DWORD arg1() { return (mod_ & 0x0001FF00) >> 8; }
			DWORD arg2() { return (mod_ & 0x000000FE); }
			DWORD arg3() { return (mod_ & 0x0003FFFF); }
			DWORD arg4() { return (mod_ & 0x00040000) >> 17; }
			DWORD arg5() { return (mod_ & 0x0001FFFE); }
			DWORD arg6() { return (mod_ & 0x00000001); }

		private:
			DWORD mod_;
		};

		struct Item{							// total : 50 BYTEs
			DWORD ItemId;						// 0000
			DWORD AgentId;						// 0004
			BYTE unknown1[4];					// 0008	|--4 BYTEs--|
			Bag* bag;							// 000C
			ItemModifier* ModStruct;						// 0010						pointer to an array of mods
			DWORD ModStructSize;				// 0014						size of this array
			wchar_t* Customized;				// 0018
			BYTE unknown3[3];
			short type;
			short extraId;
			short value;
			BYTE unknown4[4];
			short interaction;
			long ModelId;
			BYTE* modString;
			BYTE unknown5[4];
			BYTE* extraItemInfo;
			BYTE unknown6[15];
			BYTE Quantity;
			BYTE equipped;
			BYTE profession;
			BYTE slot;						// 004F
		};

		struct Inventory {
			Bag* bags[0x1F];
			DWORD gold_character;
			DWORD gold_storage;
		};


		struct Skill{							// total : A0 BYTEs
			DWORD SkillId;						// 0000
			BYTE Unknown1[4];					// 0004
			long Campaign;						// 0008	
			long Type;							// 000C
			DWORD Special;
			long ComboReq;						// 0014
			DWORD Effect1;
			DWORD Condition;
			DWORD Effect2;
			DWORD WeaponReq;
			BYTE Profession;					// 0028
			BYTE Attribute;						// 0029
			BYTE Unknown2[2];					// 002A
			long SkillId_PvP;					// 002C
			BYTE Combo;							// 0030
			BYTE Target;						// 0031
			BYTE unknown3;						// 0032
			BYTE SkillEquipType;				// 0033
			BYTE Unknown4;						// 0034
			BYTE EnergyCost;
			BYTE HealthCost;
			BYTE Unknown7;
			DWORD Adrenaline;					// 0038
			float Activation;					// 003C
			float Aftercast;					// 0040
			long Duration0;						// 0044
			long Duration15;					// 0048
			long Recharge;						// 004C
			BYTE Unknown5[12];					// 0050
			long Scale0;						// 005C
			long Scale15;						// 0060
			long BonusScale0;					// 0064
			long BonusScale15;					// 0068
			float AoERange;						// 006C
			float ConstEffect;					// 0070
			BYTE unknown6[44];					// 0074

			BYTE GetEnergyCost()
			{
				switch (EnergyCost){
				case 11: return 15;
				case 12: return 25;
				default: return EnergyCost;
				}
			}
		};

		struct SkillbarSkill {
			long AdrenalineA;				// 0000					
			long AdrenalineB;				// 0004					
			DWORD Recharge;					// 0008					
			DWORD SkillId;					// 000C						see GWConst::SkillIds
			DWORD Event;					// 0010	s
			long GetRecharge() const {		// returns recharge time remaining in milliseconds, or 0 if recharged
				if (Recharge == 0) return 0;
				return Recharge - MemoryMgr::GetSkillTimer();
			}
		};
		struct Skillbar {						// total : BC BYTEs
			Skillbar() : AgentId(0) {}
			DWORD AgentId;						// 0000						id of the agent whose skillbar this is
			SkillbarSkill Skills[8];			// 0004
			DWORD Disabled;
			BYTE unknown1[8];					// 00A8	|--8 BYTEs--|
			DWORD Casting;						// 00B0
			BYTE unknown2[8];					// 00B4	|--8 BYTEs--|
			static Skillbar Nil() { return Skillbar(); }
			bool IsValid() const { return AgentId > 0; }
		};

		typedef gw_array<Skillbar> SkillbarArray;

		struct Effect {							// total : 18 BYTEs
			DWORD SkillId;						// 0000						skill id of the effect
			long EffectType;					// 0004						type classifier 0 = condition/shout, 8 = stance, 11 = maintained enchantment, 14 = enchantment/nature ritual
			DWORD EffectId;						// 0008						unique identifier of effect
			DWORD AgentId;						// 000C						non-zero means maintained enchantment - caster id
			float Duration;						// 0010						non-zero if effect has a duration
			DWORD TimeStamp;					// 0014						GW-timestamp of when effect was applied - only with duration

			long GetTimeElapsed() const { return MemoryMgr::GetSkillTimer() - TimeStamp; }
			long GetTimeRemaining() const { return (long)(Duration * 1000) - GetTimeElapsed(); }
			static Effect Nil() { return Effect{ 0, 0, 0, 0, 0, 0 }; }
		};

		struct Buff {							// total : 10 bytes
			DWORD SkillId;						// 0000						skill id of the buff
			DWORD Unknown1;						// 0004
			DWORD BuffId;						// 0008						id of buff in the buff array
			DWORD TargetAgentId;				// 000C						agent id of the target (0 if no target)
			static Buff Nil() { return Buff{ 0, 0, 0, 0 }; }
		};

		using EffectArray = gw_array<Effect>;
		using BuffArray = gw_array<Buff>;


		struct AgentEffects {
			DWORD AgentId;
			BuffArray Buffs;
			EffectArray Effects;
		};

		using AgentEffectsArray = gw_array<AgentEffects>;


		struct GHKey { DWORD k[4]; };

		struct GuildPlayer {
			void* vtable;
			wchar_t* name_ptr; // ptr to invitedname, why? dunno
			wchar_t invitedname[20]; // name of character that was invited in
			wchar_t currentname[20]; // name of character currently being played
			wchar_t invitorname[20]; // name of character that invited player
			DWORD invitetime; // time in ms from game creation ??
			wchar_t promotername[20]; // name of player that last modified rank
		};

		using GuildRoster = gw_array<GuildPlayer*>;

		struct GuildHistoryEvent {
			DWORD time1; // Guessing one of these is time in ms
			DWORD time2;
			wchar_t name[0x100]; // Name of added/kicked person, then the adder/kicker, they seem to be in the same array
		};

		using GuildHistory = gw_array<GuildHistoryEvent*>;

		struct Guild {
			GHKey key;

			inline wchar_t* name() const { return (wchar_t*)(this + 0x30); }
			inline wchar_t* tag() const { return (wchar_t*)(this + 0x80); }

			inline DWORD& factioncount() const { return *(DWORD*)(this + 0x78); }

			inline DWORD& rating() const { return *(DWORD*)(this + 0x70); }
			inline DWORD& rank() const { return *(DWORD*)(this + 0x28); }
			inline DWORD& qualifierpoints() const { return *(DWORD*)(this + 0x7C); }
		};

		using GuildArray = gw_array<Guild*>;

		struct MissionMapIcon { // MapOverlay from GWCA
			long index;
			float X;
			float Y;
			long unknown1; // = 0
			long unknown2; // = 0
			long option; // Affilitation/color. Enum { 0 = gray, blue, red, yellow, teal, purple, green, gray };
			long unknown3; // = 0
			long modelId; // Model of the displayed icon in the Minimap
			long unknown4; // = 0
			void* unknown5; // May concern the name
		};

		using MissionMapIconArray = gw_array<MissionMapIcon>;

		using MerchItemArray = gw_array<ItemID>;

		struct Friend {
			DWORD type;				// 0 = Friend, 1 = Ignore, 2 = Played, 3 = Trade
			DWORD status;			// 0 = Offline, 1 = Online, 2 = Do not disturb, 3 = Away
			WCHAR account[20];
			WCHAR name[20];
			DWORD index;			// Order by adding time
			DWORD zoneId;
		};

		typedef gw_array<Friend*> FriendsListArray;

		struct FriendList {
			FriendsListArray friends;
			BYTE unknow[20];
			DWORD nFriend;
			DWORD nIgnore;
			DWORD nPlayed;
			DWORD nTrade;
			BYTE unknow2[72];
			DWORD myStatus;

			inline DWORD size() { return (nFriend + nIgnore + nPlayed + nTrade); };
		};

		struct Camera {
			DWORD lookAtAgentID;
			DWORD unk1;
			float unk2[2];
			float maxdistance; // Max distance camera can go, usually constant @ 750 unless max zoom changed
			float unk3;
			float yaw; // left/right camera angle, radians w/ origin @ east
			float pitch; // up/down camera angle, range of [-1,1]
			float distance; // current distance from players head.
			DWORD unk4[4];
			float yaw_rightclick; // Only changes when you move camera using right click, keyboard movement does not effect.
			float yaw_rightclick2; // ^
			float pitch_rightclick; // ^
			float distance2;
			float accelerationconstant; // According to http://www.gamerevision.com/showthread.php?217-Guild-Wars-WorldToSreen-by-Cronos&p=20003&viewfull=1#post20003
			float timesincelastkeyboardrotation; // In seconds it seems.
			float timesincelastmouserotation;
			float timesincelastmousemove;
			float timesincelastagentselection;
			float timeinthemap;
			float timeinthedistrict;
			float yaw_togo;
			float pitch_togo;
			float dist_togo;
			float maxdistance2;
			float unk5[2];
			Vector3f camerapos;
			Vector3f camerapos_togo;
			Vector3f campos_inverted;
			Vector3f campos_inverted_togo;
			Vector3f LookAtTarget;
			Vector3f LookAt_togo;
			float fieldofview;
			float fieldofview2;
			// ...
		};

		struct PathingTrapezoid {
			DWORD ID;
			PathingTrapezoid* Adjacent[4];
			DWORD idekwhyisthisherebutok;
			float XTL;
			float XTR;
			float YT;
			float XBL;
			float XBR;
			float YB;
		};

		struct PathingMap {
			DWORD zplane; // ground plane = UINT_MAX, rest 0 based index
			DWORD unkafterzplane;
			void* unk1ptr;
			DWORD unk1size;
			void* unk2ptr;
			DWORD unk2size;
			PathingTrapezoid* trapezoids;
			DWORD trapezoidcount;
			DWORD restunknown[0xD];
		};

		using PathingMapArray = gw_array<PathingMap>;


		struct Attribute {
			DWORD ID; // ID of attribute
			DWORD level_base; // Level of attribute without modifiers (runes,pcons,etc)
			DWORD level; // Level with modifiers
			DWORD decrement_points; // Points that you will recieve back if you decrement level.
			DWORD increment_points; // Points you will need to increment level.
		};

		struct PartyAttribute {
			DWORD AgentID;
			Attribute Attribute[49];
		};

		using PartyAttributeArray = gw_array<PartyAttribute>;


		struct Quest {
			DWORD questid;
			int logstate;
			void* unk1[3];
			DWORD mapfrom;
			GamePos marker;
			DWORD unk2;
			DWORD mapto;
			void* unk3[2];
		};

		using QuestLog = gw_array<Quest>;

		struct HeroFlag {
			GwConstants::HeroID hero;
			AgentID heroid;
			DWORD unk1;
			GwConstants::HeroState state;
			GamePos flag;
			DWORD unk2;
			AgentID lockedtargetid;
		};

		using HeroFlagArray = gw_array<HeroFlag>;

		struct Title {
			DWORD unk1;
			DWORD currentpoints;
			DWORD unk2;
			DWORD pointsneeded_currentrank;
			DWORD unk3;
			DWORD pointsneeded_nextrank;
			DWORD maxtitlerank;
			DWORD unk4;
			void* unk5[2]; // Pretty sure these are ptrs to title hash strings
		}; // size = 0x28

		using TitleArray = gw_array<Title>;

	}
}