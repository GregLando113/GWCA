#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Player.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PlayerMgr.h>

namespace GW {

    Module PlayerModule = {
        "PlayerModule",     // name
        NULL,               // param
        NULL,               // init_module
        NULL,               // exit_module
        NULL,               // exit_module
        NULL,               // remove_hooks
    };

    void PlayerMgr::SetActiveTitle(Constants::TitleID title_id) {
        CtoS::SendPacket(0x8, CtoGS_MSGSetDisplayedTitle, (uint32_t)title_id);
    }

    void PlayerMgr::RemoveActiveTitle() {
        CtoS::SendPacket(0x4, CtoGS_MSGRemoveDisplayedTitle);
    }

    PlayerArray& PlayerMgr::GetPlayerArray() {
        return GameContext::instance()->world->players;
    }

    Player *PlayerMgr::GetPlayerByID(uint32_t player_id) {
        return &GetPlayerArray()[player_id];
    }

    wchar_t *PlayerMgr::GetPlayerName(uint32_t player_id) {
        return GetPlayerArray()[player_id].name;
    }

    void PlayerMgr::SetPlayerName(uint32_t player_id, const wchar_t *replace_name) {
        wcsncpy(GetPlayerArray()[player_id].name_enc + 2, replace_name, 20);
    }

    void PlayerMgr::ChangeSecondProfession(Constants::Profession prof, uint32_t hero_index) {
        CtoS::SendPacket(12, CtoGS_MSGChangeSecondary, Agents::GetHeroAgentID(hero_index), prof);
    }

    static int wcsncasecmp(const wchar_t *s1, const wchar_t *s2, size_t n)
    {
        if (s1 == s2)   return 0;
        if (s1 == NULL) return -*s2;
        if (s2 == NULL) return *s1;
        for (size_t i = 0; i < n; i++) {
            if (tolower(s1[i]) != tolower(s2[i]) || s1[i] == 0)
                return s1[i] - s2[i];
        }
        return 0;
    }

    Player *PlayerMgr::GetPlayerByName(const wchar_t *name) {
        if (!name) return NULL;
        PlayerArray& players = GetPlayerArray();
        for (Player &player : players) {
            if (!player.name) continue;
            if (!wcsncasecmp(name, player.name, 32))
                return &player;
        }
        return NULL;
    }

} // namespace GW