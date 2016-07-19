#include "GWCA.h"

#include "MemoryMgr.h"
#include "GameThreadMgr.h"
#include "CtoSMgr.h"
#include "AgentMgr.h"
#include "PartyMgr.h"
#include "ItemMgr.h"
#include "SkillbarMgr.h"
#include "EffectMgr.h"
#include "MapMgr.h"
#include "ChatMgr.h"
#include "MerchantMgr.h"
#include "GuildMgr.h"
#include "FriendListMgr.h"
#include "StoCMgr.h"
#include "CameraMgr.h"
#include "PlayerMgr.h"
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
GWCA::GameThreadMgr&	GWCA::Gamethread()	{ return GameThreadMgr::Instance(); }
GWCA::CtoSMgr&			GWCA::CtoS()		{ return CtoSMgr::Instance(); }
GWCA::StoCMgr&			GWCA::StoC()		{ return StoCMgr::Instance(); }
GWCA::AgentMgr&			GWCA::Agents()		{ return AgentMgr::Instance(); }
GWCA::PartyMgr&			GWCA::Party()		{ return PartyMgr::Instance(); }
GWCA::ItemMgr&			GWCA::Items()		{ return ItemMgr::Instance(); }
GWCA::SkillbarMgr&		GWCA::Skillbar()	{ return SkillbarMgr::Instance(); }
GWCA::EffectMgr&		GWCA::Effects()		{ return EffectMgr::Instance(); }
GWCA::ChatMgr&			GWCA::Chat()		{ return ChatMgr::Instance(); }
GWCA::MerchantMgr&		GWCA::Merchant()	{ return MerchantMgr::Instance(); }
GWCA::GuildMgr&			GWCA::Guild()		{ return GuildMgr::Instance(); }
GWCA::MapMgr&			GWCA::Map()			{ return MapMgr::Instance(); }
GWCA::FriendListMgr&	GWCA::FriendList()	{ return FriendListMgr::Instance(); }
GWCA::CameraMgr&		GWCA::Camera()		{ return CameraMgr::Instance(); }
GWCA::PlayerMgr&		GWCA::Players()		{ return PlayerMgr::Instance(); }

