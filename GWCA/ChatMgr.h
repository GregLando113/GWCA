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
		typedef std::function<void(std::vector<std::wstring>)> Callback_t;
		typedef std::function<std::wstring(std::wstring)> ParseMessage_t;
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
			const DWORD id;  // 0, default
			Channel* next;
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

		std::wstring CreateChannel(ParseMessage_t parser);
		std::wstring CreateChannel(std::wstring format_string);

		// Write to chat as a PM with printf style arguments.
		void WriteChatF(const wchar_t* from, const wchar_t* format, ...);
		
		// Simple write to chat as a PM
		void WriteChat(const wchar_t* from, const wchar_t* msg);

		inline void SetTimestampColor(DWORD xrgb_color) {
			timestamp_color_ = xrgb_color && 0x00FFFFFF; // remove alpha
		}

		inline void RegisterKey(std::wstring key, 
			std::function<void(std::vector<std::wstring>)> callback, 
			bool override = true) { 
			chatcmd_callbacks[key] = { callback, override };
		}
		inline void DeleteKey(std::wstring key) { chatcmd_callbacks.erase(key); }

	private:
		std::wstring chatlog_result;
		Color_t timestamp_color_;

		std::map< std::wstring, CallBack > chatcmd_callbacks;

		std::vector<ParseMessage_t> parsers;

		std::wstring RemakeMessage(const wchar_t* format, const wchar_t* message);
		size_t getChan(const wchar_t* message);

		typedef void(__fastcall *ChatLog_t)(DWORD, DWORD, DWORD);
		typedef void(__fastcall *ChatCmd_t)(DWORD);

		ChatMgr(GWAPIMgr& api);
		void RestoreHooks() override;

		Hook hk_chatlog_;
		Hook hk_chatcmd_;
		ChatLog_t ori_chatlog;
		ChatCmd_t ori_chatcmd;

		static void __fastcall det_chatlog(DWORD, DWORD, DWORD);
		static void __fastcall det_chatcmd(DWORD);
	};

}