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
		friend class ChatCommandMgr;
		friend class FriendListMgr;
		friend class CameraMgr;
		friend class ChatLogMgr;

		GameThreadMgr* gamethread_;
		CtoSMgr* ctos_;
		AgentMgr* agents_;
		ItemMgr* items_;
		SkillbarMgr* skillbar_;
		EffectMgr* effects_;
		MapMgr* map_;
		ChatMgr* chat_;
		ChatCommandMgr* chatcommands_;
		ChatLogMgr* chatlog_;
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
		inline ChatCommandMgr* ChatCommands() const { return chatcommands_; }
		inline FriendListMgr* FriendList() const { return fList_; }
		inline CameraMgr* Camera() const { return camera_; }
		inline ChatLogMgr* ChatLog() const { return chatlog_; }

#ifdef GWAPI_USEDIRECTX
		inline DirectXMgr* DirectX() const { return directx_; }
#endif
		inline StoCMgr* StoC() const { return stoc_; }

	};

}