#include "ChatLogMgr.h"
#include "PatternScanner.h"

GWAPI::ChatLogMgr::ChatLogMgr(GWAPIMgr* parent) : parent_(parent)
{
	CreateHook((BYTE*)NewChatLog);
}

void GWAPI::ChatLogMgr::CreateHook(BYTE *detour)
{
	PatternScanner scanner(0x401000, 0x4FF000);
	BYTE *origin = (BYTE*)scanner.FindPattern("\x53\x56\x8B\xF1\x57\x8B\x56\x14\x8B\x4E\x0C\xE8", "xxxxxxxxxxxx", -6);
	DWORD length = Hook::CalculateDetourLength((BYTE*)ChatLogOri_);

	ChatLogOri_ = (ChatLog_t)hk_chatLog_.Detour(origin, detour, length);
}

void GWAPI::ChatLogMgr::RestoreHook()
{
	if (!hooked_) return;
	hk_chatLog_.Retour();

	hooked_ = false;
}

void __fastcall GWAPI::ChatLogMgr::NewChatLog(DWORD ecx, DWORD edx, DWORD useless /* same as edx */)
{
	GWAPI::ChatLogMgr *chat = GWAPI::GWAPIMgr::instance()->ChatLog();

	return chat->ChatLogOri_(edx, edx, useless);
}
