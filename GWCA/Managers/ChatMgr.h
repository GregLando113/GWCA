#pragma once
#include <string>
#include <functional>
#include <vector>
#include <map>

#include "GWCAManager.h"
#include <GWCA\Utilities\Hooker.h>

namespace GW {

	enum Channel {
		CHANNEL_ALLIANCE	= 0,
		CHANNEL_NPC			= 1,
		CHANNEL_GWCA1		= 2,
		CHANNEL_ALL			= 3,
		CHANNEL_GWCA2		= 4,
		CHANNEL_MODERATOR	= 5,
		CHANNEL_GWCA3		= 6,
		CHANNEL_GWCA4		= 7,
		CHANNEL_GWCA5		= 8,
		CHANNEL_GUILD		= 9,
		CHANNEL_GLOBAL		= 10,
		CHANNEL_GROUP		= 11,
		CHANNEL_TRADE		= 12,
		CHANNEL_ADVISORY	= 13,
		CHANNEL_WHISPER		= 14,

		CHANNEL_COUNT
	};

	typedef DWORD Color;

	class ChatMgr : public GWCAManager<ChatMgr> {
		friend class GWCAManager<ChatMgr>;

		struct ChatTemplate {
			DWORD unk1[2];
			wchar_t* template_code; // Is actually part of a gw_array<wchar_t>
			DWORD templatecode_sizealloc; // Is actually part of a gw_array<wchar_t>
			DWORD templatecode_size; // Is actually part of a gw_array<wchar_t>
			DWORD unk_identifier; // Is actually part of a gw_array<wchar_t>
			wchar_t* template_name;
		};

	public:
		// Send a message to an in-game channel (! for all, @ for guild, etc)
		void SendChat(const wchar_t* msg, wchar_t channel);

		// Write to chat as a PM with printf style arguments.
		void WriteChatF(const wchar_t* from, const wchar_t* format, ...);

		// Simple write to chat as a PM
		void WriteChat(const wchar_t* from, const wchar_t* msg);

		inline void RegisterCommand(const String& command, Callback callback) {
			sendchat_callbacks[command] = callback;
		}
		inline void DeleteCommand(const String& command) { sendchat_callbacks.erase(command); }

		inline void SetOpenLinks(bool b) { open_links_ = b; }

		Color SetSenderColor(Channel chan, Color col);
		Color SetMessageColor(Channel chan, Color col);

	protected:
		ChatMgr();

	private:
		typedef std::wstring String;
		typedef std::vector<String> StringArray;
		typedef bool (*Callback)(String& command, StringArray& args);

		std::map< std::wstring, Callback > sendchat_callbacks;
		bool open_links_;

		/* Hook stuff */
		typedef void(__fastcall *SendChat_t)(wchar_t*);
		typedef void(__fastcall *OpenTemplate_t)(DWORD unk, ChatTemplate* info);

		void RestoreHooks() override;

		Hook hk_sendchat_;
		Hook hk_opentemplate_;
		Hook hk_sendercolor_;
		Hook hk_messagecolor_;

		SendChat_t ori_sendchat;
		OpenTemplate_t ori_opentemplate;

		static void __fastcall det_sendchat(wchar_t *_message);
		static void __fastcall det_opentemplate(DWORD unk, ChatTemplate* info);
		static Color* __fastcall det_sendercolor(Color *color, Channel chan);
		static Color* __fastcall det_messagecolor(Color *color, Channel chan);
	};
}
