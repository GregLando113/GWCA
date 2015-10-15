#pragma once
#include "GWAPIMgr.h"
#include <string>
#include <functional>
#include <map>


namespace GWAPI {

	class ChatMgr{

		ChatMgr(GWAPIMgr* parent);

		typedef std::function<void(std::wstring)> CB_T;
		struct CallBack {
			CB_T callback; // 0, default
			bool override;
		};

		struct P5E_SendChat {
			const DWORD header = 0x5E;
			wchar_t channel;
			wchar_t msg[137];
			DWORD unk;
		};

		struct Channel {
			const DWORD id;
		};

	public:
		
		typedef unsigned int CHAT_COLOR;

		// Sendchat, should be self explanatory. SendChat(L"I love gwtoolbox",L'!');
		void SendChat(const wchar_t* msg, wchar_t channel);

		// Write to chat as a PM with printf style arguments.
		void WriteChatF(const wchar_t* format, ...);
		
		// Simple write to chat as a PM
		void WriteChat(const wchar_t* msg, const wchar_t* from = L"GWToolbox++");

		inline void SetColor(DWORD rgb_color) { timestamp_color = rgb_color; }
		inline void RegisterKey(std::wstring key, CB_T callback, bool override = true) { chatcmd_callbacks[key] = { callback, override }; }
		inline void DeleteKey(std::wstring key) { chatcmd_callbacks.erase(key); }
		inline void RestoreHook() { EndHook(); }

	private:
		friend class GWAPIMgr;
		GWAPIMgr* const parent_;

		std::wstring chatlog_result;
		CHAT_COLOR timestamp_color;

		std::map< std::wstring, CallBack > chatcmd_callbacks;

		std::wstring RemakeMessage(const wchar_t* format, ...);
		DWORD getChan(wchar_t* message);

		typedef void(__fastcall *ChatLog_t)(DWORD, DWORD, DWORD);
		typedef void(__fastcall *ChatCmd_t)(DWORD);

		void BeginHook(BYTE*, BYTE*);
		void EndHook();
		bool hooked_;

		Hook hk_chatlog_;
		Hook hk_chatcmd_;
		ChatLog_t ori_chatlog;
		ChatCmd_t ori_chatcmd;

		static void __fastcall det_chatlog(DWORD, DWORD, DWORD);
		static void __fastcall det_chatcmd(DWORD);
	};

}