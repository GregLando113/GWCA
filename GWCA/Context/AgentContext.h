#ifndef _AGENT_CONTEXT_INC
#define _AGENT_CONTEXT_INC

#include <Windows.h>

#include <GWCA\GameEntities\Agent.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
	using wchar = wchar_t;

    struct AgentToGadget {
		struct GadgetID {
			DWORD h0000;
			DWORD h0004;
			DWORD GadgetId;
			DWORD h000C;
			wchar *NameString;
		};

        DWORD h0000;
        DWORD h0004;
		GadgetID *GadgetIds;
    };

    struct AgentContext {
        /* +h0000 */ Array<void*> h0000;
        /* +h0010 */ DWORD h0010[5];
        /* +h0024 */ DWORD h0024; // function
        /* +h0028 */ DWORD h0028[2];
        /* +h0030 */ DWORD h0030; // function
        /* +h0034 */ DWORD h0034[2];
        /* +h003C */ DWORD h003C; // function
        /* +h0040 */ DWORD h0040[2];
        /* +h0048 */ DWORD h0048; // function
        /* +h004C */ DWORD h004C[2];
        /* +h0054 */ DWORD h0054; // function
        /* +h0058 */ DWORD h0058[11];
        /* +h0084 */ Array<void*> h0084;
        /* +h0094 */   DWORD h0094; // this field and the next array are link together in a structure.
        /* +h0098 */   Array<AgentToGadget> GadgetData; // elements are of size 12. {ptr, func, ptr}
        /* +h00A8 */ Array<void*> h00A8;
        /* +h00B8 */ Array<void*> h00B8;
        /* +h00C8 */ DWORD rand1; // Number seems to be randomized quite a bit o.o seems to be accessed by textparser.cpp
        /* +h00CC */ DWORD rand2;
        /* +h00D0 */ BYTE h00D0[24];
        /* +h00E8 */ Array<AgentMovement*> agentmovement;
        /* +h00F8 */ Array<void*> h00F8;
        /* +h0108 */ BYTE h0108[68];
        /* +h014C */ Array<void*> h014C;
        /* +h015C */ Array<void*> h015C;
        /* +h016C */ BYTE h016C[64];
        /* +h01AC */ DWORD instancetimer;
        //... more but meh
    };
}

#endif // _AGENT_CONTEXT_INC
