#pragma once

namespace GW {
    struct Agent;

    namespace Agents {
        GWCA_API void Initialize();

        // === Dialogs ===
        // Same as pressing button (id) while talking to an NPC.
        GWCA_API void Dialog(DWORD id);

        GWCA_API void SetupLastDialogHook();
        GWCA_API void RestoreLastDialogHook();

        // Returns last dialog id sent to the server. Requires the hook.
        GWCA_API DWORD GetLastDialogId();

        inline void RestoreHooks() { RestoreLastDialogHook(); }

        // === Agent Array ===
        // Get Current AgentID's of player or target.
        GWCA_API DWORD GetPlayerId();
        GWCA_API DWORD GetTargetId();
        GWCA_API DWORD GetMouseoverId();

        // Returns Agentstruct Array of agents in compass range, full structs.
        GWCA_API GW::AgentArray GetAgentArray();

        // Get AgentArray Structures of player or target.
        GWCA_API Agent* GetAgentByID(DWORD id);
        inline Agent* GetPlayer() { return GetAgentByID(GetPlayerId()); }
        inline Agent* GetTarget() { return GetAgentByID(GetTargetId()); }

        // Returns array of alternate agent array that can be read beyond compass range.
        // Holds limited info and needs to be explored more.
        GWCA_API GW::MapAgentArray GetMapAgentArray();

        // === Other Arrays ===
        GWCA_API GW::PlayerArray GetPlayerArray();

        GWCA_API GW::NPCArray GetNPCArray();
        inline GW::NPC& GetNPCByID(DWORD id) { return GetNPCArray()[id]; }

        // Computes distance between the two agents in game units
        GWCA_API float GetDistance(Vector2f a, const Vector2f b);

        // Computes squared distance between the two agents in game units
        GWCA_API float GetSqrDistance(Vector2f a, const Vector2f b);

        // Change targeted agent to (Agent)
        GWCA_API void ChangeTarget(GW::AgentID agentid);
        inline void ChangeTarget(GW::Agent* agent) { if (agent) ChangeTarget(agent->agent_id); }

        // Move to specified coordinates.
        // Note: will do nothing if coordinate is outside the map!
        GWCA_API void Move(float X, float Y, DWORD ZPlane = 0);
        GWCA_API void Move(const GW::GamePos& pos);

        // Go to an NPC and begin interaction.
        GWCA_API void GoNPC(GW::Agent* Agent, DWORD CallTarget = 0);

        // Walk to a player.
        GWCA_API void GoPlayer(GW::Agent* Agent);

        // Go to a chest/signpost (yellow nametag) specified by (Agent).
        // Also sets agent as your open chest target.
        GWCA_API void GoSignpost(GW::Agent* Agent, BOOL CallTarget = 0);

        // Call target of specified agent without interacting with the agent.
        GWCA_API void CallTarget(GW::Agent* Agent);

        // Uses size of player array. Needs testing.
        GWCA_API DWORD GetAmountOfPlayersInInstance();

        // Returns name of player with selected loginnumber.
        GWCA_API wchar_t* GetPlayerNameByLoginNumber(DWORD loginnumber);

        // Returns AgentID of player with selected loginnumber.
        GWCA_API DWORD GetAgentIdByLoginNumber(DWORD loginnumber);

        GWCA_API GW::AgentID GetHeroAgentID(DWORD heroindex);

        // Might be bugged, avoid to use.
        GWCA_API std::wstring GetAgentName(GW::Agent *Agent);

        GWCA_API void AsyncGetAgentName(GW::Agent *agent, std::wstring& name);
    };
}
