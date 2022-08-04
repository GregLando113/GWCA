#include "stdafx.h"

#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Title.h>

#include <GWCA/Managers/PlayerMgr.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

namespace GW {

    uint32_t Title::name_id() {
        return PlayerMgr::GetTitleData(title_id())->name_id;
    }
    Constants::TitleID Title::title_id()  {
        const Array<Title>& titles = GameContext::instance()->world->titles;
        for (uint32_t i = 0; i < titles.size(); i++) {
            if (titles[i].max_title_tier_index == max_title_tier_index)
                return (Constants::TitleID)i;
        }
        return (Constants::TitleID)0xff;
    }
}
