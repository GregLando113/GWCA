#pragma once

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
	private:
		State state;
		DWORD pad1[3]; // Seemingly 3 null dwords
		Trader player;
		Trader partner;
	};

	class GameContext
	{
		GameContext(){}
		GameContext(const GameContext&){}

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


		static GameContext* instanceptr()
		{
			BYTE* baseptr = nullptr;

			if (baseptr == nullptr)
			{
				PatternScanner scan(0x401000,0x4FF000);
				baseptr = (BYTE*)scan.FindPattern("\x85\xC0\x75\x0F\x8B\xCE", "xxxxxx", -4);
			}

			return *(GameContext**)((*(BYTE**)baseptr) + 0x18);
		}

		inline static GameContext instance() { return *instanceptr(); }
	};


}