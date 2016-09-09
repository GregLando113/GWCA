#include <GWCA\Managers\ChatMgr.h>

#include <GWCA\Utilities\PatternScanner.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

#define COLOR_ARGB(a, r, g, b) (GW::Color)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xff, r, g, b)

struct RawMessage {
	int channel;
	wchar_t *message;
	int player_id;
};

static GW::Color SenderColor[] = {
	COLOR_RGB(0xff, 0xc0, 0x60),
	COLOR_RGB(0x60, 0xa0, 0xff),
	COLOR_RGB(0xc0, 0xd0, 0xff),
	COLOR_RGB(0xff, 0xff, 0x80),
	COLOR_RGB(0xcc, 0xcc, 0xcc),
	COLOR_RGB(0xff, 0x50, 0xdf),
	COLOR_RGB(0xff, 0xff, 0xff),
	COLOR_RGB(0xcc, 0xcc, 0xcc),
	COLOR_RGB(0xcc, 0xcc, 0xcc),
	COLOR_RGB(0x00, 0xff, 0x60),
	COLOR_RGB(0x80, 0xff, 0x80),
	COLOR_RGB(0x80, 0xc0, 0xff),
	COLOR_RGB(0xff, 0xc0, 0xc4),
	COLOR_RGB(0xff, 0x90, 0x20),
	COLOR_RGB(0x80, 0xc0, 0xff)
};

static GW::Color MessageColor[] = {
	COLOR_RGB(0xe0, 0xe0, 0xe0),
	COLOR_RGB(0xe0, 0xe0, 0xe0),
	COLOR_RGB(0xc0, 0xd0, 0xff),
	COLOR_RGB(0xff, 0xff, 0xff),
	COLOR_RGB(0xb0, 0xb0, 0xb0),
	COLOR_RGB(0x50, 0xff, 0xdf),
	COLOR_RGB(0xff, 0xff, 0xff),
	COLOR_RGB(0xcc, 0xcc, 0xcc),
	COLOR_RGB(0x50, 0xff, 0xdf),
	COLOR_RGB(0xe0, 0xe0, 0xe0),
	COLOR_RGB(0x80, 0xff, 0x80),
	COLOR_RGB(0xe0, 0xe0, 0xe0),
	COLOR_RGB(0xff, 0xc4, 0xc0),
	COLOR_RGB(0xff, 0x90, 0x20),
	COLOR_RGB(0xe0, 0xe0, 0xe0)
};

static void (__fastcall *GwSendChat)(const wchar_t *message);
static void (__fastcall *GwWriteChat)(int, const wchar_t*, const wchar_t*);
static void (__fastcall *GwSendMessage)(int id, void *param, void *extended);

static wchar_t *tohstr(wchar_t *buffer, const wchar_t *str);

GW::ChatMgr::ChatMgr() {
	PatternScanner scanner(0x401000, 0x49A000);

	BYTE* addr = (BYTE*)scanner.FindPattern("\xC7\x85\xE4\xFE\xFF\xFF\x5E", "xxxxxxx", -25);
	DWORD length = Hook::CalculateDetourLength(addr);
	ori_sendchat = (SendChat_t)hk_sendchat_.Detour(addr, (BYTE*)det_sendchat, length);
	GwSendChat = (decltype(GwSendChat))addr;

	addr = (BYTE*)scanner.FindPattern("\x53\x8B\xDA\x57\x8B\xF9\x8B\x43", "xxxxxxxx", 0);
	length = Hook::CalculateDetourLength(addr);
	ori_opentemplate = (OpenTemplate_t)hk_opentemplate_.Detour(addr, (BYTE*)det_opentemplate, length);

	addr = (BYTE*)0x00481650;
	length = Hook::CalculateDetourLength(addr);
	hk_sendercolor_.Detour(addr, (BYTE*)det_sendercolor, length);

	addr = (BYTE*)0x00481570;
	length = Hook::CalculateDetourLength(addr);
	hk_messagecolor_.Detour(addr, (BYTE*)det_messagecolor, length);

	GwSendMessage = (decltype(GwSendMessage))0x00605AC0;
	GwWriteChat = (decltype(GwWriteChat))MemoryMgr::WriteChatFunction;
}

void GW::ChatMgr::RestoreHooks() {
	hk_sendchat_.Retour();
	hk_opentemplate_.Retour();
	hk_sendercolor_.Retour();
	hk_messagecolor_.Retour();
}

void GW::ChatMgr::SendChat(const wchar_t* msg, wchar_t channel) {
	wchar_t buffer[140] = {channel};
	wcscpy_s(&buffer[1], 139, msg);
	GwSendChat(buffer);
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
	GwWriteChat(0, from, msg);
}

void GW::ChatMgr::WriteChat(Channel channel, const wchar_t *message) {
	size_t len = wcslen(message);
	wchar_t *buffer = new wchar_t[len + 4];
	if (tohstr(buffer, message)) {
		RawMessage msg;
		msg.channel = channel;
		msg.message = buffer;
		msg.player_id = 0;

		GameThreadMgr::Instance().Enqueue(GwSendMessage, 0x1000007E, &msg, NULL);
	}
	delete buffer;
}

static wchar_t *tohstr(wchar_t *buffer, const wchar_t *str) {
	if (!buffer || !str)
		return NULL;

	*buffer++ = 0x0108;
	*buffer++ = 0x0107;
	while(*str != '\0') *buffer++ = *str++;
	*buffer++ = 0x0001;
	*buffer++ = 0;
	return buffer;
}

GW::Color GW::ChatMgr::SetSenderColor(Channel chan, Color col) {
	Color old = SenderColor[chan];
	SenderColor[chan] = col;
	return old;
}

GW::Color GW::ChatMgr::SetMessageColor(Channel chan, Color col) {
	Color old = MessageColor[chan];
	MessageColor[chan] = col;
	return old;
}

void __fastcall GW::ChatMgr::det_sendchat(wchar_t *message) {
	ChatMgr &chat = ChatMgr::Instance();
	if (*message == '/') {
		GW::ChatMgr::String msg = &message[1];

		size_t index = msg.find(' ');
		GW::ChatMgr::String command = msg.substr(0, index);
		GW::ChatMgr::StringArray args;

		size_t start = index + 1, end = String::npos;
		while (start != String::npos) {
			end = msg.find(' ', start);
			args.push_back( msg.substr(start, end) );
			start = end + 1;
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
		&& (!wcsncmp(info->template_name, L"http://", 7)
			|| !wcsncmp(info->template_name, L"https://", 8))) {
		ShellExecute(NULL, L"open", info->template_name, NULL, NULL, SW_SHOWNORMAL);
	} else {
		ChatMgr::Instance().ori_opentemplate(unk, info);
	}
}

GW::Color* __fastcall GW::ChatMgr::det_sendercolor(Color *color, Channel chan) {
	*color = SenderColor[chan];
	return color;
};

GW::Color* __fastcall GW::ChatMgr::det_messagecolor(Color *color, Channel chan) {
	*color = MessageColor[chan];
	return color;
};
