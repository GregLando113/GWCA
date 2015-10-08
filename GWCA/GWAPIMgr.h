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
		friend class CommandMgr;

		GameThreadMgr* gamethread_;
		CtoSMgr* ctos_;
		AgentMgr* agents_;
		ItemMgr* items_;
		SkillbarMgr* skillbar_;
		EffectMgr* effects_;
		MapMgr* map_;
		ChatMgr* chat_;
		CommandMgr* command_;
		MerchantMgr* merchant_;
		GuildMgr* guild_;
#ifdef GWAPI_USEDIRECTX
		DirectXMgr* directx_;
#endif
		
		GWAPIMgr();
		~GWAPIMgr();

	public:

		// Module Accessors.
		GameThreadMgr* Gamethread() const { return gamethread_; }
		CtoSMgr* CtoS() const { return ctos_; }
		AgentMgr* Agents() const { return agents_; }
		ItemMgr* Items() const { return items_; }
		SkillbarMgr* Skillbar() const { return skillbar_; }
		EffectMgr* Effects() const { return effects_; }
		ChatMgr* Chat() const { return chat_; }
		CommandMgr* Command() const { return command_; }
		MerchantMgr* Merchant() const { return merchant_; }
		GuildMgr* Guild() const { return guild_; }
		MapMgr* Map() const { return map_; }
#ifdef GWAPI_USEDIRECTX
		DirectXMgr* DirectX() const { return directx_; }
#endif

	};

}