#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Title.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

namespace {
    struct TitleClientData {
        GW::Constants::TitleID title_id;
        uint32_t name_id;
    };
    TitleClientData** title_data = 0;
}

namespace GW {

    uint32_t Title::name_id() {
        if (!title_data) {
            title_data = (TitleClientData**)Scanner::FindAssertion("p:\\code\\gw\\const\\consttitle.cpp", "index < arrsize(s_titleClientData)", 0x12);
        }
        return title_data ? (*title_data)[(uint32_t)title_id()].name_id : 0;
    }
    Constants::TitleID Title::title_id()  {
        const Array<Title>& titles = GameContext::instance()->world->titles;
        for (uint32_t i = 0; i < titles.size(); i++) {
            if (titles[i].max_title_tier_index == max_title_tier_index)
                return (Constants::TitleID)i;
        }
        return Constants::TitleID::None;
    }
}
