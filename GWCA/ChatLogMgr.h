#pragma once

#include <Windows.h>
#include <string>
#include "GWAPIMgr.h"

namespace GWAPI {

	class ChatLogMgr {

		ChatLogMgr(GWAPIMgr* parent);

	public:
		void setColor(DWORD rgbColor);

	private:
		friend class GWAPIMgr;
		GWAPIMgr* parent_;

		std::wstring buffer;
		WCHAR color[7];

		typedef void(__fastcall *ChatLog_t)(DWORD, DWORD, DWORD);
		ChatLog_t ChatLogOri_;
		Hook hk_chatLog_;
		bool hooked_;

		void CreateHook(BYTE *detour);
		void RestoreHook();
		static void __fastcall NewChatLog(DWORD, DWORD, DWORD);
	};

}