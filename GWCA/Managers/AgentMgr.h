#pragma once

#include <Windows.h>
#include <vector>

#include "MemoryMgr.h"
#include <GWCA\Utilities\Hooker.h>
#include <GWCA\Constants\Constants.h>
#include <GWCA\GameEntities\Position.h>
#include <GWCA\GameEntities\NPC.h>
#include <GWCA\GameEntities\Player.h>
#include <GWCA\GameEntities\Agent.h>

namespace GW {
	namespace Agents {
		void Initialize();

		// === Dialogs ===
		// Same as pressing button (id) while talking to an NPC.
		void Dialog(DWORD id);

		void SetupLastDialogHook();
		void RestoreLastDialogHook();

		// Returns last dialog id sent to the server. Requires the hook.
		DWORD GetLastDialogId();

		inline void RestoreHooks() { RestoreLastDialogHook(); }

		// === Agent Array ===
		// Get Current AgentID's of player or target.
		DWORD GetPlayerId();
		DWORD GetTargetId();
		DWORD GetMouseoverId();

		// Returns Agentstruct Array of agents in compass range, full structs.
		GW::AgentArray GetAgentArray();

		// Get AgentArray Structures of player or target.
		Agent* GetAgentByID(DWORD id);
		inline Agent* GetPlayer() { return GetAgentByID(GetPlayerId()); }
		inline Agent* GetTarget() { return GetAgentByID(GetTargetId()); }

		// Returns array of alternate agent array that can be read beyond compass range.
		// Holds limited info and needs to be explored more.
		GW::MapAgentArray GetMapAgentArray();

		// === Other Arrays ===
		GW::PlayerArray GetPlayerArray();

		GW::NPCArray GetNPCArray();
		inline GW::NPC& GetNPCByID(DWORD id) { return GetNPCArray()[id]; }

		// Computes distance between the two agents in game units
		float GetDistance(Vector2f a, const Vector2f b);

		// Computes squared distance between the two agents in game units
		float GetSqrDistance(Vector2f a, const Vector2f b);

		// Change targeted agent to (Agent)
		void ChangeTarget(GW::AgentID agentid);
		inline void ChangeTarget(GW::Agent* agent) { if (agent) ChangeTarget(agent->Id); }

		// Move to specified coordinates.
		// Note: will do nothing if coordinate is outside the map!
		void Move(float X, float Y, DWORD ZPlane = 0);
		void Move(const GW::GamePos& pos);

		// Go to an NPC and begin interaction.
		void GoNPC(GW::Agent* Agent, DWORD CallTarget = 0);

		// Walk to a player.
		void GoPlayer(GW::Agent* Agent);

		// Go to a chest/signpost (yellow nametag) specified by (Agent).
		// Also sets agent as your open chest target.
		void GoSignpost(GW::Agent* Agent, BOOL CallTarget = 0);

		// Call target of specified agent without interacting with the agent.
		void CallTarget(GW::Agent* Agent);

		// Uses size of player array. Needs testing.
		DWORD GetAmountOfPlayersInInstance();

		// Returns name of player with selected loginnumber.
		wchar_t* GetPlayerNameByLoginNumber(DWORD loginnumber);

		// Returns AgentID of player with selected loginnumber.
		DWORD GetAgentIdByLoginNumber(DWORD loginnumber);

		GW::AgentID GetHeroAgentID(DWORD heroindex);

		std::wstring GetAgentName(GW::Agent *Agent);
	};
}
