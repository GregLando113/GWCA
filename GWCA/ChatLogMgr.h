#pragma once

#include <Windows.h>
#include <string>
#include "GWAPIMgr.h"

namespace GWAPI {

	class ChatLogMgr {

		ChatLogMgr(GWAPIMgr* parent);

	public:

	private:
		friend class GWAPIMgr;
		GWAPIMgr* parent_;

		typedef void(__fastcall *ChatLog_t)(DWORD, DWORD, DWORD);

		ChatLog_t ChatLogOri_;
		Hook hk_chatLog_;
		bool hooked_;

		void CreateHook(BYTE *detour);
		void RestoreHook();
		static void __fastcall NewChatLog(DWORD, DWORD, DWORD);

		std::wstring buffer;
	};

}