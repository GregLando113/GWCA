#include "ChatLogMgr.h"
#include "PatternScanner.h"
#include <sstream>

GWAPI::ChatLogMgr::ChatLogMgr(GWAPIMgr* parent) : parent_(parent)
{
	setColor(0x00ff00);
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

void GWAPI::ChatLogMgr::setColor(DWORD rgbColor)
{
#define __itoc(number) (WCHAR)((number) < 0xA ? ((number) + '0') : ((number) + 'A' - 0xA))
	DWORD red = (rgbColor & 0x00ff0000) >> 16;
	DWORD green = (rgbColor & 0x0000ff00) >> 8;
	DWORD blue = rgbColor & 0x000000ff;

	WCHAR buffer[] = {
		__itoc(red / 0x10), __itoc(red % 0x10),
		__itoc(green / 0x10), __itoc(green % 0x10),
		__itoc(blue / 0x10), __itoc(blue % 0x10), '\0'};

	memcpy(color, buffer, 14); /*14 = 7 * sizeof(WCHAR) */
#undef __itoc
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

	stream << L"<c=#" << chat->color << L">" << L"[30:00]" << L"</c> " << mInfo->message;
	DWORD length = stream.str().length() + 1;
	chat->buffer = stream.str(); // String should free memory for old string

	mInfo->message = (WCHAR*)chat->buffer.c_str();
	mInfo->size1 = (mInfo->size2 = length);

	return chat->ChatLogOri_(ecx, edx, useless);
}
