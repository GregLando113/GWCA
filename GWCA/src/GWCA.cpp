#include "..\GWCA.h"

#include "..\Managers\MemoryMgr.h"
#include "..\Managers\GameThreadMgr.h"
#include "..\Managers\CtoSMgr.h"
#include "..\Managers\AgentMgr.h"
#include "..\Managers\PartyMgr.h"
#include "..\Managers\ItemMgr.h"
#include "..\Managers\SkillbarMgr.h"
#include "..\Managers\EffectMgr.h"
#include "..\Managers\MapMgr.h"
#include "..\Managers\ChatMgr.h"
#include "..\Managers\MerchantMgr.h"
#include "..\Managers\GuildMgr.h"
#include "..\Managers\FriendListMgr.h"
#include "..\Managers\StoCMgr.h"
#include "..\Managers\CameraMgr.h"
#include "..\Managers\GWCAManager.h"

std::vector<GWCA::GWCABaseManager*> GWCA::Api::managers;

bool GWCA::Api::Initialize() {
	if (MemoryMgr::Scan()) {

		// force the construction of at least gamethread and ctos
		Gamethread();
		CtoS();

		return true;
	} else {
		return false;
	}
}

void GWCA::Api::Destruct() {
	GWCA::Gamethread().calls_.clear();
	for (GWCABaseManager* manager : managers) {
		manager->RestoreHooks();
	}
	Sleep(100);
	for (GWCABaseManager* manager : managers) {
		delete manager;
	}
}

// GWCA Module Accessors.
GWCA::GameThreadMgr&	GWCA::Gamethread() { return GameThreadMgr::Instance(); }
GWCA::CtoSMgr&			GWCA::CtoS() { return CtoSMgr::Instance(); }
GWCA::StoCMgr&			GWCA::StoC() { return StoCMgr::Instance(); }
GWCA::AgentMgr&			GWCA::Agents() { return AgentMgr::Instance(); }
GWCA::PartyMgr&			GWCA::Party() { return PartyMgr::Instance(); }
GWCA::ItemMgr&			GWCA::Items() { return ItemMgr::Instance(); }
GWCA::SkillbarMgr&		GWCA::Skillbar() { return SkillbarMgr::Instance(); }
GWCA::EffectMgr&		GWCA::Effects() { return EffectMgr::Instance(); }
GWCA::ChatMgr&			GWCA::Chat() { return ChatMgr::Instance(); }
GWCA::MerchantMgr&		GWCA::Merchant() { return MerchantMgr::Instance(); }
GWCA::GuildMgr&			GWCA::Guild() { return GuildMgr::Instance(); }
GWCA::MapMgr&			GWCA::Map() { return MapMgr::Instance(); }
GWCA::FriendListMgr&	GWCA::FriendList() { return FriendListMgr::Instance(); }
GWCA::CameraMgr&		GWCA::Camera() { return CameraMgr::Instance(); }

