#include "ChatMgr.h"

#include <sstream>
#include <iomanip>
#include <ctime>

#include "GWCA.h"
#include "PatternScanner.h"

static wchar_t* wcssep(wchar_t* str, wchar_t sep);

GWAPI::ChatMgr::ChatMgr(GWAPIMgr& api) : GWCAManager(api)
{
	PatternScanner scanner("Gw.exe");
	BYTE* chatlog_addr = (BYTE*)scanner.FindPattern("\x53\x56\x8B\xF1\x57\x8B\x56\x14\x8B\x4E\x0C\xE8", "xxxxxxxxxxxx", -6);
	BYTE* chatcmd_addr = (BYTE*)scanner.FindPattern("\x8B\xD1\x68\x8A\x00\x00\x00\x8D\x8D\xE8\xFE\xFF\xFF", "xxxxxxxxxxxxx", -0xC);
	BYTE* writebuf_addr = (BYTE*)scanner.FindPattern("\x57\x8B\xFA\x85\xC0\x8B\xF1\x74", "xxxxxxxx", -6);
	BYTE* reloadchat_addr = (BYTE*)scanner.FindPattern("\x83\xEC\x08\x56\x8B\xF1\x57\x6A\xFF", "xxxxxxxxx", -3);
	BYTE* opentemplate_addr = (BYTE*)scanner.FindPattern("\x53\x8B\xDA\x57\x8B\xF9\x8B\x43","xxxxxxxx",0);

	ChatBufferLoca = (ChatBuffer**)(*(DWORD*)(writebuf_addr + 1));

	DWORD chatlog_length = Hook::CalculateDetourLength(chatlog_addr);
	DWORD chatcmd_length = Hook::CalculateDetourLength(chatcmd_addr);
	DWORD writebuf_length = Hook::CalculateDetourLength(writebuf_addr);
	DWORD reloadchat_length = Hook::CalculateDetourLength(reloadchat_addr);
	DWORD opentemplate_length = Hook::CalculateDetourLength(opentemplate_addr);

	ori_chatlog = (ChatLog_t)hk_chatlog_.Detour(chatlog_addr, (BYTE*)det_chatlog, chatlog_length);
	ori_chatcmd = (ChatCmd_t)hk_chatcmd_.Detour(chatcmd_addr, (BYTE*)det_chatcmd, chatcmd_length);
	ori_writebuf = (WriteBuf_t)hk_writebuf_.Detour(writebuf_addr, (BYTE*)det_writebuf, writebuf_length);
	ori_reloadchat = (ReloadChat_t)hk_reloadchat_.Detour(reloadchat_addr, (BYTE*)det_realoadchat, reloadchat_length);
	ori_opentemplate = (OpenTemplate_t)hk_opentemplate_.Detour(opentemplate_addr, (BYTE*)det_opentemplate, opentemplate_length);

	memset(timestamp, UNKNOW_TIMESTAMP, 0x100 * sizeof(DWORD));
	messageId = GetChatBuffer()->current;
	ToggleTimeStamp(false);
	SetTimestampColor(0x00ff00); // green
}

void GWAPI::ChatMgr::RestoreHooks() {
	hk_chatlog_.Retour();
	hk_chatcmd_.Retour();
	hk_writebuf_.Retour();
	hk_reloadchat_.Retour();
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

void __fastcall GWAPI::ChatMgr::det_chatlog(MessageInfo *info, Message *mes, DWORD useless /* same as edx */)
{
	GWAPI::ChatMgr& chat = GWAPI::GWCA::Api().Chat();

	std::wstring message(mes->message), sender;
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

	Channel chan = {};
	if (!sender.empty())
		chan = chat.chatlog_channel[sender];
	/* END MESSAGE PARSING, We have seperatly the sender, the message & the special channel data if there is one */

	DWORD mIndex = chat.messageId;
	if (chat.hashArray.find(info->hash) == chat.hashArray.end()) {
		chat.hashArray[info->hash] = mIndex;
		chat.messageId = (chat.messageId + 1) % 0x100;
	}
	else {
		mIndex = chat.hashArray[info->hash];
	}

	// Now we should have the index of the message inside the ChatBuffer & his timestamp is at same index in timestamp array
	DWORD time = chat.timestamp[mIndex];
	DWORD second = time % 60;
	DWORD minute = (time / 60) % 60;
	DWORD hour = time / 3600;
	// In theory we now have the hour/minute/seconde when the message was receive

	wchar_t timeBuffer[50] = L"";
	if (chat.timestamp_enable_) {
		if (time != chat.UNKNOW_TIMESTAMP)
			wsprintfW(timeBuffer, L"<c=#%06x>[%02u:%02u:%02u]</c>", chat.timestamp_color_, hour, minute, second);
		else
			wsprintfW(timeBuffer, L"<c=#%06x>[--:--:--]</c>", chat.timestamp_color_);
	}

	wchar_t mesBuffer[0x200] = L"";
	if (!chan.name.empty()) {
		wsprintfW(mesBuffer, L"<c=#%06x>%s</c>: <c=#%06x>%s", chan.col_sender, chan.name.c_str(), chan.col_message, message.c_str());
	} else {
		wcscpy_s(mesBuffer, mes->message);
	}

	wchar_t finalMessage[0x400]; // Cost nothing to overalloc but improvement required
	wsprintfW(finalMessage, L"%s %s", timeBuffer, mesBuffer);
	mes->message = finalMessage;
	mes->size1 = (mes->size2 = wcslen(mes->message));

	chat.ori_chatlog(info, mes, useless);
}

void __fastcall GWAPI::ChatMgr::det_chatcmd(wchar_t *_message)
{
	ChatMgr& chat = GWAPI::GWCA::Api().Chat();
	unsigned int length = wcslen(_message);
	wchar_t* message = new wchar_t[length + 1];
	wcscpy_s(message, length + 1, _message);

	if (*message == '/')
	{
		wchar_t* cmd = wcssep(message + 1, '\x20'); // \x20 is space
		CallBack cb = chat.chatcmd_callbacks[std::wstring(cmd)];

		if (cb.callback)
		{
			std::vector<std::wstring> args;

			wchar_t* arg = NULL;
			while (arg = wcssep(NULL, '\x20'))
				args.push_back(std::wstring(arg));

			cb.callback(std::wstring(cmd), args);

			if (cb.override)
				return;
		}
	}
	chat.ori_chatcmd(_message);
}

void __fastcall GWAPI::ChatMgr::det_writebuf(wchar_t *HMessage, DWORD channel)
{
	ChatMgr& chat = GWAPI::GWCA::Api().Chat();
	ChatBuffer *chatBuf = chat.GetChatBuffer();

	// Save timestamp of all messages that are actually save (they are the only one to get reprint on a ChatReload)
	SYSTEMTIME time;
	GetLocalTime(&time);
	chat.timestamp[chatBuf->current] = ((time.wHour * 60) + time.wMinute) * 60 + time.wSecond;

	chat.ori_writebuf(HMessage, channel);
}

void __fastcall GWAPI::ChatMgr::det_realoadchat(DWORD ecx, DWORD edx, DWORD unused)
{
	ChatMgr& chat = GWAPI::GWCA::Api().Chat();
	ChatBuffer *chatBuf = chat.GetChatBuffer();

	// On a reload of the chat we create internal counter for all reloaded message
	chat.messageId = (chatBuf->current + 1) % 0x100;
	if (!chatBuf->HMessage[chat.messageId])
		chat.messageId = 0;
	
	chat.hashArray.clear();

	chat.ori_reloadchat(ecx, edx, unused);
}

void __fastcall GWAPI::ChatMgr::det_opentemplate(DWORD unk, ChatTemplate* info) {
	if (!memcmp(info->template_name, L"http://", 7 * sizeof(wchar_t))
		|| !memcmp(info->template_name, L"https://", 8 * sizeof(wchar_t))) {
		ShellExecute(NULL, L"open", info->template_name, NULL, NULL, SW_SHOWNORMAL);
	} else {
		GWAPI::GWCA::Api().Chat().ori_opentemplate(unk, info);
	}
}

static wchar_t* wcssep(wchar_t* str, wchar_t sep)
{
	static wchar_t* next = NULL;
	if (str) next = str;
	if (!next) return NULL;

	while (*next == sep) next++;
	if (*next == '\0') return NULL;
	str = next;
	while (*next != sep && *next != '\0') next++;

	if (*next == '\0')
		next = NULL;
	else
		*next++ = '\0';

	return str;
}
