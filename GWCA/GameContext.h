#pragma once

#include "GwConstants.h"
#include "GWStructures.h"
#include "PatternScanner.h"

namespace GWAPI {

	using namespace GW;

	class AgentContext
	{
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
			};
			//... Bunch of arrays and shit
		} *sub1;
		//... Player coords and shit beyond this point if they are desirable :p
	};

	class WorldContext 
	{
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
		BYTE pad4[0x3FC]; // 0xB0
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
		gw_array<void*> unk12_array; // 0x60C
		DWORD unk3[4]; // 0x61C
		gw_array<void*> unk13_array; // 0x62C
		BYTE pad8[0x44]; // 0x63C
		gw_array<void*> unk14_array; // 0x680 - Struct size = 0x138 i think, has a vtable
		DWORD salvagesessionid; // 0x690
		DWORD unk4[0x9]; // 0x6A0
		gw_array<void*> unk15_array; // 0x6BC
		gw_array<void*> unk16_array; // 0x6CC
		DWORD unk5; // 0x6DC
		gw_array<void*> unk17_array; // 0x6E0
		SkillbarArray skillbar; // 0x6F0
		gw_array<void*> unk18_array; // 0x700
		gw_array<void*> unk19_array; // 0x710
		gw_array<void*> unk20_array; // 0x720
		gw_array<void*> unk21_array; // 0x730
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
		gw_array<void*> unk22_array; // 0x7CC
		gw_array<void*> unk23_array; // 0x7DC
		gw_array<void*> unk24_array; // 0x7EC
		gw_array<void*> unk25_array; // 0x7FC
		PlayerArray players;
		TitleArray titles;
		//... couple more arrays after this
	};
	

	class GuildContext
	{
	private:

	};

	class ItemContext
	{

	};

	class CharContext
	{

	};

	class PartyContext
	{

	};

	class TradeContext
	{
	public:
		enum State : DWORD { NO_TRADE, TRADE_INITIATED, OFFER_ACCEPTED = 3 };

		struct Item {
			DWORD itemid;
			DWORD quantity;
		};

		struct Trader {
			DWORD gold;
			gw_array<Item> items;
		};

		inline State state() const { return state_; }
		inline Trader player() const { return player_; }
		inline Trader partner() const { return partner_; }

	private:
		State state_;
		DWORD pad1_[3]; // Seemingly 3 null dwords
		Trader player_;
		Trader partner_;
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

		AgentContext* Agent() const { return agent_; }
		MapContext* Map() const { return map_; }
		WorldContext* World() const { return world_; }
		GuildContext* Guild() const { return guild_; }
		ItemContext* Items() const { return items_; }
		CharContext* Character() const { return character_; }
		PartyContext* Party() const { return party_; }
		TradeContext* Trade() const { return trade_; }

	private:
		void* unkcontext1;
		void* unkcontext2;
		AgentContext* agent_; // Most functions that access are prefixed with Agent.
		void* unkcontext4;
		void* nullcontext1;
		MapContext* map_; // Static object/collision data
		void* unkcontext5;
		void* nullcontext2;
		DWORD somenumber; // 0x30 for me at the moment.
		void* unkcontext6;
		void* unkcontext7;
		WorldContext* world_; // Best name to fit it that I can think of.
		void* unkcontext8;
		void* nullcontext3;
		void* unkcontext9;
		GuildContext* guild_;
		ItemContext* items_;
		CharContext* character_;
		void* nullcontext4;
		PartyContext* party_;
		void* nullcontext5;
		void* nullcontext6;
		TradeContext* trade_;
	};


}