#ifndef _GAME_CONTEXT_INC
#define _GAME_CONTEXT_INC

#include "AgentContext.h"
#include "MapContext.h"
#include "WorldContext.h"
#include "GuildContext.h"
#include "ItemContext.h"
#include "CharContext.h"
#include "PartyContext.h"
#include "TradeContext.h"
#include "GadgetContext.h"
#include "TextParser.h"

#include <GWCA\Utilities\Scanner.h>

namespace GW {

    struct GameContext {

        static GameContext* instance() {
            static BYTE* baseptr = nullptr;

            if (baseptr == nullptr) {
                baseptr = *(BYTE**)Scanner::Find("\x85\xC0\x75\x0F\x8B\xCE", "xxxxxx", -4);
            }

            return *(GameContext**)((*(BYTE**)baseptr) + 0x18);
        }

        /* +h0000 */ void* unkcontext1;
        /* +h0004 */ void* unkcontext2;
        /* +h0008 */ AgentContext* agent; // Most functions that access are prefixed with Agent.
        /* +h000C */ void* unkcontext4;
        /* +h0010 */ void* nullcontext1;
        /* +h0014 */ MapContext* map; // Static object/collision data
        /* +h0018 */ TextParser *text_parser;
        /* +h001C */ void* nullcontext2;
        /* +h0020 */ DWORD somenumber; // 0x30 for me at the moment.
        /* +h0024 */ void* unkcontext6;
        /* +h0028 */ void* unkcontext7;
        /* +h002C */ WorldContext* world; // Best name to fit it that I can think of.
        /* +h0030 */ void* unkcontext8;
        /* +h0034 */ void* nullcontext3;
        /* +h0038 */ GadgetContext* gadget;
        /* +h003C */ GuildContext* guild;
        /* +h0040 */ ItemContext* items;
        /* +h0044 */ CharContext* character;
        /* +h0048 */ void* nullcontext4;
        /* +h004C */ PartyContext* party;
        /* +h0050 */ void* nullcontext5;
        /* +h0054 */ void* nullcontext6;
        /* +h0058 */ TradeContext* trade;
    };
}

#endif // _GAME_CONTEXT_INC
