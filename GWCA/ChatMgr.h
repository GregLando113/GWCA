#pragma once
#include <string>
#include <functional>
#include <vector>
#include <map>

#include "GWCAManager.h"
#include "Hooker.h"

namespace GWAPI {

	class ChatMgr : public GWCAManager {
		friend class GWAPIMgr;

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

		struct MessageInfo {
			const wchar_t* message;
			DWORD size1;
			DWORD size2;
			DWORD unknow;
		};

		struct ChannelInfo {
			DWORD unknow1;
			DWORD channel;
			DWORD isHandled; // seem to be 1 until he is handled
			BYTE unknow2[12];
			DWORD unknow3; // alway 6
			DWORD unknow4;
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
		}

		// Write to chat as a PM with printf style arguments.
		void WriteChatF(const wchar_t* from, const wchar_t* format, ...);

		// Simple write to chat as a PM
		void WriteChat(const wchar_t* from, const wchar_t* msg);

		inline void SetTimestampColor(DWORD xrgb_color) {
			timestamp_color_ = xrgb_color && 0x00FFFFFF; // remove alpha
		}

		inline void RegisterChannel(std::wstring sender, Color_t col_sender, Color_t col_message, DWORD channel = 0) {
			chatlog_channel[sender] = { channel, sender, col_sender, col_message };
		}
		inline void DeleteChannel(std::wstring sender) { chatlog_channel.erase(sender); };

		inline void RegisterCommand(std::wstring command, Callback_t callback, bool override = true) {
			chatcmd_callbacks[command] = { callback, override };
		}
		inline void DeleteCommand(std::wstring command) { chatcmd_callbacks.erase(command); }

	private:
		Color_t timestamp_color_;

		std::map< std::wstring, Channel > chatlog_channel;
		std::map< std::wstring, CallBack > chatcmd_callbacks;

		/* Hook stuff */
		typedef void(__fastcall *ChatLog_t)(DWORD, DWORD, DWORD);
		typedef void(__fastcall *ChatCmd_t)(wchar_t*);

		ChatMgr(GWAPIMgr& api);
		void RestoreHooks() override;

		Hook hk_chatlog_;
		Hook hk_chatcmd_;
		ChatLog_t ori_chatlog;
		ChatCmd_t ori_chatcmd;

		static void __fastcall det_chatlog(DWORD, DWORD, DWORD);
		static void __fastcall det_chatcmd(wchar_t *_message);
	};

}