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
    namespace PlayerMgr {
        void SetActiveTitle(Constants::TitleID title_id) {
            CtoS::SendPacket(0x8, GAME_CMSG_TITLE_DISPLAY, (uint32_t)title_id);
        }

        void RemoveActiveTitle() {
            CtoS::SendPacket(0x4, GAME_CMSG_TITLE_HIDE);
        }
        uint32_t GetAmountOfPlayersInInstance() {
            auto* w = WorldContext::instance();
            // -1 because the 1st array element is nil
            return w && w->players.valid() ? w->players.size() - 1 : 0;
        }
        PlayerArray* GetPlayerArray() {
            auto* w = WorldContext::instance();
            return w && w->players.valid() ? &w->players : nullptr;
        }
        PlayerNumber GetPlayerNumber() {
            auto* c = CharContext::instance();
            return c ? c->player_number : 0;
        }

        Player* GetPlayerByID(uint32_t player_id) {
            auto* players = GetPlayerArray();
            return players && player_id < players->size() ? &players->at(player_id) : nullptr;
        }

        wchar_t* GetPlayerName(uint32_t player_id) {
            GW::Player* p = GetPlayerByID(player_id);
            return p ? p->name : nullptr;
        }

        wchar_t* SetPlayerName(uint32_t player_id, const wchar_t* replace_name) {
            GW::Player* p = GetPlayerByID(player_id);
            return p ? wcsncpy(p->name_enc + 2, replace_name, 20) : nullptr;
        }

        void ChangeSecondProfession(Constants::Profession prof, uint32_t hero_index) {
            CtoS::SendPacket(12, GAME_CMSG_CHANGE_SECOND_PROFESSION, Agents::GetHeroAgentID(hero_index), prof);
        }

        static int wcsncasecmp(const wchar_t* s1, const wchar_t* s2, size_t n)
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

        Player* GetPlayerByName(const wchar_t* name) {
            if (!name) return nullptr;
            PlayerArray* players = GetPlayerArray();
            if (!players) return nullptr;
            for (Player& player : *players) {
                if (!player.name) continue;
                if (!wcsncasecmp(name, player.name, 32))
                    return &player;
            }
            return nullptr;
        }
    }

} // namespace GW