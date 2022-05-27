#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>

#include <GWCA/GameContainers/GamePos.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Player.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>
#include <GWCA/Context/CharContext.h>

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
        CtoS::SendPacket(0x8, GAME_CMSG_TITLE_DISPLAY, (uint32_t)title_id);
    }

    void PlayerMgr::RemoveActiveTitle() {
        CtoS::SendPacket(0x4, GAME_CMSG_TITLE_HIDE);
    }

    PlayerArray* PlayerMgr::GetPlayerArray() {
        auto* w = WorldContext::instance();
        return w && w->players.valid() ? &w->players : nullptr;
    }
    PlayerID PlayerMgr::GetPlayerID() {
        auto* c = CharContext::instance();
        return c ? c->player_id : PlayerID::None;
    }

    Player *PlayerMgr::GetPlayerByID(PlayerID player_id) {
		PlayerArray* players = GetPlayerArray();
        if (!(players && player_id > PlayerID::None && (uint32_t)player_id < players->size()))
            return nullptr;
        return &players->at((uint32_t)player_id);
    }

    wchar_t *PlayerMgr::GetPlayerName(PlayerID player_id) {
		GW::Player* p = GetPlayerByID(player_id);
        return p ? p->name : nullptr;
    }

    bool PlayerMgr::SetPlayerName(PlayerID player_id, const wchar_t *replace_name) {
		GW::Player* p = GetPlayerByID(player_id);
        if (!p) return false;
        // @Cleanup: wcslen, terminate, encoded check
        wcsncpy(p->name_enc + 2, replace_name, 20);
        return true;
    }

    bool PlayerMgr::ChangeSecondProfession(Constants::Profession prof, uint32_t hero_index) {
        AgentID agent_id = Agents::GetHeroAgentID(hero_index);
        if (agent_id == AgentID::None)
            return false;
        CtoS::SendPacket(12, GAME_CMSG_CHANGE_SECOND_PROFESSION, agent_id, prof);
        return true;
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
        PlayerArray* players = GetPlayerArray();
        for (size_t i = 1; players && i < players->size();i++) {
            Player& player = players->at(i);
            if (!player.name) continue;
            if (!wcsncasecmp(name, player.name, 32))
                return &player;
        }
        return nullptr;
    }

} // namespace GW