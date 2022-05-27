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

#include <GWCA/Context/WorldContext.h>
#include <GWCA/Context/PartyContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PlayerMgr.h>
#include <GWCA/Managers/PartyMgr.h>

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
    
    void PartyMgr::Tick(bool flag) {
        CtoS::SendPacket(0x8, GAME_CMSG_PARTY_READY_STATUS, flag);
    }

    GW::Array<PartyInfo*>* PartyMgr::GetPartyArray() {
        auto* p = PartyContext::instance();
        return p && p->parties.valid() ? &p->parties : nullptr;
    }

    PartyInfo* PartyMgr::GetPartyInfo(PartyID party_id) {
        if (party_id == PartyID::PlayerParty) {
            auto* p = PartyContext::instance();
            return p ? p->player_party : nullptr;
        }
        auto* parties = GetPartyArray();
        return parties && (uint32_t)party_id < parties->size() ? parties->at((uint32_t)party_id) : nullptr;
    }

    uint32_t PartyMgr::GetPartySize() {
        PartyInfo* info = GetPartyInfo();
        return info ? info->GetPartySize() : 0;
    }

    uint32_t PartyMgr::GetPartyPlayerCount() {
        PartyInfo* info = GetPartyInfo();
        return info && info->players.valid() ? info->players.size() : 0;
    }
    uint32_t PartyMgr::GetPartyHeroCount() {
        PartyInfo* info = GetPartyInfo();
        return info && info->heroes.valid() ? info->heroes.size() : 0;
    }
    uint32_t PartyMgr::GetPartyHenchmanCount() {
        PartyInfo* info = GetPartyInfo();
        return info && info->henchmen.valid() ? info->heroes.size() : 0;
    }

    bool PartyMgr::GetIsPartyDefeated() {
        auto* p = PartyContext::instance();
        return p ? p->IsDefeated() : false;
    }

    void PartyMgr::SetHardMode(bool flag) {
        CtoS::SendPacket(0x8, GAME_CMSG_PARTY_SET_DIFFICULTY, flag);
    }
    bool PartyMgr::GetIsPartyInHardMode() {
        auto* p = PartyContext::instance();
        return p ? p->InHardMode() : false;
    }
    bool PartyMgr::GetIsHardModeUnlocked() {
        auto* w = WorldContext::instance();
        return w ? w->is_hard_mode_unlocked : false;
    }

    bool PartyMgr::GetIsPartyTicked() {
        PartyInfo* info = GetPartyInfo();
        if (info == nullptr) return false;
        if (!info->players.valid()) return false;
        for (const PlayerPartyMember& player : info->players) {
            if (!player.ticked()) return false;
        }
        return true;
    }

    bool PartyMgr::GetIsPartyLoaded() {
        PartyInfo* info = GetPartyInfo();
        if (!(info && info->players.valid())) return false;
        for (const PlayerPartyMember& player : info->players) {
            if (!player.connected()) return false;
        }
        return true;
    }

    bool PartyMgr::GetIsTicked(uint32_t player_index) {
        PartyInfo* info = GetPartyInfo();
        if (!(info && info->players.valid())) return false;
        if (player_index >= info->players.size()) return false;
        return (info->players[player_index].ticked());
    }

    bool PartyMgr::GetIsPlayerTicked() {
        PartyInfo* info = GetPartyInfo();
        if (!(info && info->players.valid())) return false;
        PlayerID player_id = PlayerMgr::GetPlayerID();
        for (const PlayerPartyMember& player : info->players) {
            if (player.player_id == player_id) return player.ticked();
        }
        return false;
    }

    bool PartyMgr::GetPlayerIsLeader() {
        PartyInfo * info = GetPartyInfo();
        if (!(info && info->players.valid())) return false;
        PlayerID player_id = PlayerMgr::GetPlayerID();
        for (const PlayerPartyMember& player : info->players) {
            if (player.connected()) return player.player_id == player_id;;
        }
        return false;
    }

    void PartyMgr::RespondToPartyRequest(bool accept) {
        CtoS::SendPacket(0x8, accept ? GAME_CMSG_PARTY_ACCEPT_INVITE : GAME_CMSG_PARTY_ACCEPT_CANCEL, 1);
    }

    void PartyMgr::AddHero(Constants::HeroID heroid) {
        CtoS::SendPacket(0x8, GAME_CMSG_HERO_ADD, heroid);
    }

    void PartyMgr::KickHero(Constants::HeroID heroid) {
        CtoS::SendPacket(0x8, GAME_CMSG_HERO_KICK, heroid);
    }

    void PartyMgr::KickAllHeroes() {
        KickHero((Constants::HeroID)0x26);
    }

    void PartyMgr::LeaveParty() {
        CtoS::SendPacket(0x4, GAME_CMSG_PARTY_LEAVE_GROUP);
    }

    void PartyMgr::FlagHero(uint32_t hero_index, GamePos pos) {
        FlagHeroAgent(Agents::GetHeroAgentID(hero_index), pos);
    }

    void PartyMgr::FlagHeroAgent(AgentID agent_id, GamePos pos) {
        struct FlagHero { // Flag Hero
            const uint32_t header = GAME_CMSG_HERO_FLAG_SINGLE;
            AgentID id;
            GamePos pos;
        };
        if (agent_id == AgentID::None) return;
        if (agent_id == Agents::GetPlayerId()) return;
        static FlagHero pak; // TODO
        pak.id = agent_id;
        pak.pos = pos;
        CtoS::SendPacket(&pak);
    }

    void PartyMgr::FlagAll(GamePos pos) {
        struct FlagAll { // Flag All
            const uint32_t header = GAME_CMSG_HERO_FLAG_ALL;
            GamePos pos;
        };
        static FlagAll pak;
        pak.pos = pos; // TODO
        CtoS::SendPacket(&pak);
    }

    void PartyMgr::UnflagHero(uint32_t hero_index) {
        FlagHero(hero_index, GamePos(HUGE_VALF, HUGE_VALF, 0));
    }

    void PartyMgr::UnflagAll() {
        FlagAll(GamePos(HUGE_VALF, HUGE_VALF, 0)); 
    }

    void PartyMgr::SetTickToggle(bool enable) {
        tick_work_as_toggle = enable;
    }

} // namespace GW