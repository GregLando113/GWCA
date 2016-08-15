#pragma once

#include <Windows.h>
#include <vector>

#include "GWCAManager.h"
#include "GWStructures.h"
#include "GwConstants.h"
#include "Hooker.h"

namespace GWCA {

	class AgentMgr : public GWCAManager<AgentMgr> {
		friend class GWCAManager<AgentMgr>;

	public:
		// Get AgentArray Structures of player or target.
		GW::Agent* GetPlayer();
		GW::Agent* GetTarget();
		inline GW::Agent* GetAgentByID(DWORD id) { return GetAgentArray()[id]; }

		// Get Current AgentID's of player or target.
		inline DWORD GetPlayerId() { return *(DWORD*)MemoryMgr::PlayerAgentIDPtr; }
		inline DWORD GetTargetId() { return *(DWORD*)MemoryMgr::TargetAgentIDPtr; }

		// Returns array of alternate agent array that can be read beyond compass range.
		// Holds limited info and needs to be explored more.
		GW::MapAgentArray GetMapAgentArray();

		// Returns Agentstruct Array of agents in compass range, full structs.
		GW::AgentArray GetAgentArray();

		GW::PlayerArray GetPlayerArray();

		GW::NPCArray GetNPCArray();
		inline GW::NPC& GetNPCByID(DWORD id) { return GetNPCArray()[id]; }

		// Computes distance between the two agents in game units
		float GetDistance(const GW::GamePos& a, const GW::GamePos& b);

		// Computes squared distance between the two agents in game units
		float GetSqrDistance(const GW::GamePos& a, const GW::GamePos& b);

		// Change targeted agent to (Agent)
		void ChangeTarget(GW::Agent* Agent);

		// Move to specified coordinates.
		// Note: will do nothing if coordinate is outside the map!
		void Move(float X, float Y, DWORD ZPlane = 0);

		void Move(const GW::GamePos& pos);

		// Same as pressing button (id) while talking to an NPC.
		void Dialog(DWORD id);

		// Go to an NPC and begin interaction.
		void GoNPC(GW::Agent* Agent, DWORD CallTarget = 0);

		// Walk to a player.
		void GoPlayer(GW::Agent* Agent);

		// Go to a chest/signpost (yellow nametag) specified by (Agent).
		// Also sets agent as your open chest target.
		void GoSignpost(GW::Agent* Agent, BOOL CallTarget = 0);

		// Call target of specified agent without interacting with the agent.
		void CallTarget(GW::Agent* Agent);

		// Returns last dialog id sent to the server.
		DWORD GetLastDialogId() const { return last_dialog_id_; }

		// Uses size of player array. Needs testing.
		DWORD GetAmountOfPlayersInInstance();

		// Returns name of player with selected loginnumber.
		wchar_t* GetPlayerNameByLoginNumber(DWORD loginnumber);

		// Returns AgentID of player with selected loginnumber.
		DWORD GetAgentIdByLoginNumber(DWORD loginnumber);

		// Returns the profession as short string, e.g. "W" for warrior
		const char* GetProfessionAcronym(GwConstants::Profession profession);

		GW::AgentID GetHeroAgentID(DWORD heroindex);

	private:
		
		typedef void(__fastcall *ChangeTarget_t)(DWORD AgentID,DWORD smth);

		typedef void(__fastcall *Move_t)(GW::GamePos* Pos);

		AgentMgr();

		void RestoreHooks() override;

		ChangeTarget_t change_target_;
		Move_t move_;

		Hook hk_dialog_log_;

		static BYTE* dialog_log_ret_;
		static DWORD last_dialog_id_;

		static void detourDialogLog();
	};
}
