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
#include "..\Managers\PlayerMgr.h"
#include "..\Managers\TradeMgr.h"
#include "..\Managers\GWCAManager.h"

std::vector<GW::GWCABaseManager*> GW::Api::managers;

bool GW::Api::Initialize() {
	if (MemoryMgr::Scan()) {

		// force the construction of at least gamethread and ctos
		Gamethread();

		return true;
	} else {
		return false;
	}
}

void GW::Api::Destruct() {
	GW::Gamethread().calls_.clear();
	for (GWCABaseManager* manager : managers) {
		manager->RestoreHooks();
	}
	GW::CameraMgr::RestoreHooks();
	GW::Chat::RestoreHooks();
	GW::PartyMgr::RestoreHooks();
	GW::StoC::RestoreHooks();
	GW::Agents::RestoreHooks();

	Sleep(100);
	for (GWCABaseManager* manager : managers) {
		delete manager;
	}
}

// GWCA Module Accessors.
GW::GameThreadMgr&	GW::Gamethread() { return GameThreadMgr::Instance(); }
GW::SkillbarMgr&	GW::Skillbarmgr() { return SkillbarMgr::Instance(); }
GW::EffectMgr&		GW::Effects() { return EffectMgr::Instance(); }
