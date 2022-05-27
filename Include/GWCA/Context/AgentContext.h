#pragma once
#include <GWCA/GameEntities/Agent.h>

namespace GW {

    struct AgentContext {

        static AgentContext* instance();

        /* +h0000 */ Array<void *> h0000;
        /* +h0010 */ uint32_t h0010[5];
        /* +h0024 */ uint32_t h0024; // function
        /* +h0028 */ uint32_t h0028[2];
        /* +h0030 */ uint32_t h0030; // function
        /* +h0034 */ uint32_t h0034[2];
        /* +h003C */ uint32_t h003C; // function
        /* +h0040 */ uint32_t h0040[2];
        /* +h0048 */ uint32_t h0048; // function
        /* +h004C */ uint32_t h004C[2];
        /* +h0054 */ uint32_t h0054; // function
        /* +h0058 */ uint32_t h0058[11];
        /* +h0084 */ Array<void *> h0084;
        /* +h0094 */ uint32_t h0094; // this field and the next array are link together in a structure.
        /* +h0098 */ Array<AgentSummaryInfo> agent_summary_info; // elements are of size 12. {ptr, func, ptr}
        /* +h00A8 */ Array<void *> h00A8;
        /* +h00B8 */ Array<void *> h00B8;
        /* +h00C8 */ uint32_t rand1; // Number seems to be randomized quite a bit o.o seems to be accessed by textparser.cpp
        /* +h00CC */ uint32_t rand2;
        /* +h00D0 */ uint8_t h00D0[24];
        /* +h00E8 */ Array<AgentMovement *> agent_movement;
        /* +h00F8 */ Array<void *> h00F8;
        /* +h0108 */ uint32_t h0108[0x11];
        /* +h014C */ Array<void *> agent_array1;
        /* +h015C */ Array<void *> agent_async_movement;
        /* +h016C */ uint32_t h016C[0x10];
        /* +h01AC */ uint32_t instance_timer;
        //... more but meh
    };

    static_assert(sizeof(AgentContext) == 0x1B0);
}
