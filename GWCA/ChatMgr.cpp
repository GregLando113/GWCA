#include "ChatMgr.h"

#include "MemoryMgr.h"
#include "CtoSMgr.h"
#include "PatternScanner.h"
#include <sstream>

GWAPI::ChatMgr::ChatMgr(GWAPIMgr* parent) : parent_(parent)
{
	BeginHook((BYTE*)det_chatlog, (BYTE*)det_chatcmd);
	SetColor(0xff00); // green
}

void GWAPI::ChatMgr::BeginHook(BYTE* chatlog, BYTE* chatcmd)
{
	PatternScanner scanner(0x401000, 0x4FF000);
	BYTE* chatlog_adr = (BYTE*)scanner.FindPattern("\x53\x56\x8B\xF1\x57\x8B\x56\x14\x8B\x4E\x0C\xE8", "xxxxxxxxxxxx", -6);
	BYTE* chatcmd_adr = (BYTE*)scanner.FindPattern("\x8B\xD1\x68\x8A\x00\x00\x00\x8D\x8D\xE8\xFE\xFF\xFF", "xxxxxxxxxxxxx", -0xC);

	DWORD chatlog_length = Hook::CalculateDetourLength(chatlog_adr);
	DWORD chatcmd_length = Hook::CalculateDetourLength(chatcmd_adr);

	ori_chatlog = (ChatLog_t)hk_chatlog_.Detour(chatlog_adr, chatlog, chatlog_length);
	ori_chatcmd = (ChatCmd_t)hk_chatcmd_.Detour(chatcmd_adr, chatcmd, chatcmd_length);

	hooked_ = true;
}

void GWAPI::ChatMgr::EndHook()
{
	if (!hooked_) return;
	hk_chatlog_.Retour();
	hk_chatcmd_.Retour();

	hooked_ = false;
}

void GWAPI::ChatMgr::SendChat(const wchar_t* msg, wchar_t channel)
{
	static P5E_SendChat* chat = new P5E_SendChat();

	chat->channel = channel;
	wcscpy_s(chat->msg, msg);

	parent_->CtoS()->SendPacket<P5E_SendChat>(chat);
}

void GWAPI::ChatMgr::WriteChatF(const wchar_t* format, ...)
{
	static wchar_t* chat = NULL;


	if (chat != NULL) delete[] chat;

	va_list vl;
	va_start(vl, format);
	size_t szbuf = _vscwprintf(format, vl) + 1;
	chat = new wchar_t[szbuf];
	vswprintf_s(chat, szbuf, format, vl);
	va_end(vl);

	WriteChat(chat);
}

void GWAPI::ChatMgr::WriteChat(const wchar_t* msg, const wchar_t* from) {

	((void(__fastcall *)(DWORD, const wchar_t*, const wchar_t*))
		MemoryMgr::WriteChatFunction)
		(0, from, msg);
}

void GWAPI::ChatMgr::SetColor(DWORD rgb_color)
{
#define __itoc(number) (WCHAR)((number) < 0xA ? ((number) + '0') : ((number) + 'A' - 0xA))
	DWORD red = (rgb_color & 0x00ff0000) >> 16;
	DWORD green = (rgb_color & 0x0000ff00) >> 8;
	DWORD blue = rgb_color & 0x000000ff;

	WCHAR buffer[] = {
		__itoc(red / 0x10), __itoc(red % 0x10),
		__itoc(green / 0x10), __itoc(green % 0x10),
		__itoc(blue / 0x10), __itoc(blue % 0x10), '\0' };

	memcpy(chatlog_prefix_color, buffer, 14); /*14 = 7 * sizeof(WCHAR) */
#undef __itoc
}

void GWAPI::ChatMgr::RegisterKey(std::wstring key, CallBack_t callback, bool override)
{
	chatcmd_callbacks[key] = std::tuple<CallBack_t, bool>(callback, override);
}

#define COLORS(color, str) L"<c=#" << (color) << L">" << (str) << L"</c>"
void __fastcall GWAPI::ChatMgr::det_chatlog(DWORD ecx, DWORD edx, DWORD useless /* same as edx */)
{
	GWAPI::ChatMgr *chat = GWAPI::GWAPIMgr::instance()->Chat();
	MessageInfo *mInfo = reinterpret_cast<MessageInfo*>(edx);
	ChannelInfo *cInfo = reinterpret_cast<ChannelInfo*>(ecx);
	std::wostringstream stream;

	stream << COLORS(chat->chatlog_prefix_color, L"[20:30] ") << mInfo->message;
	DWORD length = stream.str().length() + 1;
	chat->chatlog_result = stream.str(); // String should free memory for old string

	mInfo->message = (WCHAR*)chat->chatlog_result.c_str();
	mInfo->size1 = (mInfo->size2 = length);

	return chat->ori_chatlog(ecx, edx, useless);
}
#undef COLOR

void __fastcall GWAPI::ChatMgr::det_chatcmd(DWORD ecx)
{
	ChatMgr* chat = GWAPI::GWAPIMgr::instance()->Chat();
	WCHAR* message = reinterpret_cast<WCHAR*>(ecx);
	WCHAR channel = *message;

	std::wstring key(message + 1);
	std::wstring arguments(L"");

	DWORD fPos = key.find_first_of(' ');
	if (fPos != std::wstring::npos)
	{
		key = key.substr(0, fPos);
		arguments = std::wstring(message + fPos + 2);
	}

	std::tuple<CallBack_t, bool> callback = chat->chatcmd_callbacks[key];
	if (std::get<0>(callback) && channel == '/')
	{
		std::get<0>(callback)(arguments);

		if (std::get<1>(callback))
			return chat->ori_chatcmd((DWORD)L"");
	}

	return chat->ori_chatcmd(ecx);
}
