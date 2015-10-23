#include "GWAPIMgr.h"

GWAPI::GWAPIMgr::GWAPIMgr() :
	gamethread_(*new GameThreadMgr(*this)),
	ctos_(*new CtoSMgr(*this)),
	agents_(*new AgentMgr(*this)),
	items_(*new ItemMgr(*this)),
#ifdef GWAPI_USEDIRECTX
	directx_(*new DirectXMgr(*this)),
#endif
	skillbar_(*new SkillbarMgr(*this)),
	effects_(*new EffectMgr(*this)),
	map_(*new MapMgr(*this)),
	chat_(*new ChatMgr(*this)),
	merchant_(*new MerchantMgr(*this)),
	guild_(*new GuildMgr(*this)),
	fList_(*new FriendListMgr(*this)),
	stoc_(*new StoCMgr(*this)),
	camera_(*new CameraMgr(*this))
{ }

GWAPI::GWAPIMgr::~GWAPIMgr() {
	Gamethread().calls_.clear();

	gamethread_.RestoreHooks();
	ctos_.RestoreHooks();
	agents_.RestoreHooks();
	items_.RestoreHooks();
#ifdef GWAPI_USEDIRECTX
	directx_.RestoreHooks();
#endif
	skillbar_.RestoreHooks();
	effects_.RestoreHooks();
	map_.RestoreHooks();
	chat_.RestoreHooks();
	merchant_.RestoreHooks();
	guild_.RestoreHooks();
	fList_.RestoreHooks();
	stoc_.RestoreHooks();
	camera_.RestoreHooks();

	Sleep(100);
	
	delete &gamethread_;
	delete &ctos_;
	delete &agents_;
	delete &items_;
#ifdef GWAPI_USEDIRECTX
	delete &directx_;
#endif
	delete &skillbar_;
	delete &effects_;
	delete &map_;
	delete &chat_;
	delete &merchant_;
	delete &guild_;
	delete &fList_;
	delete &stoc_;
	delete &camera_;
}
