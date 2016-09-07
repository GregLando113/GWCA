#include <GWCA\Managers\ChatMgr.h>

#include <sstream>
#include <iomanip>
#include <ctime>

#include <GWCA\Utilities\PatternScanner.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

GW::ChatMgr::ChatMgr() {
	PatternScanner scanner(0x401000, 0x49A000);
	BYTE* sendchat_addr = (BYTE*)scanner.FindPattern("\xC7\x85\xE4\xFE\xFF\xFF\x5E", "xxxxxxx", -25);
	BYTE* opentemplate_addr = (BYTE*)scanner.FindPattern("\x53\x8B\xDA\x57\x8B\xF9\x8B\x43", "xxxxxxxx", 0);

	DWORD sendchat_length = Hook::CalculateDetourLength(sendchat_addr);
	DWORD opentemplate_length = Hook::CalculateDetourLength(opentemplate_addr);

	ori_sendchat = (SendChat_t)hk_sendchat_.Detour(sendchat_addr, (BYTE*)det_sendchat, sendchat_length);
	ori_opentemplate = (OpenTemplate_t)hk_opentemplate_.Detour(opentemplate_addr, (BYTE*)det_opentemplate, opentemplate_length);
}

void GW::ChatMgr::RestoreHooks() {
	hk_sendchat_.Retour();
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

void __fastcall GW::ChatMgr::det_sendchat(wchar_t *message)
{
    ChatMgr &chat = ChatMgr::Instance();
    if (*message == '/') {
        GW::ChatMgr::String msg = &message[1];

        size_t index = msg.find_first_of(' ');
        GW::ChatMgr::String command = msg.substr(0, index);
        GW::ChatMgr::StringArray args;

        size_t start = index, end = String::npos;
        while (start != String::npos) {
            end = msg.find_first_of(' ', start);
            args.push_back( msg.substr(start, end) );
            start = end;
        }

        auto callback = chat.sendchat_callbacks.find(command);
        if (callback != chat.sendchat_callbacks.end()) {
            if (!callback->second(command, args))
                return;
        }
    }
    chat.ori_sendchat(message);
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
