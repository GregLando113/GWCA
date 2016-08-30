#pragma once

#include "GwConstants.h"
#include "GWStructures.h"
#include "PatternScanner.h"

namespace GWCA {

	using namespace GW;


  class AgentContext;
  class MapContext;
  class WorldContext;
  class GuildContext;
  class ItemContext;
  class CharContext;
  class PartyContext;
  class TradeContext;

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
