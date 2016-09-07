#include <GWCA\Managers\ChatMgr.h>

#include <sstream>
#include <iomanip>
#include <ctime>

#include <GWCA\Utilities\PatternScanner.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

static wchar_t* wcssep(wchar_t* str, wchar_t sep);

GW::ChatMgr::ChatMgr() {
	PatternScanner scanner(0x401000, 0x49A000);
	BYTE* chatcmd_addr = (BYTE*)scanner.FindPattern("\x8B\xD1\x68\x8A\x00\x00\x00\x8D\x8D\xE8\xFE\xFF\xFF", "xxxxxxxxxxxxx", -0xC);
	BYTE* opentemplate_addr = (BYTE*)scanner.FindPattern("\x53\x8B\xDA\x57\x8B\xF9\x8B\x43", "xxxxxxxx", 0);

	DWORD chatcmd_length = Hook::CalculateDetourLength(chatcmd_addr);
	DWORD opentemplate_length = Hook::CalculateDetourLength(opentemplate_addr);

	ori_chatcmd = (ChatCmd_t)hk_chatcmd_.Detour(chatcmd_addr, (BYTE*)det_chatcmd, chatcmd_length);
	ori_opentemplate = (OpenTemplate_t)hk_opentemplate_.Detour(opentemplate_addr, (BYTE*)det_opentemplate, opentemplate_length);
}

void GW::ChatMgr::RestoreHooks() {
	hk_chatcmd_.Retour();
	hk_opentemplate_.Retour();
}

void GW::ChatMgr::SendChat(const wchar_t* msg, wchar_t channel) {
	static P5E_SendChat* chat = new P5E_SendChat();

	chat->channel = channel;
	wcscpy_s(chat->msg, msg);

	CtoSMgr::Instance().SendPacket<P5E_SendChat>(chat);
}

void GW::ChatMgr::WriteChatF(const wchar_t* from, const wchar_t* format, ...) {
	va_list vl;
	va_start(vl, format);
	size_t szbuf = _vscwprintf(format, vl) + 1;
	wchar_t* chat = new wchar_t[szbuf];
	vswprintf_s(chat, szbuf, format, vl);
	va_end(vl);

	WriteChat(from, chat);

	delete[] chat;
}

void GW::ChatMgr::WriteChat(const wchar_t* from, const wchar_t* msg) {

	((void(__fastcall *)(DWORD, const wchar_t*, const wchar_t*))
		MemoryMgr::WriteChatFunction)
		(0, from, msg);
}

void __fastcall GW::ChatMgr::det_chatcmd(wchar_t *_message) {
	ChatMgr& chat = ChatMgr::Instance();
	unsigned int length = wcslen(_message);
	wchar_t* message = new wchar_t[length + 1];
	wcscpy_s(message, length + 1, _message);

	if (*message == '/') {
		wchar_t* cmd = wcssep(message + 1, '\x20'); // \x20 is space
		CallBack cb = chat.chatcmd_callbacks[std::wstring(cmd)];

		if (cb.callback) {
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

void __fastcall GW::ChatMgr::det_opentemplate(DWORD unk, ChatTemplate* info) {
	if (ChatMgr::Instance().open_links_
		&& info->template_name != nullptr
		&& (!memcmp(info->template_name, L"http://", 7 * sizeof(wchar_t))
			|| !memcmp(info->template_name, L"https://", 8 * sizeof(wchar_t)))) {
		ShellExecute(NULL, L"open", info->template_name, NULL, NULL, SW_SHOWNORMAL);
	} else {
		ChatMgr::Instance().ori_opentemplate(unk, info);
	}
}

static wchar_t* wcssep(wchar_t* str, wchar_t sep) {
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
