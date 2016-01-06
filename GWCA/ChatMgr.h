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

		struct Channel {
			DWORD id; // unused
			std::wstring name;
			Color_t col_sender;
			Color_t col_message;
		};

		struct Message { // a gw_array
			const wchar_t* message;
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
		const DWORD UNKNOW_TIMESTAMP = -1;

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

		inline void SetTimestampColor(DWORD xrgb_color) {
			timestamp_color_ = xrgb_color & 0x00FFFFFF; // remove alpha
		}

		inline bool ToggleTimeStamp(bool enable) { return (timestamp_enable_ = enable); };
		inline bool ToggleTimeStamp() { return (timestamp_enable_ = !timestamp_enable_); };

		inline void RegisterChannel(std::wstring sender, Color_t col_sender, Color_t col_message, DWORD channel = 0) {
			chatlog_channel[sender] = { channel, sender, col_sender, col_message };
		}
		inline void DeleteChannel(std::wstring sender) { chatlog_channel.erase(sender); };

		inline void RegisterCommand(std::wstring command, Callback_t callback, bool override = true) {
			chatcmd_callbacks[command] = { callback, override };
		}
		inline void DeleteCommand(std::wstring command) { chatcmd_callbacks.erase(command); }

		inline ChatBuffer* GetChatBuffer() { return *(ChatBufferLoca); }

	protected:
		ChatMgr();

	private:
		ChatBuffer **ChatBufferLoca;
		DWORD messageId;
		// The hash are unique to the message & unique to the "chat reload"
		std::map < DWORD, DWORD > hashArray;

		Color_t timestamp_color_;
		bool timestamp_enable_;

		std::map< std::wstring, Channel > chatlog_channel;
		std::map< std::wstring, CallBack > chatcmd_callbacks;

		DWORD timestamp[0x100];

		/* Hook stuff */
		typedef void(__fastcall *ChatLog_t)(MessageInfo*, Message*, DWORD);
		typedef void(__fastcall *ChatCmd_t)(wchar_t*);
		typedef void(__fastcall *WriteBuf_t)(wchar_t*, DWORD);
		typedef void(__fastcall *ReloadChat_t)(DWORD, DWORD, DWORD);
		typedef void(__fastcall *OpenTemplate_t)(DWORD unk, ChatTemplate* info);

		void RestoreHooks() override;

		Hook hk_chatlog_;
		Hook hk_chatcmd_;
		Hook hk_writebuf_;
		Hook hk_reloadchat_;
		Hook hk_opentemplate_;

		ChatLog_t ori_chatlog;
		ChatCmd_t ori_chatcmd;
		WriteBuf_t ori_writebuf;
		ReloadChat_t ori_reloadchat;
		OpenTemplate_t ori_opentemplate;

		static void __fastcall det_chatlog(MessageInfo*, Message*, DWORD);
		static void __fastcall det_chatcmd(wchar_t *_message);
		static void __fastcall det_writebuf(wchar_t *HMessage, DWORD channel);
		static void __fastcall det_realoadchat(DWORD ecx, DWORD edx, DWORD unused);
		static void __fastcall det_opentemplate(DWORD unk, ChatTemplate* info);
	};

}