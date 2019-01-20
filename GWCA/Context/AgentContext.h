#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct AgentMovement;

    struct AgentToGadget {
        struct GadgetID {
            uint32_t h0000;
            uint32_t h0004;
            uint32_t gadget_id;
            uint32_t h000C;
            wchar_t *name_enc;
        };

        uint32_t h0000;
        uint32_t h0004;
        GadgetID *gadget_ids;
    };

    struct AgentContext {
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
        /* +h0094 */   uint32_t h0094; // this field and the next array are link together in a structure.
        /* +h0098 */   Array<AgentToGadget> gadget_data; // elements are of size 12. {ptr, func, ptr}
        /* +h00A8 */ Array<void *> h00A8;
        /* +h00B8 */ Array<void *> h00B8;
        /* +h00C8 */ uint32_t rand1; // Number seems to be randomized quite a bit o.o seems to be accessed by textparser.cpp
        /* +h00CC */ uint32_t rand2;
        /* +h00D0 */ uint8_t h00D0[24];
        /* +h00E8 */ Array<AgentMovement *> agent_movement;
        /* +h00F8 */ Array<void *> h00F8;
        /* +h0108 */ uint8_t h0108[68];
        /* +h014C */ Array<void *> h014C;
        /* +h015C */ Array<void *> h015C;
        /* +h016C */ uint8_t h016C[64];
        /* +h01AC */ uint32_t instance_timer;
        //... more but meh
    };
}
