#pragma  once
#include <Windows.h>
#include <exception>

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

namespace GWAPI {

	typedef BYTE APIException_t;
	const APIException_t API_EXCEPTION = 1;

	class GWAPIMgr {
		friend class GWCA;

	public:
		// Module Accessors.
		inline GameThreadMgr& Gamethread() const { return gamethread_; }
		inline CtoSMgr& CtoS() const { return ctos_; }
		inline AgentMgr& Agents() const { return agents_; }
		inline ItemMgr& Items() const { return items_; }
		inline SkillbarMgr& Skillbar() const { return skillbar_; }
		inline EffectMgr& Effects() const { return effects_; }
		inline ChatMgr& Chat() const { return chat_; }
		inline MerchantMgr& Merchant() const { return merchant_; }
		inline GuildMgr& Guild() const { return guild_; }
		inline MapMgr& Map() const { return map_; }
		inline FriendListMgr& FriendList() const { return fList_; }
		inline CameraMgr& Camera() const { return camera_; }
#ifdef GWAPI_USEDIRECTX
		inline DirectXMgr& DirectX() const { return directx_; }
#endif
		inline StoCMgr& StoC() const { return stoc_; }

	private:
		GWAPIMgr();
		~GWAPIMgr();

		GameThreadMgr& gamethread_;
		CtoSMgr& ctos_;
		AgentMgr& agents_;
		ItemMgr& items_;
		SkillbarMgr& skillbar_;
		EffectMgr& effects_;
		MapMgr& map_;
		ChatMgr& chat_;
		MerchantMgr& merchant_;
		GuildMgr& guild_;
		FriendListMgr& fList_;
#ifdef GWAPI_USEDIRECTX
		DirectXMgr& directx_;
#endif
		StoCMgr& stoc_;
		CameraMgr& camera_;
	};
}
