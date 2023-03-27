#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Quest.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Player.h>
#include <GWCA/GameEntities/Title.h>

#include <GWCA/Context/WorldContext.h>
#include <GWCA/Context/CharContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PlayerMgr.h>
#include <GWCA/Managers/SkillbarMgr.h>

namespace {
    using namespace GW;

    typedef void(__cdecl* Void_pt)();
    Void_pt RemoveActiveTitle_Func = 0;

    typedef void(__cdecl* DoAction_pt)(uint32_t identifier);
    DoAction_pt SetActiveTitle_Func = 0;

    typedef void(__cdecl* DepositFaction_pt)(uint32_t always_0, uint32_t allegiance, uint32_t amount);
    DepositFaction_pt DepositFaction_Func;

    GW::TitleClientData* title_data = 0;

    void Init() {
        DWORD address = 0;

        address = Scanner::Find("\x8b\x4e\x08\x6a\x00\x68\x00\x83\x01\x00", "xxxxxxxxxx", -0x31); // UI::UIInteractionCallback for title list
        RemoveActiveTitle_Func = (Void_pt)Scanner::FunctionFromNearCall(address + 0x1d6);
        SetActiveTitle_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address + 0x1c8);

        address = Scanner::Find("\x68\x88\x13\x00\x00\xff\x76\x0c\x6a\x00", "xxxxxxxxxx", 0xa); // UI::UIInteractionCallback for entering player name for faction donation
        DepositFaction_Func = (DepositFaction_pt)Scanner::FunctionFromNearCall(address);

        title_data = *(TitleClientData**)Scanner::FindAssertion("p:\\code\\gw\\const\\consttitle.cpp", "index < arrsize(s_titleClientData)", 0x12);

        GWCA_INFO("[SCAN] title_data = %p", title_data);
        GWCA_INFO("[SCAN] RemoveActiveTitle_Func = %p", RemoveActiveTitle_Func);
        GWCA_INFO("[SCAN] SetActiveTitle_Func = %p", SetActiveTitle_Func);
        GWCA_INFO("[SCAN] DepositFaction_Func = %p", DepositFaction_Func);

#ifdef _DEBUG
        GWCA_ASSERT(RemoveActiveTitle_Func);
        GWCA_ASSERT(SetActiveTitle_Func);
        GWCA_ASSERT(DepositFaction_Func);
#endif
    }
}

namespace GW {

    Module PlayerModule = {
        "PlayerModule",     // name
        NULL,               // param
        Init,               // init_module
        NULL,               // exit_module
        NULL,               // exit_module
        NULL,               // remove_hooks
    };
    namespace PlayerMgr {
        bool SetActiveTitle(Constants::TitleID title_id) {
            return SetActiveTitle_Func ? SetActiveTitle_Func((uint32_t)title_id), true : false;
        }

        bool RemoveActiveTitle() {
            return RemoveActiveTitle_Func ? RemoveActiveTitle_Func(), true : false;
        }

        uint32_t GetPlayerAgentId(uint32_t player_id) {
            auto* player = GetPlayerByID(player_id);
            return player ? player->agent_id : 0;
        }

        GW::TitleArray* GetTitleArray() {
            auto w = GetWorldContext();
            return (w && w->titles.valid()) ? &w->titles : nullptr;
        }

        Title* GetTitleTrack(Constants::TitleID title_id) {
            auto a = GetTitleArray();
            if (!(a && a->size() > (uint32_t)title_id))
                return nullptr;
            return &a->at((uint32_t)title_id);
        }

        TitleClientData* GetTitleData(Constants::TitleID title_id) {
            return title_data ? &title_data[(uint32_t)title_id] : nullptr;
        }

        Constants::TitleID GetActiveTitleId() {
            auto* player = GetPlayerByID();
            if (!(player && player->active_title_tier))
                return Constants::TitleID::None;
            auto a = GetTitleArray();
            if (!a)
                return Constants::TitleID::None;
            auto& titles = *a;
            for (size_t title_id = 0; title_id < titles.size();title_id++) {
                if (titles[title_id].current_title_tier_index == player->active_title_tier) {
                    return (Constants::TitleID)title_id;
                }
            }
            return Constants::TitleID::None;
        }

        Title* GetActiveTitle() {
            return GetTitleTrack(GetActiveTitleId());
        }

        uint32_t GetAmountOfPlayersInInstance() {
            return Agents::GetAmountOfPlayersInInstance();
        }
        PlayerArray* GetPlayerArray() {
            auto* w = GetWorldContext();
            return w && w->players.valid() ? &w->players : nullptr;
        }
        PlayerNumber GetPlayerNumber() {
            auto* c = GetCharContext();
            return c ? c->player_number : 0;
        }

        Player* GetPlayerByID(uint32_t player_id) {
            if (!player_id) {
                player_id = GetPlayerNumber();
            }
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

        bool ChangeSecondProfession(Constants::Profession prof, uint32_t hero_index) {
            return SkillbarMgr::ChangeSecondProfession(prof, hero_index);
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

        bool DepositFaction(uint32_t allegiance) {
            return DepositFaction_Func ? DepositFaction_Func(0, allegiance, 5000), true : false;
        }
    }

} // namespace GW
