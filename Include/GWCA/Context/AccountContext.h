#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct AccountContext;
    GWCA_API AccountContext* GetAccountContext();

    struct AccountUnlockedCount {
        uint32_t id;
        uint32_t unk1;
        uint32_t unk2;
    };

    struct AccountContext {

        static AccountContext* instance();

        /* +h0000 */ Array<AccountUnlockedCount> account_unlocked_counts; // e.g. number of unlocked storage panes
        /* +h0010 */ uint32_t h0010[0x7];
        /* +h0029 */ Array<void*> h0029;
        /* +h0039 */ uint32_t h0039[0x1e]; // Some arrays, some linked lists, meh
        /* +h00b4 */ Array<uint32_t> unlocked_pvp_items;
        /* +h00c4 */ uint32_t h00c4[24];
        /* +h0124 */ Array<uint32_t> unlocked_account_skills; // List of skills unlocked (but not learnt) for this account, i.e. skills that heros can use, tomes can unlock
        /* +h0134 */ uint32_t account_flags;
    };
}
