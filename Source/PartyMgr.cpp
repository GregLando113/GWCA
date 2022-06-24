#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/GamePos.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Attribute.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/PartyContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PartyMgr.h>
#include <GWCA/Managers/PlayerMgr.h>

namespace {
    using namespace GW;

    typedef void(__cdecl* UpdatePartyWindow_pt)(void* unk0, void* unk1, void* unk2, void* unk3, void* unk4);
    UpdatePartyWindow_pt RetUpdatePartyWindow;
    UpdatePartyWindow_pt UpdatePartyWindow_Func;

    bool tick_work_as_toggle = false;

    void __cdecl OnUpdatePartyWindow(void* unk0, void* unk1, void* unk2, void* unk3, void* unk4) {
        HookBase::EnterHook();
        if (!tick_work_as_toggle) {
            RetUpdatePartyWindow(unk0, unk1, unk2, unk3, unk4);
            HookBase::LeaveHook();
            return;
        }
        uint32_t action = (uint32_t)unk4;
        bool blocked = action < 0x100; // Don't block pointers
        switch (action) {
        case 0x0:
        case 0x1:   // Updating icon
        case 0x8:   // Updating icon
        case 0xA:   // Show ready state icon (map load)
        case 0x13:  // Adding heroes
        case 0x14:  // Mouse move
        case 0x15:  // Mouse move
        case 0x23:  // Hover tooltip
        case 0x29:  // Hover tooltip
        case 0x33:  // Show ready state icon (map load)
        case 0x30:  // Show ready state icon (map load)
        case 0x34:  // Adding heroes
        case 0x2a:  // Ready state icon hover
            blocked = false;
            break;
        case 0x22:  // Ready state icon clicked
            PartyMgr::Tick(!PartyMgr::GetIsPlayerTicked());
            blocked = true;
            break;
        case 0x2c:  // Show ready state dropdown
            blocked = true;
        }
        if (!blocked)
            RetUpdatePartyWindow(unk0, unk1, unk2, unk3, unk4);
        HookBase::LeaveHook();
    }

    void Init() {
        // This function runs every time an update is made to the party window
        UpdatePartyWindow_Func = (UpdatePartyWindow_pt)Scanner::Find(
            "\x4D\x08\x83\xEC\x10\x8B\x41\x04\x56\x83\xF8\x04", "xxxxxxxxxxxx", -0x4);
        GWCA_INFO("[SCAN] PartyWindowUpdate_Func = %p\n", UpdatePartyWindow_Func);
        if (UpdatePartyWindow_Func) {
            HookBase::CreateHook(UpdatePartyWindow_Func, OnUpdatePartyWindow, (void**)&RetUpdatePartyWindow);
        }


    }

    void Exit() {
        if (UpdatePartyWindow_Func)
            HookBase::RemoveHook(UpdatePartyWindow_Func);

    }
}

namespace GW {

    Module PartyModule = {
        "PartyModule",  // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,         // exit_module
        NULL,           // enable_hooks
        NULL,           // disable_hooks
    };
    namespace PartyMgr {
        void Tick(bool flag) {
            CtoS::SendPacket(0x8, GAME_CMSG_PARTY_READY_STATUS, flag);
        }

        Attribute* GetAgentAttributes(uint32_t agent_id) {
            auto* w = WorldContext::instance();
            if (!(w && w->attributes.valid()))
                return nullptr;
            for (auto& agent_attributes : w->attributes) {
                if (agent_attributes.agent_id == agent_id)
                    return agent_attributes.attribute;
            }
            return nullptr;
        }

        PartyInfo* GetPartyInfo(uint32_t party_id) {
            GW::PartyContext* ctx = GW::PartyContext::instance();
            if (!ctx) return 0;
            if (!party_id) return ctx->player_party;
            if (!ctx->parties.valid() || party_id >= ctx->parties.size())
                return 0;
            return ctx->parties[party_id];
        }

        bool GetIsPartyInHardMode() {
            auto* p = PartyContext::instance();
            return p ? p->InHardMode() : false;
        }
        bool GetIsHardModeUnlocked() {
            auto* w = WorldContext::instance();
            return w ? w->is_hard_mode_unlocked != 0 : false;
        }

        uint32_t GetPartySize() {
            PartyInfo* info = GetPartyInfo();
            return info ? info->players.size() + info->heroes.size() + info->henchmen.size() : 0;
        }

        uint32_t GetPartyPlayerCount() {
            PartyInfo* info = GetPartyInfo();
            return info ? info->players.size() : 0;
        }
        uint32_t GetPartyHeroCount() {
            PartyInfo* info = GetPartyInfo();
            return info ? info->heroes.size() : 0;
        }
        uint32_t GetPartyHenchmanCount() {
            PartyInfo* info = GetPartyInfo();
            return info ? info->henchmen.size() : 0;
        }

        bool GetIsPartyDefeated() {
            auto* p = PartyContext::instance();
            return p ? p->IsDefeated() : false;
        }

        bool SetHardMode(bool flag) {
            auto* w = WorldContext::instance();
            if (!w->is_hard_mode_unlocked)
                return false;
            auto* p = PartyContext::instance();
            if (p && p->InHardMode() == flag)
                return true;
            CtoS::SendPacket(0x8, GAME_CMSG_PARTY_SET_DIFFICULTY, flag);
            return true;
        }

        bool GetIsPartyTicked() {
            PartyInfo* info = GetPartyInfo();
            if (!(info && info->players.valid())) return false;
            for (const PlayerPartyMember& player : info->players) {
                if (!player.ticked()) return false;
            }
            return true;
        }

        bool GetIsPartyLoaded() {
            PartyInfo* info = GetPartyInfo();
            if (!(info && info->players.valid())) return false;
            for (const PlayerPartyMember& player : info->players) {
                if (!player.connected()) return false;
            }
            return true;
        }

        bool GetIsPlayerTicked(uint32_t player_index) {
            PartyInfo* info = GetPartyInfo();
            if (!(info && info->players.valid())) return false;
            if (player_index == -1) {
                // Player
                uint32_t player_id = GW::PlayerMgr::GetPlayerNumber();
                for (const PlayerPartyMember& player : info->players) {
                    if (player.login_number == player_id)
                        return player.ticked();
                }
                return false;
            }
            else {
                // Someone else
                if (player_index >= info->players.size()) return false;
                return (info->players[player_index].ticked());
            }
        }

        bool GetIsLeader() {
            PartyInfo* info = GetPartyInfo();
            if (!(info && info->players.valid())) return false;
            uint32_t player_id = GW::PlayerMgr::GetPlayerNumber();
            for (const PlayerPartyMember& player : info->players) {
                if (player.connected())
                    return player.login_number == player_id;
            }
            return false;
        }

        void RespondToPartyRequest(bool accept) {
            CtoS::SendPacket(0x8, accept ? GAME_CMSG_PARTY_ACCEPT_INVITE : GAME_CMSG_PARTY_ACCEPT_CANCEL, 1);
        }

        void AddHero(uint32_t heroid) {
            CtoS::SendPacket(0x8, GAME_CMSG_HERO_ADD, heroid);
        }

        void KickHero(uint32_t heroid) {
            CtoS::SendPacket(0x8, GAME_CMSG_HERO_KICK, heroid);
        }

        void KickAllHeroes() {
            KickHero(0x26);
        }

        void LeaveParty() {
            CtoS::SendPacket(0x4, GAME_CMSG_PARTY_LEAVE_GROUP);
        }

        void FlagHero(uint32_t hero_index, GamePos pos) {
            FlagHeroAgent(Agents::GetHeroAgentID(hero_index), pos);
        }

        void FlagHeroAgent(AgentID agent_id, GamePos pos) {
            struct FlagHero { // Flag Hero
                const uint32_t header = GAME_CMSG_HERO_FLAG_SINGLE;
                uint32_t id;
                GamePos pos;
            };
            if (agent_id == 0) return;
            if (agent_id == Agents::GetPlayerId()) return;
            static FlagHero pak; // TODO
            pak.id = agent_id;
            pak.pos = pos;
            CtoS::SendPacket(&pak);
        }

        void FlagAll(GamePos pos) {
            struct FlagAll { // Flag All
                const uint32_t header = GAME_CMSG_HERO_FLAG_ALL;
                GamePos pos;
            };
            static FlagAll pak;
            pak.pos = pos; // TODO
            CtoS::SendPacket(&pak);
        }

        void UnflagHero(uint32_t hero_index) {
            FlagHero(hero_index, GamePos(HUGE_VALF, HUGE_VALF, 0));
        }

        void UnflagAll() {
            FlagAll(GamePos(HUGE_VALF, HUGE_VALF, 0));
        }

        void SetTickToggle(bool enable) {
            tick_work_as_toggle = enable;
        }
        AgentID GetHeroAgentID(uint32_t hero_index) {
            if (hero_index == 0)
                return Agents::GetPlayerId();
            hero_index--;
            PartyInfo* party = GetPartyInfo();
            if (!party)
                return 0;
            HeroPartyMemberArray& heroes = party->heroes;
            return heroes.valid() && hero_index < heroes.size() ? heroes[hero_index].agent_id : 0;
        }
    }

} // namespace GW