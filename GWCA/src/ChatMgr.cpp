#include <GWCA\Managers\ChatMgr.h>

#include <sstream>

#include <GWCA\Utilities\Scanner.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Constants\Constants.h>

#define COLOR_ARGB(a, r, g, b) (GW::Chat::Color)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xff, r, g, b)

namespace {
	using namespace GW::Chat;

	struct RawMessage {
		int channel;
		wchar_t *message;
		int player_id;
	};

	struct ChatMessage {
		uint32_t channel;
		wchar_t string;
	};

	struct ChatBuffer {
		uint32_t next;
		uint32_t unk;
		ChatMessage messages[256];
	};
	SYSTEMTIME Timestamps[256];

	Color SenderColor[] = {
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

	Color MessageColor[] = {
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

	void(__fastcall *GwSendMessage)(int id, const RawMessage* msg, void *extended) = nullptr;

	#if 0
	void(__fastcall *GwWriteBuffer)(WCHAR *message, DWORD channel) = nullptr;
	void(__fastcall *DetWriteBuffer)(WCHAR *message, DWORD channel) = nullptr;
	#endif

	void __fastcall SendChat_detour(wchar_t *_message);
	typedef void(__fastcall *SendChat_t)(wchar_t* message);
	SendChat_t SendChat_addr = nullptr;
	GW::THook<SendChat_t> SendChat_hook;
	std::map<std::wstring, GW::Chat::Callback> commands_callbacks;
	std::function<void(Channel chan, wchar_t msg[139])> SendChat_callback;

	typedef void(__fastcall *OpenTemplate_t)(DWORD unk, GW::Chat::ChatTemplate* info);
	void __fastcall OpenTemplate_detour(DWORD unk, GW::Chat::ChatTemplate* info);
	GW::THook<OpenTemplate_t> OpenTemplate_hook;
	bool open_links = false;

	typedef Color* (__fastcall *GetChannelColor_t)(Color* color, Channel chan);
	Color* __fastcall SenderColor_detour(Color *color, Channel chan);
	Color* __fastcall MessageColor_detour(Color *color, Channel chan);
	GW::THook<GetChannelColor_t> SenderColor_hook;
	GW::THook<GetChannelColor_t> MessageColor_hook;

	typedef void(__fastcall *ChatEvent_t)(DWORD id, DWORD type, wchar_t* info, void* unk);
	GW::THook<ChatEvent_t> ChatEvent_hook;
	std::function<void(DWORD, DWORD, wchar_t*, void*)> ChatEvent_callback;
	void __fastcall ChatEvent_detour(DWORD id, DWORD type, wchar_t* info, void* unk) {
		if (ChatEvent_callback) ChatEvent_callback(id, type, info, unk);
		ChatEvent_hook.Original()(id, type, info, unk);
	}

	typedef void(__fastcall *LocalMessage_t)(int channel, wchar_t *message);
	GW::THook<LocalMessage_t> LocalMessage_hook;
	std::function<bool(int, wchar_t*)> LocalMessage_callback;
	void __fastcall LocalMessage_detour(int channel, wchar_t *message) {
		if (LocalMessage_callback && !LocalMessage_callback(channel, message))
			return;
		LocalMessage_hook.Original()(channel, message);
	}

	typedef void(__fastcall *WriteWhisper_t)(int, const wchar_t*, const wchar_t*);
	WriteWhisper_t WriteWhisper_addr = nullptr;
	GW::THook<WriteWhisper_t> WriteWhisper_hook;
	std::function<void(const wchar_t[20], const wchar_t[140])> WriteWhisper_callback;
	void __fastcall WriteWhisper_detour(int unk, const wchar_t *from, const wchar_t *msg) {
		if (WriteWhisper_callback) WriteWhisper_callback(from, msg);
		WriteWhisper_hook.Original()(unk, from, msg);
	}
}

void GW::Chat::SetChatEventCallback(std::function<void(DWORD, DWORD, wchar_t*, void*)> callback) {
	if (ChatEvent_hook.Empty()) {
		ChatEvent_t addr = (ChatEvent_t)Scanner::Find("\x83\xFB\x06\x1B", "xxxx", -0x28);
		printf("Chat Event Func address = 0x%X\n", (DWORD)addr);
		ChatEvent_hook.Detour((ChatEvent_t)addr, ChatEvent_detour);
	}
	ChatEvent_callback = callback;
}

void GW::Chat::SetLocalMessageCallback(std::function<bool(int, wchar_t*)> callback) {
	if (LocalMessage_hook.Empty()) {
		LocalMessage_t addr = (LocalMessage_t)0x007DEF00;
		printf("LocalMessage Func address = 0x%X\n", (DWORD)addr);
		LocalMessage_hook.Detour((LocalMessage_t)addr, LocalMessage_detour);
	}
	LocalMessage_callback = callback;
}

void GW::Chat::SetSendChatCallback(std::function<void(Channel chan, wchar_t msg[139])> callback) {
	if (SendChat_hook.Empty()) {
		if (SendChat_addr == nullptr) Initialize();
		SendChat_hook.Detour(SendChat_addr, SendChat_detour);
	}
	SendChat_callback = callback;
}

void GW::Chat::RegisterCommand(const String& command, Callback callback) {
	if (SendChat_hook.Empty()) {
		if (SendChat_addr == nullptr) Initialize();
		SendChat_hook.Detour(SendChat_addr, SendChat_detour);
	}
	commands_callbacks[command] = callback;
}
void GW::Chat::DeleteCommand(const String& command) {
	commands_callbacks.erase(command);
}

void GW::Chat::SetOpenLinks(bool b) {
	if (b && OpenTemplate_hook.Empty()) {
		OpenTemplate_t addr = (OpenTemplate_t)Scanner::Find("\x53\x8B\xDA\x57\x8B\xF9\x8B\x43", "xxxxxxxx", 0);
		printf("OpenTemplateFunc = %X\n", (DWORD)addr);
		OpenTemplate_hook.Detour(addr, OpenTemplate_detour);
	}
	open_links = b;
}

GW::Chat::Color GW::Chat::SetSenderColor(Channel chan, Color col) {
	if (SenderColor_hook.Empty()) {
		GetChannelColor_t addr = (GetChannelColor_t)0x00481650; // Need scan!
		SenderColor_hook.Detour(addr, SenderColor_detour);
	}
	Color old = SenderColor[chan];
	SenderColor[chan] = col;
	return old;
}

GW::Chat::Color GW::Chat::SetMessageColor(Channel chan, Color col) {
	if (MessageColor_hook.Empty()) {
		GetChannelColor_t addr = (GetChannelColor_t)0x00481570; // Need scan!
		MessageColor_hook.Detour(addr, MessageColor_detour);
	}
	Color old = MessageColor[chan];
	MessageColor[chan] = col;
	return old;
}

void GW::Chat::SetWhisperCallback(std::function<void(const wchar_t[20], const wchar_t[140])> callback) {
	if (!WriteWhisper_addr) {
		WriteWhisper_addr = (WriteWhisper_t)Scanner::Find("\x55\x8B\xEC\x51\x53\x89\x4D\xFC\x8B\x4D\x08\x56\x57\x8B", "xxxxxxxxxxxxxx", 0);
		printf("Write Whisper Func = 0x%X\n", (DWORD)WriteWhisper_addr);
	}
	if (WriteWhisper_hook.Empty())
		WriteWhisper_hook.Detour(WriteWhisper_addr, WriteWhisper_detour);
	WriteWhisper_callback = callback;
}

void GW::Chat::Initialize() {
	SendChat_addr = (SendChat_t)Scanner::Find("\xC7\x85\xE4\xFE\xFF\xFF\x5E", "xxxxxxx", -25);
	printf("Send Chat Func = 0x%X\n", (DWORD)SendChat_addr);

	GwSendMessage = (decltype(GwSendMessage))0x00605AC0;
	printf("Send Message Func = 0x%X\n", (DWORD)GwSendMessage);

	WriteWhisper_addr = (WriteWhisper_t)Scanner::Find("\x55\x8B\xEC\x51\x53\x89\x4D\xFC\x8B\x4D\x08\x56\x57\x8B", "xxxxxxxxxxxxxx", 0);
	printf("Write Whisper Func = 0x%X\n", (DWORD)WriteWhisper_addr);

}

void GW::Chat::RestoreHooks() {
	SendChat_hook.Retour();
	OpenTemplate_hook.Retour();
	SenderColor_hook.Retour();
	MessageColor_hook.Retour();
	ChatEvent_hook.Retour();
	LocalMessage_hook.Retour();
	WriteWhisper_hook.Retour();
}

void GW::Chat::SendChat(const wchar_t* msg, wchar_t channel) {
	if (SendChat_addr == nullptr) Initialize();
	wchar_t buffer[140] = {channel};
	wcscpy_s(&buffer[1], 139, msg);
	SendChat_addr(buffer);
}

void GW::Chat::SendChat(const char* msg, char channel) {
	if (SendChat_addr == nullptr) Initialize();
	wchar_t buffer[140];
	wchar_t* buf = buffer;
	*buf++ = static_cast<wchar_t>(channel);
	for (int i = 1; i < 139 && *msg; ++i) {
		*buf++ = static_cast<wchar_t>(*msg++);
	}
	*buf = L'\0';
	SendChat_addr(buffer);
}

void GW::Chat::WriteChatF(const wchar_t* from, const wchar_t* format, ...) {
	va_list vl;
	va_start(vl, format);
	size_t szbuf = _vscwprintf(format, vl) + 1;
	wchar_t* chat = new wchar_t[szbuf];
	vswprintf_s(chat, szbuf, format, vl);
	va_end(vl);
	
	WriteChat(from, chat);
	delete[] chat;
}

void GW::Chat::WriteChat(const wchar_t* from, const wchar_t* msg) {
	if (WriteWhisper_addr == nullptr) Initialize();
	WriteWhisper_addr(0, from, msg);
}

void GW::Chat::WriteChat(Channel channel, const wchar_t *message) {
	if (GwSendMessage == nullptr) Initialize();

	wchar_t *buffer = new wchar_t[wcslen(message) + 4];
	
	RawMessage msg;
	msg.channel = channel;
	msg.message = buffer;
	msg.player_id = 0;
	
	*buffer++ = 0x0108;
	*buffer++ = 0x0107;
	while (*message != L'\0') *buffer++ = *message++;
	*buffer++ = 0x0001;
	*buffer++ = 0;
	
	GwSendMessage(0x1000007E, &msg, NULL);
	delete[] msg.message;
}

void GW::Chat::WriteChat(Channel channel, const char* message) {
	if (GwSendMessage == nullptr) Initialize();

	wchar_t* buffer = new wchar_t[strlen(message) + 4];
	
	RawMessage msg;
	msg.channel = channel;
	msg.message = buffer;
	msg.player_id = 0;
	
	*buffer++ = 0x0108;
	*buffer++ = 0x0107;
	while (*message != L'\0') *buffer++ = static_cast<wchar_t>(*message++);
	*buffer++ = 0x0001;
	*buffer++ = 0;
	
	GwSendMessage(0x1000007E, &msg, NULL);
	delete[] msg.message;
}

namespace {
	using namespace GW::Chat;

	Channel GetChannel(wchar_t opcode) {
		switch (opcode) {
			case '!': return CHANNEL_ALL;
			case '@': return CHANNEL_GUILD;
			case '#': return CHANNEL_GROUP;
			case '$': return CHANNEL_TRADE;
			case '%': return CHANNEL_ALLIANCE;
			case '"': return CHANNEL_WHISPER;
			case '/': return CHANNEL_COMMAND;
			default: return CHANNEL_UNKNOW;
		}
	}

	void __fastcall SendChat_detour(wchar_t *message) {
		if (*message == '/') {
			String msg = &message[1];

			size_t index = msg.find(' ');
			String command = msg.substr(0, index);
			StringArray args;

			if (index != String::npos) {
				++index;
				while (index < msg.size()) {
					size_t end = msg.find(' ', index);
					if (end == String::npos) {
						args.push_back(msg.substr(index, end));
						index = end;
					} else {
						args.push_back(msg.substr(index, end - index));
						index = end + 1;
					}
				}
			}

			auto callback = commands_callbacks.find(command);
			if (callback != commands_callbacks.end()) {
				if (callback->second(command, args))
					return;
			}
		}

		if (SendChat_callback) SendChat_callback(GetChannel(*message), &message[1]);
		SendChat_hook.Original()(message);
	}

	void __fastcall OpenTemplate_detour(DWORD unk, ChatTemplate* info) {
		if (open_links
			&& info
			&& info->template_code
			&& info->template_name
			&& (!wcsncmp(info->template_name, L"http://", 7)
				|| !wcsncmp(info->template_name, L"https://", 8))) {
			ShellExecuteW(NULL, L"open", info->template_name, NULL, NULL, SW_SHOWNORMAL);
		} else {
			OpenTemplate_hook.Original()(unk, info);
		}
	}

	Color* __fastcall SenderColor_detour(Color *color, Channel chan) {
		*color = SenderColor[chan];
		return color;
	};

	Color* __fastcall MessageColor_detour(Color *color, Channel chan) {
		*color = MessageColor[chan];
		return color;
	};
}

#if 0
static void __fastcall det_write_buffer(WCHAR *message, DWORD channel)
{
	// @Robustness, Change to non static address.
	static ChatBuffer **buffer = (ChatBuffer**)0x00D560F0;
	GetLocalTime(&Timestamps[(*buffer)->next]);
	return DetWriteBuffer(message, channel);
}
#endif
