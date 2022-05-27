#pragma once
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Player.h>

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>



namespace GW {

    typedef Array<NPC> NPCArray;
    typedef Array<Agent *> AgentArray;
    typedef Array<Player> PlayerArray;
    typedef Array<MapAgent> MapAgentArray;
    typedef Array<AgentInfo> AgentInfoArray;

    struct Module;
    extern Module AgentModule;

    namespace Agents {
        // === Dialogs ===
        // Same as pressing button (id) while talking to an NPC.
        GWCA_API void SendDialog(uint32_t dialog_id);

        // Returns last dialog id sent to the server. Requires the hook.
        GWCA_API uint32_t GetLastDialogId();

        // === Agent Array ===

        // Get Agent ID of currently observed player
        GWCA_API AgentID GetPlayerId();
        // Get Agent ID of current target
        GWCA_API AgentID GetTargetId();
        // Get Agent ID of current hover
        GWCA_API AgentID GetMouseoverId();

        // Returns Agentstruct Array of agents in compass range, full structs.
        GWCA_API AgentArray* GetAgentArray();

        GWCA_API AgentInfoArray* GetAgentInfoArray();

        GWCA_API AgentInfo* GetAgentInfo(AgentID agent_id);

        // Get AgentArray Structures of player or target.
        GWCA_API Agent *GetAgentByID(AgentID id);
        // Get Agent of currently observed player
        inline Agent   *GetPlayer() { return GetAgentByID(GetPlayerId()); }
        // Get Agent of current target
        inline Agent   *GetTarget() { return GetAgentByID(GetTargetId()); }
        

        GWCA_API Agent *GetPlayerByID(PlayerID player_id);

        // Get Agent of current logged in character
        GWCA_API AgentLiving* GetCharacter();

        // Get AgentLiving of currently observed player
        AgentLiving    *GetPlayerAsAgentLiving();

        // Get AgentLiving of current target
        AgentLiving    *GetTargetAsAgentLiving();

        // Returns array of alternate agent array that can be read beyond compass range.
        // Holds limited info and needs to be explored more.
        GWCA_API MapAgentArray* GetMapAgentArray();

        // === Other Arrays ===
        GWCA_API PlayerArray* GetPlayerArray();

        GWCA_API NPCArray* GetNPCArray();
        GWCA_API NPC *GetNPCByID(Constants::NpcID npc_id);

        // Change targeted agent to (Agent)
        GWCA_API bool ChangeTarget(const Agent *agent);
        GWCA_API bool ChangeTarget(AgentID agent_id);

        // Move to specified coordinates.
        // Note: will do nothing if coordinate is outside the map!
        GWCA_API void Move(float x, float y, uint32_t zplane = 0);
        GWCA_API void Move(GamePos pos);

        // Go to an NPC and begin interaction.
        GWCA_API void GoNPC(const Agent *agent, bool call_target = false);

        // Walk to a player.
        GWCA_API void GoPlayer(const Agent *agent);

        // Go to a chest/signpost (yellow nametag) specified by (Agent).
        // Also sets agent as your open chest target.
        GWCA_API void GoSignpost(const Agent *agent, bool call_target = false);

        // Call target of specified agent without interacting with the agent.
        GWCA_API void CallTarget(const Agent *agent);

        // Uses size of player array. Needs testing.
        GWCA_API uint32_t GetAmountOfPlayersInInstance();

        // Returns name of player with selected login_number.
        GWCA_API wchar_t *GetPlayerNameByLoginNumber(PlayerID login_number);

        // Returns AgentID of player with selected login_number.
        GWCA_API AgentID GetAgentIdByLoginNumber(PlayerID login_number);

        GWCA_API AgentID GetHeroAgentID(uint32_t hero_index);

        GWCA_API wchar_t* GetAgentEncName(const Agent* agent);
        GWCA_API wchar_t* GetAgentEncName(AgentID agent_id);

        GWCA_API bool AsyncGetAgentName(const Agent *agent, std::wstring& name);

        typedef HookCallback<uint32_t> DialogCallback;
        GWCA_API void RegisterDialogCallback(
            HookEntry* entry,
            DialogCallback callback);

        GWCA_API void RemoveDialogCallback(
            HookEntry* entry);
    };
}
