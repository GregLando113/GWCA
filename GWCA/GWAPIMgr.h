#pragma  once
#include <Windows.h>
#include "APIMain.h"
#include <exception>

namespace GWAPI {

	typedef BYTE APIException_t;
	const APIException_t API_EXCEPTION = 1;

	class GWAPIMgr {
	public:

		static bool Initialize();
		static GWAPIMgr* instance();
		static void Destruct();

	private:

		static bool init_sucessful_;
		static GWAPIMgr* instance_;

		friend class GameThreadMgr;
		friend class CtoSMgr;
		friend class AgentMgr;
		friend class ItemMgr;
#ifdef GWAPI_USEDIRECTX
		friend class DirectXMgr;
#endif
		friend class SkillbarMgr;
		friend class EffectMgr;
		friend class MapMgr;
		friend class ChatMgr;
		friend class MerchantMgr;
		friend class GuildMgr;
		friend class StoCMgr;
		friend class FriendListMgr;
		friend class CameraMgr;

		GameThreadMgr* gamethread_;
		CtoSMgr* ctos_;
		AgentMgr* agents_;
		ItemMgr* items_;
		SkillbarMgr* skillbar_;
		EffectMgr* effects_;
		MapMgr* map_;
		ChatMgr* chat_;
		MerchantMgr* merchant_;
		GuildMgr* guild_;
		FriendListMgr* fList_;
#ifdef GWAPI_USEDIRECTX
		DirectXMgr* directx_;
#endif
		StoCMgr* stoc_;
		CameraMgr* camera_;
		
		GWAPIMgr();
		~GWAPIMgr();

	public:

		// Module Accessors.
		inline GameThreadMgr* Gamethread() const { return gamethread_; }
		inline CtoSMgr* CtoS() const { return ctos_; }
		inline AgentMgr* Agents() const { return agents_; }
		inline ItemMgr* Items() const { return items_; }
		inline SkillbarMgr* Skillbar() const { return skillbar_; }
		inline EffectMgr* Effects() const { return effects_; }
		inline ChatMgr* Chat() const { return chat_; }
		inline MerchantMgr* Merchant() const { return merchant_; }
		inline GuildMgr* Guild() const { return guild_; }
		inline MapMgr* Map() const { return map_; }
		inline FriendListMgr* FriendList() const { return fList_; }
		inline CameraMgr* Camera() const { return camera_; }

#ifdef GWAPI_USEDIRECTX
		inline DirectXMgr* DirectX() const { return directx_; }
#endif
		inline StoCMgr* StoC() const { return stoc_; }

	};



	/* 
	   Accessor class that should become standardized.
	   Will allow for thread-safe operations as well as safe access (safe access needs work :P)
	*/
	class GWCA {
	public:

		/* Simple accessor that will wait infinitely for other threads to relinquish api use. */
		GWCA();

		/* Waits specified milliseconds for object to be free, will throw API_EXCEPTION if this does not occur. */
		GWCA(DWORD mutex_timeout);

		/* Destructor relinquishes mutex ownership and allows other thread to use GWCA++ */
		~GWCA();

		/* Module Accessor */
		GWAPIMgr* operator->() { return api_; }

		/* Optional to use these instead of GWAPIMgr::Initialize() */
		inline static bool Initialize() { return GWAPIMgr::Initialize(); }
		inline static void Destruct() { return GWAPIMgr::Destruct(); }


		static const DWORD API_NO_OWNERSHIP;
	private:
		GWAPIMgr* api_;
		static HANDLE mutex_;
	};

}