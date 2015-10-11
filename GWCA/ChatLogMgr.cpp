#include "ChatLogMgr.h"
#include "PatternScanner.h"
#include <sstream>

GWAPI::ChatLogMgr::ChatLogMgr(GWAPIMgr* parent) : parent_(parent)
{
	CreateHook((BYTE*)NewChatLog);
}

void GWAPI::ChatLogMgr::CreateHook(BYTE *detour)
{
	PatternScanner scanner(0x401000, 0x4FF000);
	BYTE *origin = (BYTE*)scanner.FindPattern("\x53\x56\x8B\xF1\x57\x8B\x56\x14\x8B\x4E\x0C\xE8", "xxxxxxxxxxxx", -6);
	DWORD length = Hook::CalculateDetourLength(origin);

	ChatLogOri_ = (ChatLog_t)hk_chatLog_.Detour(origin, detour, length);
}

void GWAPI::ChatLogMgr::RestoreHook()
{
	if (!hooked_) return;
	hk_chatLog_.Retour();

	hooked_ = false;
}

struct MessageInfo {
	WCHAR *message;
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

void __fastcall GWAPI::ChatLogMgr::NewChatLog(DWORD ecx, DWORD edx, DWORD useless /* same as edx */)
{
	GWAPI::ChatLogMgr *chat = GWAPI::GWAPIMgr::instance()->ChatLog();
	MessageInfo *mInfo = reinterpret_cast<MessageInfo*>(edx);
	ChannelInfo *cInfo = reinterpret_cast<ChannelInfo*>(ecx);
	std::wostringstream stream;

	stream << L"<c=#ffffff>[20:30]</c> " << mInfo->message;
	DWORD length = stream.str().length() + 1;
	chat->buffer = stream.str(); // String should prevent memory leak

	mInfo->message = (WCHAR*)chat->buffer.c_str();
	mInfo->size1 = (mInfo->size2 = length);

	return chat->ChatLogOri_(ecx, useless, useless);
}
