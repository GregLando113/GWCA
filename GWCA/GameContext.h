#pragma once

#include "GwConstants.h"
#include "GWStructures.h"
#include "PatternScanner.h"

namespace GWCA {

	using namespace GW;

	class AgentContext
	{
		AgentContext(){}
		AgentContext(const AgentContext&){}
	public:
		gw_array<void*> unk1_array;
		DWORD pad1[5];
		void* function1;
		DWORD pad2[2];
		void* function2;
		DWORD pad3[2];
		void* function3;
		DWORD pad4[2];
		void* function4;
		DWORD pad5[2];
		void* function5;
		DWORD pad6[11];
		gw_array<void*> unk2_array;
		DWORD unk1; // was 1 for me
		gw_array<void*> unk3_array;
		gw_array<void*> unk4_array;
		gw_array<void*> unk5_array;
		DWORD rand1; // Number seems to be randomized quite a bit o.o seems to be accessed by textparser.cpp
		DWORD rand2;
		BYTE pad7[0x18];
		gw_array<AgentMovement*> agentmovement;
		gw_array<void*> unk6_array;
		BYTE pad8[0x44];
		gw_array<void*> unk7_array;
		gw_array<void*> unk8_array;
		BYTE pad9[0x40];
		DWORD instancetimer;
		//... more but meh
	};

	class MapContext
	{
		MapContext(){}
		MapContext(const MapContext&){}
	public:
		float mapboundaries[5];
		BYTE pad1[0x18];
		gw_array<void*> spawns1; // Seem to be arena spawns. struct is X,Y,unk 4 byte value,unk 4 byte value.
		gw_array<void*> spawns2; // Same as above
		gw_array<void*> spawns3; // Same as above
		float unk1[6]; // Some trapezoid i think.
		struct sub1 {
			struct sub2 {
				BYTE pad1[0x18];
				PathingMapArray pmaps;
			} *sub2;
			//... Bunch of arrays and shit
		} *sub1;
		//... Player coords and shit beyond this point if they are desirable :p
	};

	class WorldContext 
	{
	public:
		WorldContext(){}
		WorldContext(const WorldContext&){}
	 
		struct sub1 {
			wchar_t* name;
			//...
		} *sub1; // 0x0
		struct sub2 {
			DWORD unk1;
			wchar_t lastmessage[0x9C]; // Last chat message recieved via gameserver
		} *chat; // 0x4
		BYTE pad1[0x1C]; // 0x8
		MerchItemArray merchitems; // 0x24
		BYTE pad1_1[0x48]; // 0x34
		MapAgentArray mapagents; // 0x7C
		BYTE pad2[0x10]; // 0x8C
		GamePos all_flag; // 0x9C
		BYTE pad3[0x4]; // 0xA8
		PartyAttributeArray attributes; // 0xAC
		BYTE pad4[0x3FC]; // 0xBC
		gw_array<void*> unk1_array; // 0x4B8
		gw_array<void*> unk2_array; // 0x4C8
		BYTE pad5[0x4]; // 0x4D8
		gw_array<void*> unk3_array; // 0x4DC
		BYTE pad6[0x1C]; // 0x4EC
		AgentEffectsArray partyeffects; // 0x508
		gw_array<void*> unk4_array; // 0x518
		DWORD unk1; // 0x528
		QuestLog questlog; // 0x52C
		BYTE pad7[0x38]; // 0x53C
		gw_array<DWORD> unk5_array; // 0x574 - Seems like agent/item ids (only appears in outpost?)
		HeroFlagArray hero_flags; // 0x584
		gw_array<void*> unk6_array; // 0x594
		gw_array<void*> unk7_array; // 0x5A4 - Struct size = 0x20
		DWORD unk2[2]; // 0x5B4
		gw_array<void*> unk8_array; // 0x5BC
		gw_array<void*> unk9_array; // 0x5CC
		gw_array<void*> unk10_array; // 0x5DC
		gw_array<void*> unk11_array; // 0x5EC
		gw_array<void*> unk12_array; // 0x5FC
		gw_array<void*> unk13_array; // 0x60C
		DWORD unk3[4]; // 0x61C
		gw_array<void*> unk14_array; // 0x62C
		BYTE pad8[0x44]; // 0x63C
		gw_array<void*> unk15_array; // 0x680 - Struct size = 0x138 i think, has a vtable
		DWORD salvagesessionid; // 0x690
		DWORD unk4[0xA]; // 0x694
		gw_array<void*> unk16_array; // 0x6BC
		gw_array<void*> unk17_array; // 0x6CC
		DWORD unk5; // 0x6DC
		gw_array<void*> unk18_array; // 0x6E0
		SkillbarArray skillbar; // 0x6F0
		gw_array<void*> unk19_array; // 0x700
		gw_array<void*> unk20_array; // 0x710
		gw_array<void*> unk21_array; // 0x720
		gw_array<void*> unk22_array; // 0x730
		DWORD experience; // 0x740
		DWORD experience_dupe; // 0x744
		DWORD currentkurzick; // 0x748
		DWORD currentkurzick_dupe; // 0x74C
		DWORD totalearnedkurzick; // 0x750
		DWORD totalearnedkurzick_dupe; // 0x754
		DWORD currentluxon; // 0x758
		DWORD currentluxon_dupe; // 0x75C
		DWORD totalearnedluxon; // 0x760
		DWORD totalearnedluxon_dupe; // 0x764
		DWORD currentimperial; // 0x768
		DWORD currentimperial_dupe; // 0x76C
		DWORD totalearnedimperial; // 0x770
		DWORD totalearnedimperial_dupe; // 0x774
		DWORD unkfaction4; // 0x778
		DWORD unkfaction4_dupe; // 0x77C
		DWORD unkfaction5; // 0x780
		DWORD unkfaction5_dupe; // 0x784
		DWORD level; // 0x788
		DWORD level_dupe; // 0x78C
		DWORD unkfaction6; // 0x790
		DWORD unkfaction6_dupe; // 0x794
		DWORD currentbalth; // 0x798
		DWORD currentbalth_dupe; // 0x79C
		DWORD totalearnedbalth; // 0x7A0
		DWORD totalearnedbalth_dupe; // 0x7A4
		DWORD currentskillpoints; // 0x7A8
		DWORD currentskillpoints_dupe; // 0x7AC
		DWORD totalearnedskillpoints; // 0x7B0
		DWORD totalearnedskillpoints_dupe; // 0x7B4
		DWORD maxkurzick; // 0x7B8
		DWORD maxluxon; // 0x7BC
		DWORD maxbalth; // 0x7C0
		DWORD maximperial; // 0x7C4
		DWORD unk6; // 0x7C8
		gw_array<void*> unk23_array; // 0x7CC
		gw_array<void*> unk24_array; // 0x7DC
		MissionMapIconArray missionmapicons; // 0x7EC
		NPCArray npcs; // 0x7FC
		PlayerArray players; // 0x80C
		TitleArray titles; // 0x81C
		//... couple more arrays after this
	};

	class GuildContext
	{
	public:
	  void* unk1_ptr; // 0
	  DWORD unk1; // 4
	  void* unk2_ptr; // 8
	  void* unk3_ptr; // c
	  DWORD unk2; // 10
	  void* unk4_ptr; // 14
	  void* unk5_ptr; // 18
	  DWORD unk3; // 1c
	  gw_array<void*> unk1_arr; // 20 - wierd fucking graph/binary tree i think
	  DWORD unk4; // 30
	  wchar_t playername[20]; // 34
	  DWORD unk5; // 5c
	  DWORD playerguildindex; // 60
	  GHKey playerghkey; // 64
	  DWORD unk6; // 74
	  wchar_t guildannouncement[256]; // 78
	  wchar_t lastannouncementwriter[20]; // 278
	  DWORD unk7; // 2A0
	  void* unk6_ptr; // 2A4
	  gw_array<void*> unk2_arr; // 2A8
	  gw_array<void*> unk3_arr; // 2B8
	  DWORD unk8; //2C8
	  GuildHistory playerguildhistory; // 2CC
	  BYTE pad1[0x1C]; // 2DC
	  GuildArray guilds; // 2F8
	  DWORD unk9[4]; // 308
	  gw_array<void*> unk4_arr; // 318 - some binary tree pointing to guilds
	  DWORD unk10; // 328
	  gw_array<void*> unk5_arr; // 32C
	  BYTE pad2[0x1C]; // 33C
	  GuildRoster playerroster; // 358
		//... end of what i care about
	};

	class ItemContext
	{
	public:
		char pad_0x0000[0x10]; //0x0000
		gw_array<void*> unk1_arr; //0x0010 
		char pad_0x0020[0x4]; //0x0020
		gw_array<Bag*> bagarray; //0x0024 
		char pad_0x0034[0xC]; //0x0034
		gw_array<void*> unk3_arr; //0x0040 
		gw_array<void*> unk4_arr; //0x0050 
		char pad_0x0060[0x58]; //0x0060
		gw_array<Item*> itemarray; //0x00B8 
		char pad_0x00C8[0x30]; //0x00C8
		Inventory* inventory; //0x00F8 
		gw_array<void*> unk5_arr; //0x00FC 

	};//Size=0x010C

	class CharContext
	{
	public:
		gw_array<void*> unk1_arr; //0x0000 
		char pad_0x0010[0x4]; //0x0010
		gw_array<void*> unk2_arr; //0x0014 
		char pad_0x0024[0x10]; //0x0024
		gw_array<void*> unk3_arr; //0x0034 
		gw_array<void*> unk4_arr; //0x0044 
		char pad_0x0054[0x20]; //0x0054
		wchar_t playername[20]; //0x0074 
		char pad_0x009C[0x50]; //0x009C
		gw_array<void*> unk6_arr; //0x00EC 
		char pad_0x00FC[0x30]; //0x00FC
		DWORD mapidagain; //0x012C 
		char pad_0x0130[0x60]; //0x0130
		DWORD currentmapid_again; //0x0190 
		char pad_0x0194[0x28]; //0x0194
		DWORD currentmapid; //0x01BC 
		DWORD currentmapid_dupe; //0x01C0 
		char pad_0x01C4[0x8]; //0x01C4
		gw_array<void*> unk5_arr; //0x01CC 
		char pad_0x01DC[0x14C]; //0x01DC
		wchar_t playeremail[64]; //0x0328 

	};//Size=0x03A8

	

	class PartyContext
	{
	public:
		char pad_0x0000[0x4]; //0x0000
		gw_array<void*> unk1_arr; //0x0004 
		class PartyState { //0x0014
			DWORD state;
		public:
			bool InHardMode(){ return (state & 0x10) > 0; }
			bool IsDefeated(){ return (state & 0x20) > 0; }
		}partystate;
		char pad_0x0018[0x10]; //0x0018
		DWORD invitecount; // 0x0028
		char pad_0x002C[0x28]; // 0x002C
		PartyInfo* partyinfo; //0x0054 
	};//Size=0x0058

	class TradeContext
	{
		TradeContext(){}
		TradeContext(const TradeContext&){}

	public:
		enum State : DWORD { NO_TRADE, TRADE_INITIATED, OFFER_ACCEPTED = 3 };

		struct Item {
			ItemID itemid;
			DWORD quantity;
		};

		struct Trader {
			DWORD gold;
			gw_array<TradeContext::Item> items;
		};

		State state;
		DWORD pad1[3]; // Seemingly 3 null dwords
		Trader player;
		Trader partner;
	};



	class GameContext
	{
		GameContext(){}
		GameContext(const GameContext&){}


	public:

		static GameContext* instance()
		{
			static BYTE* baseptr = nullptr;

			if (baseptr == nullptr)
			{
				PatternScanner scan = PatternScanner(0x401000,0x4FF000);
				baseptr = *(BYTE**)scan.FindPattern("\x85\xC0\x75\x0F\x8B\xCE", "xxxxxx", -4);
			}

			return *(GameContext**)((*(BYTE**)baseptr) + 0x18);
		}

	  void* unkcontext1;
	  void* unkcontext2;
	   AgentContext* agent; // Most functions that access are prefixed with Agent.
	  void* unkcontext4;
	  void* nullcontext1;
	   MapContext* map; // Static object/collision data
	  void* unkcontext5;
	  void* nullcontext2;
	  DWORD somenumber; // 0x30 for me at the moment.
	  void* unkcontext6;
	  void* unkcontext7;
	   WorldContext* world; // Best name to fit it that I can think of.
	  void* unkcontext8;
	  void* nullcontext3;
	  void* unkcontext9;
	   GuildContext* guild;
	   ItemContext* items;
	   CharContext* character;
	  void* nullcontext4;
	   PartyContext* party;
	  void* nullcontext5;
	  void* nullcontext6;
	   TradeContext* trade;
	};


}