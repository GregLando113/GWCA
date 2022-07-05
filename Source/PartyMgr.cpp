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
    // General purpose "void function that does something with this id"
    typedef void(__cdecl* DoAction_pt)(uint32_t identifier);
    typedef void(__cdecl* Void_pt)();

    DoAction_pt SetDifficulty_Func;
    DoAction_pt PartyAcceptInvite_Func;
    DoAction_pt PartyRejectInvite_Func;
    DoAction_pt AddHero_Func;
    DoAction_pt KickHero_Func;
    DoAction_pt KickPlayer_Func;
    DoAction_pt SetReadyStatus_Func;
    Void_pt LeaveParty_Func;

    typedef void(__cdecl* FlagHeroAgent_pt)(uint32_t agent_id,GW::GamePos* pos);
    FlagHeroAgent_pt FlagHeroAgent_Func;

    typedef void(__cdecl* FlagAll_pt)(GW::GamePos* pos);
    FlagAll_pt FlagAll_Func;

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
        DWORD address = Scanner::Find("\x8b\x75\x0c\x83\xc4\x04\x83\x3e\x00\x0f?????\xff\x70\x20","xxxxxxxxxx?????xxx",0x12);
        SetDifficulty_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        // NB: 0x4c4 is an encoded string id
        address = Scanner::Find("\x83\xc4\x30\x6a\x00\x68\xc4\x04","xxxxxxxx");
        PartyAcceptInvite_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address + 0x11f);
        AddHero_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address + 0x8f);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\party\\ptbuttons.cpp", "charHero", 0x11);
        KickHero_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\party\\ptbuttons.cpp", "m_selection.playerId", 0x13);
        KickPlayer_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        address += 0x4a;
        LeaveParty_Func = (Void_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\party\\ptbuttons.cpp", "m_selection.partyId", 0x13);
        PartyRejectInvite_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\party\\ptplayer.cpp", "No valid case for switch variable '\"\"'", 0x27);
        SetReadyStatus_Func = (DoAction_pt)Scanner::FunctionFromNearCall(address);

        address = Scanner::Find("\x8b\x45\xf8\x89\x45\xe8\x8b\x45\xfc\x89\x45\xec\x8d\x45\xe8","xxxxxxxxxxxxxxx", 0x22);
        FlagAll_Func = (FlagAll_pt)Scanner::FunctionFromNearCall(address);

        address += 0x1b;
        FlagHeroAgent_Func = (FlagHeroAgent_pt)Scanner::FunctionFromNearCall(address);

#ifdef _DEBUG
        GWCA_ASSERT(UpdatePartyWindow_Func);
        GWCA_ASSERT(SetDifficulty_Func);
        GWCA_ASSERT(PartyAcceptInvite_Func);
        GWCA_ASSERT(AddHero_Func);
        GWCA_ASSERT(KickHero_Func);
        GWCA_ASSERT(KickPlayer_Func);
        GWCA_ASSERT(LeaveParty_Func);
        GWCA_ASSERT(PartyRejectInvite_Func);
        GWCA_ASSERT(SetReadyStatus_Func);
        GWCA_ASSERT(FlagAll_Func);
        GWCA_ASSERT(FlagHeroAgent_Func);
#endif

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
        bool Tick(bool flag) {
            // @Robustness: Make sure player isn't already ticked
            if (!SetReadyStatus_Func)
                return false;
            SetReadyStatus_Func(flag);
            return true;
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
            if (!ctx || !ctx->parties.size()) return 0;
            if (!party_id) return ctx->player_party;
            if (party_id >= ctx->parties.size())
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
            auto* p = PartyContext::instance();
            if (!(SetDifficulty_Func && p))
                return false;
            if (p->InHardMode() != flag) {
                SetDifficulty_Func(flag);
            }
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
        bool RespondToPartyRequest(uint32_t party_id, bool accept) {
            // @Robustness: Cycle invitations, make sure the party is found
            if (accept) {
                if (!PartyAcceptInvite_Func)
                    return false;
                PartyAcceptInvite_Func(party_id);
            }
            else {
                if (!PartyRejectInvite_Func)
                    return false;
                PartyRejectInvite_Func(party_id);
            }
            return true;
        }

        bool AddHero(uint32_t heroid) {
            // @Robustness: Make sure player has hero available
            if (!AddHero_Func)
                return false;
            AddHero_Func(heroid);
            return true;
        }

        bool KickHero(uint32_t heroid) {
            // @Robustness: Make sure player's hero is in party
            if (!KickHero_Func)
                return false;
            KickHero_Func(heroid);
            return true;
        }
        bool KickAllHeroes() {
            return KickHero(0x26);
        }
        bool KickPlayer(uint32_t playerid) {
            // @Robustness: Make sure player is in party
            if (!KickPlayer_Func || !GetIsLeader())
                return false;
            KickPlayer_Func(playerid);
            return true;
        }

        bool LeaveParty() {
            if (!LeaveParty_Func)
                return false;
            if(GetPartySize())
                LeaveParty_Func();
            return true;
        }

        bool FlagHero(uint32_t hero_index, GamePos pos) {
            return FlagHeroAgent(Agents::GetHeroAgentID(hero_index), pos);
        }

        bool FlagHeroAgent(AgentID agent_id, GamePos pos) {
            // @Robustness: Make sure player has control of hero agent
            if (!FlagHeroAgent_Func)
                return false;
            if (agent_id == 0) return false;
            if (agent_id == Agents::GetPlayerId()) return false;
            FlagHeroAgent_Func(agent_id, &pos);
            return true;
        }

        bool FlagAll(GamePos pos) {
            // @Robustness: Make sure player has H/H and is in explorable
            if (!FlagAll_Func)
                return false;
            FlagAll_Func(&pos);
            return true;
        }

        bool UnflagHero(uint32_t hero_index) {
            // @Robustness: Make sure flag is set
            return FlagHero(hero_index, GamePos(HUGE_VALF, HUGE_VALF, 0));
        }

        bool UnflagAll() {
            // @Robustness: Make sure flag is set
            return FlagAll(GamePos(HUGE_VALF, HUGE_VALF, 0));
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