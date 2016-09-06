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
#include "..\Managers\GWCAManager.h"

std::vector<GW::GWCABaseManager*> GW::Api::managers;

bool GW::Api::Initialize() {
	if (MemoryMgr::Scan()) {

		// force the construction of at least gamethread and ctos
		Gamethread();
		CtoS();

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
	Sleep(100);
	for (GWCABaseManager* manager : managers) {
		delete manager;
	}
}

// GWCA Module Accessors.
GW::GameThreadMgr&	GW::Gamethread() { return GameThreadMgr::Instance(); }
GW::CtoSMgr&		GW::CtoS() { return CtoSMgr::Instance(); }
GW::StoCMgr&		GW::StoC() { return StoCMgr::Instance(); }
GW::AgentMgr&		GW::Agents() { return AgentMgr::Instance(); }
GW::PartyMgr&		GW::Partymgr() { return PartyMgr::Instance(); }
GW::ItemMgr&		GW::Items() { return ItemMgr::Instance(); }
GW::SkillbarMgr&	GW::Skillbarmgr() { return SkillbarMgr::Instance(); }
GW::EffectMgr&		GW::Effects() { return EffectMgr::Instance(); }
GW::ChatMgr&		GW::Chat() { return ChatMgr::Instance(); }
GW::MerchantMgr&	GW::Merchant() { return MerchantMgr::Instance(); }
GW::GuildMgr&		GW::Guildmgr() { return GuildMgr::Instance(); }
GW::MapMgr&			GW::Map() { return MapMgr::Instance(); }
GW::FriendListMgr&	GW::FriendListmgr() { return FriendListMgr::Instance(); }
GW::CameraMgr&		GW::Cameramgr() { return CameraMgr::Instance(); }
GW::PlayerMgr&		GW::Playermgr() { return PlayerMgr::Instance(); }
