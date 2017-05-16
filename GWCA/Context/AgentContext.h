#pragma once
#include <Windows.h>

#include <GWCA\GameEntities\Agent.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
	class AgentContext {
		AgentContext() {}
		AgentContext(const AgentContext&) {}

	public:
		Array<void*> unk1_array;
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
		Array<void*> unk2_array;
		DWORD unk1; // was 1 for me
		Array<void*> unk3_array;
		Array<void*> unk4_array;
		Array<void*> unk5_array;
		DWORD rand1; // Number seems to be randomized quite a bit o.o seems to be accessed by textparser.cpp
		DWORD rand2;
		BYTE pad7[0x18];
		Array<AgentMovement*> agentmovement;
		Array<void*> unk6_array;
		BYTE pad8[0x44];
		Array<void*> unk7_array;
		Array<void*> unk8_array;
		BYTE pad9[0x40];
		DWORD instancetimer;
		//... more but meh
	};
}
