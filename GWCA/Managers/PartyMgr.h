#pragma once

namespace GW {
    struct PartyInfo;

    namespace PartyMgr {

        // set or unset the fact that ticking will work as a toggle instead
        // of showing a drop-down menu
        GWCA_API void SetTickToggle();
        GWCA_API void RestoreTickToggle();

        inline void RestoreHooks() { RestoreTickToggle(); }

        // Set party ready status.
        GWCA_API void Tick(bool flag = true);

        GWCA_API GW::PartyInfo* GetPartyInfo();

        GWCA_API DWORD GetPartySize();
        GWCA_API DWORD GetPartyPlayerCount();
        GWCA_API DWORD GetPartyHeroCount();
        GWCA_API DWORD GetPartyHenchmanCount();

        GWCA_API bool GetIsPartyDefeated();

        GWCA_API void SetHardMode(bool flag);
        GWCA_API bool GetIsPartyInHardMode();

        // check if the whole party is ticked
        GWCA_API bool GetIsPartyTicked();

        // check if selected party member is ticked
        GWCA_API bool GetIsTicked(DWORD player_index);

        // check if the player is ticked
        GWCA_API bool GetIsPlayerTicked();

        // check if the whole party is loaded
        GWCA_API bool GetIsPartyLoaded();

        // returns if the player agent is leader
        GWCA_API bool GetPlayerIsLeader();

        GWCA_API void RespondToPartyRequest(bool accept);

        GWCA_API void LeaveParty();

        // hero managment
        GWCA_API void AddHero(DWORD heroid);
        GWCA_API void KickHero(DWORD heroid);
        GWCA_API void KickAllHeroes();

        // hero flagging
        GWCA_API void FlagHero(DWORD hero_index, GW::GamePos pos);
        GWCA_API void FlagHeroAgent(GW::AgentID agent_id, GW::GamePos pos);
        GWCA_API void UnflagHero(DWORD hero_index);

        GWCA_API void FlagAll(GW::GamePos pos);
        GWCA_API void UnflagAll();
    };
}
