#pragma once
#include <Windows.h>

#include <GWCA\GameEntities\Agent.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
	class AgentContext {
		AgentContext() {}
		AgentContext(const AgentContext&) {}

	public:
		/* +h0000 */ Array<void*> unk1_array;
		/* +h0010 */ DWORD pad1[5];
		/* +h0024 */ void* function1;
		/* +h0028 */ DWORD pad2[2];
		/* +h0030 */ void* function2;
		/* +h0034 */ DWORD pad3[2];
		/* +h003C */ void* function3;
		/* +h0040 */ DWORD pad4[2];
		/* +h0048 */ void* function4;
		/* +h004C */ DWORD pad5[2];
		/* +h0054 */ void* function5;
		/* +h0058 */ DWORD pad6[11];
		/* +h0084 */ Array<void*> unk2_array;
		/* +h0094 */   DWORD unk1; // this field and the next array are link together in a structure.
		/* +h0098 */   Array<void*> unk3_array; // elements are of size 12. {ptr, func, ptr}
		/* +h00A8 */ Array<void*> unk4_array;
		/* +h00B8 */ Array<void*> unk5_array;
		/* +h00C8 */ DWORD rand1; // Number seems to be randomized quite a bit o.o seems to be accessed by textparser.cpp
		/* +h00CC */ DWORD rand2;
		/* +h00D0 */ BYTE pad7[24];
		/* +h00E8 */ Array<AgentMovement*> agentmovement;
		/* +h00F8 */ Array<void*> unk6_array;
		/* +h0108 */ BYTE pad8[68];
		/* +h014C */ Array<void*> unk7_array;
		/* +h015C */ Array<void*> unk8_array;
		/* +h016C */ BYTE pad9[64];
		/* +h01AC */ DWORD instancetimer;
		//... more but meh
	};
}
