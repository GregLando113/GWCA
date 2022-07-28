#include "stdafx.h"

#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Title.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

namespace {
    GW::TitleClientData** title_data = 0;
}

namespace GW {

    uint32_t Title::name_id() {
        if (!title_data) {
            title_data = (TitleClientData**)Scanner::FindAssertion("p:\\code\\gw\\const\\consttitle.cpp", "index < arrsize(s_titleClientData)", 0x12);
        }
        return title_data ? (*title_data)[title_id()].name_id : 0;
    }
    uint32_t Title::title_id()  {
        const Array<Title>& titles = GameContext::instance()->world->titles;
        for (uint32_t i = 0; i < titles.size(); i++) {
            if (titles[i].max_title_tier_index == max_title_tier_index)
                return i;
        }
        return -1;
    }
}
