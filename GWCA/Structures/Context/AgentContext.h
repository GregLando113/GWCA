#pragma once
#include <Windows.h>

#include "..\GameEntities\Agent.h"
#include "..\GameContainers\gw_array.h"

namespace GWCA {
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
}
