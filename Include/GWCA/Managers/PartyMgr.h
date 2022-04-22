#pragma once

#include <GWCA/Utilities/Export.h>

namespace GW {
    struct GamePos;
    struct PartyInfo;

    typedef uint32_t AgentID;

    struct Module;
    extern Module PartyModule;

    namespace PartyMgr {

        // set or unset the fact that ticking will work as a toggle instead
        // of showing a drop-down menu

        GWCA_API void SetTickToggle(bool enable);

        // Set party ready status.
        GWCA_API void Tick(bool flag = true);

        GWCA_API PartyInfo *GetPartyInfo(uint32_t party_id = 0);

        GWCA_API uint32_t GetPartySize();
        GWCA_API uint32_t GetPartyPlayerCount();
        GWCA_API uint32_t GetPartyHeroCount();
        GWCA_API uint32_t GetPartyHenchmanCount();

        GWCA_API bool GetIsPartyDefeated();

        GWCA_API void SetHardMode(bool flag);
        GWCA_API bool GetIsPartyInHardMode();

        // check if the whole party is ticked
        GWCA_API bool GetIsPartyTicked();

        // check if selected party member is ticked
        GWCA_API bool GetIsTicked(uint32_t player_index);

        // check if the player is ticked
        GWCA_API bool GetIsPlayerTicked();

        // check if the whole party is loaded
        GWCA_API bool GetIsPartyLoaded();

        // returns if the player agent is leader
        GWCA_API bool GetPlayerIsLeader();

        GWCA_API void RespondToPartyRequest(bool accept);

        GWCA_API void LeaveParty();

        // hero managment
        GWCA_API void AddHero(uint32_t heroid);
        GWCA_API void KickHero(uint32_t heroid);
        GWCA_API void KickAllHeroes();

        // hero flagging
        GWCA_API void FlagHero(uint32_t hero_index, GamePos pos);
        GWCA_API void FlagHeroAgent(AgentID agent_id, GamePos pos);
        GWCA_API void UnflagHero(uint32_t hero_index);

        GWCA_API void FlagAll(GamePos pos);
        GWCA_API void UnflagAll();
    };
}
