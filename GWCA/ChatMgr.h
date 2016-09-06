#pragma once
#include <string>
#include <functional>
#include <vector>
#include <map>

#include "GWCAManager.h"
#include "Hooker.h"

namespace GWCA {

	class ChatMgr : public GWCAManager<ChatMgr> {
		friend class GWCAManager<ChatMgr>;

		typedef DWORD Color_t;
		typedef std::function<const void(std::wstring, std::vector<std::wstring>)> Callback_t;
		typedef std::function<std::wstring(std::wstring)> ParseMessage_t; // unused

		struct CallBack {
			Callback_t callback;
			bool override;
		};

		struct P5E_SendChat {
			const DWORD header = 0x5E;
			wchar_t channel;
			wchar_t msg[137];
			DWORD unk;
		};

		struct Message { // a gw_array
			wchar_t* message;
			DWORD size1;
			DWORD size2;
			DWORD unknow;
		};

		struct MessageInfo {
			DWORD hash;
			DWORD channel;
			DWORD isHandled;
		};

		struct ChatBuffer { // May want to put it in GwStructure
			DWORD current;
			DWORD useless;
			WCHAR *HMessage[0x100];
		};

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
		// Like SendChat but make sure that cmd goes through det_chatcmd
		inline void SendChatCmd(const wchar_t* msg, wchar_t channel) {
			DWORD len = wcslen(msg);
			wchar_t *newMes = new wchar_t[len + 2];
			newMes[0] = channel;
			wcscpy_s(&newMes[1], len + 1, msg);
			det_chatcmd(newMes);
			delete[] newMes;
		}

		// Write to chat as a PM with printf style arguments.
		void WriteChatF(const wchar_t* from, const wchar_t* format, ...);

		// Simple write to chat as a PM
		void WriteChat(const wchar_t* from, const wchar_t* msg);

		inline void RegisterChannel(std::wstring sender, Color_t color) {
			chatlog_channel[sender] = color;
		}
		inline void DeleteChannel(std::wstring sender) { chatlog_channel.erase(sender); };

		inline void RegisterCommand(std::wstring command, Callback_t callback, bool override = true) {
			chatcmd_callbacks[command] = { callback, override };
		}
		inline void DeleteCommand(std::wstring command) { chatcmd_callbacks.erase(command); }

		inline void SetOpenLinks(bool b) { open_links_ = b; }

	protected:
		ChatMgr();

	private:
		std::map< std::wstring, Color_t > chatlog_channel;
		std::map< std::wstring, CallBack > chatcmd_callbacks;

		bool open_links_;

		/* Hook stuff */
		typedef void(__fastcall *ChatLog_t)(MessageInfo*, Message*, DWORD);
		typedef void(__fastcall *ChatCmd_t)(wchar_t*);
		typedef void(__fastcall *OpenTemplate_t)(DWORD unk, ChatTemplate* info);

		void RestoreHooks() override;

		Hook hk_chatlog_;
		Hook hk_chatcmd_;
		Hook hk_opentemplate_;

		ChatLog_t ori_chatlog;
		ChatCmd_t ori_chatcmd;
		OpenTemplate_t ori_opentemplate;

		static void __fastcall det_chatlog(MessageInfo*, Message*, DWORD);
		static void __fastcall det_chatcmd(wchar_t *_message);
		static void __fastcall det_opentemplate(DWORD unk, ChatTemplate* info);
	};
}
