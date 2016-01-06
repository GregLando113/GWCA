#include "GWCA.h"

#include "MemoryMgr.h"
#include "GameThreadMgr.h"
#include "CtoSMgr.h"
#include "AgentMgr.h"
#include "ItemMgr.h"
#ifdef GWAPI_USEDIRECTX
#include "DirectXMgr.h"
#endif
#include "SkillbarMgr.h"
#include "EffectMgr.h"
#include "MapMgr.h"
#include "ChatMgr.h"
#include "MerchantMgr.h"
#include "GuildMgr.h"
#include "FriendListMgr.h"
#include "StoCMgr.h"
#include "CameraMgr.h"
#include "GWCAManager.h"

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
	Gamethread().calls_.clear();
	for (GWCABaseManager* manager : managers) {
		manager->RestoreHooks();
	}
	Sleep(100);
	for (GWCABaseManager* manager : managers) {
		delete manager;
	}
}

// Module Accessors.
GWCA::GameThreadMgr&	GWCA::Api::Gamethread()	{ return GameThreadMgr::Instance(); }
GWCA::CtoSMgr&			GWCA::Api::CtoS()		{ return CtoSMgr::Instance(); }
GWCA::StoCMgr&			GWCA::Api::StoC()		{ return StoCMgr::Instance(); }
GWCA::AgentMgr&			GWCA::Api::Agents()		{ return AgentMgr::Instance(); }
GWCA::ItemMgr&			GWCA::Api::Items()		{ return ItemMgr::Instance(); }
GWCA::SkillbarMgr&		GWCA::Api::Skillbar()	{ return SkillbarMgr::Instance(); }
GWCA::EffectMgr&		GWCA::Api::Effects()	{ return EffectMgr::Instance(); }
GWCA::ChatMgr&			GWCA::Api::Chat()		{ return ChatMgr::Instance(); }
GWCA::MerchantMgr&		GWCA::Api::Merchant()	{ return MerchantMgr::Instance(); }
GWCA::GuildMgr&			GWCA::Api::Guild()		{ return GuildMgr::Instance(); }
GWCA::MapMgr&			GWCA::Api::Map()		{ return MapMgr::Instance(); }
GWCA::FriendListMgr&	GWCA::Api::FriendList()	{ return FriendListMgr::Instance(); }
GWCA::CameraMgr&		GWCA::Api::Camera()		{ return CameraMgr::Instance(); }
#ifdef GWAPI_USEDIRECTX
GWCA::DirectXMgr&		GWCA::Api::DirectX()	{ return DirectXMgr::Instance(); }
#endif
