#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

// #include <GWCA/GameContainers/List.h>
#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Player.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/PartyContext.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PartyMgr.h>

namespace {
    typedef DWORD(__stdcall *Tick_t)(DWORD unk1);
    GW::THook<Tick_t> hk_tick_;

    // Parameter is always 1 or 2 creating "Ready" or "Not ready"
    DWORD __stdcall DetourTick(DWORD unk1) {
        // this func is always called twice so use this hack to tick only once
        static bool toggle = true;
        toggle = !toggle;
        if (toggle) return 4;

        GW::PartyMgr::Tick(!GW::PartyMgr::GetIsPlayerTicked());
        return 4;
    }
}

void GW::PartyMgr::SetTickToggle() {
    if (hk_tick_.Empty()) {
        Tick_t addr_tick = (Tick_t)Scanner::Find("\x74\x0A\x48\x75\x14\xB9", "xxxxxx", -33);
        printf("[SCAN] addr_tick = %p\n", addr_tick);
        hk_tick_.Detour(addr_tick, DetourTick);
    }
}

void GW::PartyMgr::RestoreTickToggle() {
    HookBase::DisableHooks(&hk_tick_);
}

void GW::PartyMgr::Tick(bool flag) {
    CtoS::SendPacket(0x8, CtoGS_MSGTick, flag);
}

GW::PartyInfo* GW::PartyMgr::GetPartyInfo() {
    return GameContext::instance()->party->player_party;
}

DWORD GW::PartyMgr::GetPartySize() {
    GW::PartyInfo* info = GetPartyInfo();
    if (info == nullptr) return 0;
    return info->players.size() + info->heroes.size() + info->henchmen.size();
}

DWORD GW::PartyMgr::GetPartyPlayerCount() {
    if (GetPartyInfo()) {
        return GetPartyInfo()->players.size();
    } else {
        return 0;
    }
}
DWORD GW::PartyMgr::GetPartyHeroCount() {
    if (GetPartyInfo()) {
        return GetPartyInfo()->heroes.size();
    } else {
        return 0;
    }
}
DWORD GW::PartyMgr::GetPartyHenchmanCount() {
    if (GetPartyInfo()) {
        return GetPartyInfo()->henchmen.size();
    } else {
        return 0;
    }
}

bool GW::PartyMgr::GetIsPartyDefeated() {
    return GameContext::instance()->party->IsDefeated(); 
}

void GW::PartyMgr::SetHardMode(bool flag) {
    CtoS::SendPacket(0x8, CtoGS_MSGSwitchMode, flag);
}
bool GW::PartyMgr::GetIsPartyInHardMode() {
    return GameContext::instance()->party->InHardMode();
}

bool GW::PartyMgr::GetIsPartyTicked() {
    GW::PartyInfo* info = GetPartyInfo();
    if (info == nullptr) return false;
    if (!info->players.valid()) return false;
    for (GW::PlayerPartyMember player : info->players) {
        if (!player.ticked()) return false;
    }
    return true;
}

bool GW::PartyMgr::GetIsPartyLoaded() {
    GW::PartyInfo* info = GetPartyInfo();
    if (info == nullptr) return false;
    if (!info->players.valid()) return false;
    for (GW::PlayerPartyMember player : info->players) {
        if (!player.connected()) return false;
    }
    return true;
}

bool GW::PartyMgr::GetIsTicked(DWORD player_index) {
    GW::PartyInfo* info = GetPartyInfo();
    if (info == nullptr) return false;
    if (!info->players.valid()) return false;
    if (player_index >= info->players.size()) return false;
    return (info->players[player_index].ticked());
}

bool GW::PartyMgr::GetIsPlayerTicked() {
    GW::PartyInfo* info = GetPartyInfo();
    if (info == nullptr) return false;
    if (!info->players.valid()) return false;
    GW::Agent* me = Agents::GetPlayer();
    if (me == nullptr) return false;
    for (DWORD i = 0; i < info->players.size(); i++) {
        if (info->players[i].login_number == me->login_number) {
            return info->players[i].ticked();
        }
    }
    return false;
}

bool GW::PartyMgr::GetPlayerIsLeader() {
    GW::PartyInfo *party = GetPartyInfo();
    if (!party) return false;
    GW::Agent *player_agent = Agents::GetPlayer();
    if (!player_agent) return false;
    if (!party->players.size()) return false;
    return (party->players[0].login_number == player_agent->login_number);
}

void GW::PartyMgr::RespondToPartyRequest(bool accept) {
    CtoS::SendPacket(0x8, accept ? CtoGS_MSGAcceptPartyRequest : CtoGS_MSGDenyPartyRequest, 1);
}

void GW::PartyMgr::AddHero(DWORD heroid) {
    CtoS::SendPacket(0x8, CtoGS_MSGAddHero, heroid);
}

void GW::PartyMgr::KickHero(DWORD heroid) {
    CtoS::SendPacket(0x8, CtoGS_MSGKickHero, heroid);
}

void GW::PartyMgr::KickAllHeroes() {
    KickHero(0x26);
}

void GW::PartyMgr::LeaveParty() {
    CtoS::SendPacket(0x4, CtoGS_MSGLeaveParty);
}

void GW::PartyMgr::FlagHero(DWORD hero_index, GW::GamePos pos) {
    DWORD agent_id = Agents::GetHeroAgentID(hero_index);
    FlagHeroAgent(agent_id, pos);
}

void GW::PartyMgr::FlagHeroAgent(GW::AgentID agent_id, GW::GamePos pos) {
    struct FlagHero { // Flag Hero
        const DWORD header = CtoGS_MSGCommandHero;
        DWORD id;
        GamePos pos;
    };
    if (agent_id == 0) return;
    if (agent_id == Agents::GetPlayerId()) return;
    static FlagHero pak; // TODO
    pak.id = agent_id;
    pak.pos = pos;
    CtoS::SendPacket(&pak);
}

void GW::PartyMgr::FlagAll(GW::GamePos pos) {
    struct FlagAll { // Flag All
        const DWORD header = CtoGS_MSGCommandAll;
        GamePos pos;
    };
    static FlagAll pak;
    pak.pos = pos; // TODO
    CtoS::SendPacket(&pak);
}

void GW::PartyMgr::UnflagHero(DWORD hero_index) {
    FlagHero(hero_index, GW::GamePos(HUGE_VALF, HUGE_VALF, 0));
}

void GW::PartyMgr::UnflagAll() {
    FlagAll(GW::GamePos(HUGE_VALF, HUGE_VALF, 0)); 
}
