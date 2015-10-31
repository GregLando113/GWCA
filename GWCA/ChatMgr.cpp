#include "ChatMgr.h"

#include <sstream>
#include <iomanip>

#include "GWCA.h"
#include "PatternScanner.h"


GWAPI::ChatMgr::ChatMgr(GWAPIMgr& api) : GWCAManager(api)
{
	PatternScanner scanner(0x401000, 0x4FF000);
	BYTE* chatlog_adr = (BYTE*)scanner.FindPattern("\x53\x56\x8B\xF1\x57\x8B\x56\x14\x8B\x4E\x0C\xE8", "xxxxxxxxxxxx", -6);
	BYTE* chatcmd_adr = (BYTE*)scanner.FindPattern("\x8B\xD1\x68\x8A\x00\x00\x00\x8D\x8D\xE8\xFE\xFF\xFF", "xxxxxxxxxxxxx", -0xC);

	DWORD chatlog_length = Hook::CalculateDetourLength(chatlog_adr);
	DWORD chatcmd_length = Hook::CalculateDetourLength(chatcmd_adr);

	ori_chatlog = (ChatLog_t)hk_chatlog_.Detour(chatlog_adr, (BYTE*)det_chatlog, chatlog_length);
	ori_chatcmd = (ChatCmd_t)hk_chatcmd_.Detour(chatcmd_adr, (BYTE*)det_chatcmd, chatcmd_length);

	SetTimestampColor(0xff00); // green
}

void GWAPI::ChatMgr::RestoreHooks() {
	hk_chatlog_.Retour();
	hk_chatcmd_.Retour();
}

void GWAPI::ChatMgr::SendChat(const wchar_t* msg, wchar_t channel)
{
	static P5E_SendChat* chat = new P5E_SendChat();

	chat->channel = channel;
	wcscpy_s(chat->msg, msg);

	api().CtoS().SendPacket<P5E_SendChat>(chat);
}

void GWAPI::ChatMgr::WriteChatF(const wchar_t* from, const wchar_t* format, ...) {
	va_list vl;
	va_start(vl, format);
	size_t szbuf = _vscwprintf(format, vl) + 1;
	wchar_t* chat = new wchar_t[szbuf];
	vswprintf_s(chat, szbuf, format, vl);
	va_end(vl);

	WriteChat(from, chat);

	delete[] chat;
}

void GWAPI::ChatMgr::WriteChat(const wchar_t* from, const wchar_t* msg) {

	((void(__fastcall *)(DWORD, const wchar_t*, const wchar_t*))
		MemoryMgr::WriteChatFunction)
		(0, from, msg);
}

void __fastcall GWAPI::ChatMgr::det_chatlog(DWORD ecx, DWORD edx, DWORD useless /* same as edx */)
{
	GWAPI::ChatMgr& chat = GWAPI::GWCA::Api().Chat();
	MessageInfo *mInfo = reinterpret_cast<MessageInfo*>(edx);
	ChannelInfo *cInfo = reinterpret_cast<ChannelInfo*>(ecx);

	std::wstring message(mInfo->message), sender;
	std::string::size_type start, end, quote, length = message.length();

	start = message.find(L"<a=1>");
	if (start != std::string::npos)
	{
		end = message.find(L"</a>", start + 5);
		if (end != std::string::npos)
			sender = message.substr(start + 5, end - start - 5);
	}

	quote = message.find(L"<quote>");
	if (quote != std::string::npos)
		message = message.substr(quote);

	Channel chan;
	if (!sender.empty())
		chan = chat.chatlog_channel[sender];

	if (!chan.name.empty())
	{
		wchar_t *buffer = new wchar_t[length + 26 + 1];
		wsprintf(buffer, L"<c=#%06x>%s</c>: <c=#%06x>%s", chan.col_sender, chan.name.c_str(), chan.col_message, message.c_str());
		chat.chatlog_result = buffer;
		delete[] buffer;
		mInfo->message = chat.chatlog_result.c_str();
	}

	return chat.ori_chatlog(ecx, edx, useless);
}

void __fastcall GWAPI::ChatMgr::det_chatcmd(DWORD ecx)
{
	ChatMgr& chat = GWAPI::GWCA::Api().Chat();
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

	CallBack callback = chat.chatcmd_callbacks[key];
	if (callback.callback && channel == '/')
	{
		std::vector<std::wstring> args;
		size_t index = 0;
		while (true) {
			size_t pos = arguments.find(L' ', index);

			std::wstring arg;
			if (pos == std::wstring::npos) {
				arg = arguments.substr(index);
			}
			else {
				arg = arguments.substr(index, pos - index);
			}
			if (!arg.empty()) {
				args.push_back(arg);
			}

			if (pos == std::wstring::npos) break;
			index = pos + 1;
		}

		callback.callback(args);

		if (callback.override)
			return chat.ori_chatcmd((DWORD)L"");
	}

	return chat.ori_chatcmd(ecx);
}
