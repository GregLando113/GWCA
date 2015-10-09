#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include "GWAPIMgr.h"

namespace GWAPI {

	/* ChatCommandMgr class by reduf */
	
	class ChatCommandMgr {

		ChatCommandMgr(GWAPIMgr *obj);
		
	public:
		typedef void(__fastcall *CommandCheck_t)(WCHAR *message);
		typedef void(__fastcall *CallBack_t)(WCHAR *arg);

		void RestoreHook();
		void RegisterKey(std::wstring key, CallBack_t callback, bool override = true);
		void DeleteKey(std::wstring key);

	private:
		friend class GWAPIMgr;
		const GWAPIMgr *parent_;

		CommandCheck_t cmdCheck_;
		Hook hk_cmdCheck_;
		bool hooked_;

		std::map < std::wstring, std::tuple<CallBack_t, bool> > callbacks;

		void CreateHook(BYTE *detour);
		static void __fastcall NewCmdCheck(WCHAR *message);
	};

}