#include <GWCA\Managers\ChatMgr.h>

#include <sstream>

#include <GWCA\Utilities\Scanner.h>
#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Constants\Constants.h>

#define COLOR_ARGB(a, r, g, b) (GW::Chat::Color)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xff, r, g, b)

#define GWCALL __fastcall

bool GW::Chat::ShowTimestamps  = true;
bool GW::Chat::KeepChatHistory = true;

GW::Chat::Color GW::Chat::TimestampsColor = COLOR_RGB(0xff, 0xff, 0xff);

namespace {
	using namespace GW::Chat;

	struct ChatMessage {
		uint32_t channel;
		wchar_t *message;
	};

	struct ChatBuffer {
		uint32_t next;
		uint32_t unk;
		ChatMessage *messages[256];
	};

	// 08 01 07 01 [Time] 01 00 02 00
	ChatBuffer **ChatBufferAddr;
	SYSTEMTIME Timestamps[256];
	int  reprint_index;
	bool reprint_chat;

	// There is maybe more.
	// Though, we can probably fix this.
	bool ChannelThatParseColorTag[] = {
		true, true, true, true, true, true, true,
		false, // WARNING
		true, true, true, true, true,
		false, // ADVISORY
		true
	};

	GW::Chat::Color ChatSenderColor[] = {
		COLOR_RGB(0xFF, 0xC0, 0x60),
		COLOR_RGB(0x60, 0xA0, 0xFF),
		COLOR_RGB(0xC0, 0xD0, 0xFF),
		COLOR_RGB(0xFF, 0xFF, 0x80),
		COLOR_RGB(0xCC, 0xCC, 0xCC),
		COLOR_RGB(0xFF, 0x50, 0xDF),
		COLOR_RGB(0xFF, 0xFF, 0xFF),
		COLOR_RGB(0xCC, 0xCC, 0xCC),
		COLOR_RGB(0xCC, 0xCC, 0xCC),
		COLOR_RGB(0x00, 0xFF, 0x60),
		COLOR_RGB(0x80, 0xFF, 0x80),
		COLOR_RGB(0x80, 0xC0, 0xFF),
		COLOR_RGB(0xFF, 0xC0, 0xC4),
		COLOR_RGB(0xFF, 0x90, 0x20),
		COLOR_RGB(0x80, 0xC0, 0xFF)
	};

	GW::Chat::Color ChatMessageColor[] = {
		COLOR_RGB(0xE0, 0xE0, 0xE0),
		COLOR_RGB(0xE0, 0xE0, 0xE0),
		COLOR_RGB(0xC0, 0xD0, 0xFF),
		COLOR_RGB(0xFF, 0xFF, 0xFF),
		COLOR_RGB(0xB0, 0xB0, 0xB0),
		COLOR_RGB(0x50, 0xFF, 0xDF),
		COLOR_RGB(0xFF, 0xFF, 0xFF),
		COLOR_RGB(0xCC, 0xCC, 0xCC),
		COLOR_RGB(0x50, 0xFF, 0xDF),
		COLOR_RGB(0xE0, 0xE0, 0xE0),
		COLOR_RGB(0x80, 0xFF, 0x80),
		COLOR_RGB(0xE0, 0xE0, 0xE0),
		COLOR_RGB(0xFF, 0xC4, 0xC0),
		COLOR_RGB(0xFF, 0x90, 0x20),
		COLOR_RGB(0xE0, 0xE0, 0xE0)
	};

	void __fastcall SendChat_detour(wchar_t *_message);
	typedef void(__fastcall *SendChat_t)(wchar_t* message);
	SendChat_t SendChat_addr = nullptr;
	GW::THook<SendChat_t> SendChat_hook;
	std::map<std::wstring, GW::Chat::CmdCB> SlashCmdList;
	std::function<void(GW::Chat::Channel chan, wchar msg[120])> SendChat_callback;

	typedef void(__fastcall *OpenTemplate_t)(DWORD unk, GW::Chat::ChatTemplate* info);
	void __fastcall OpenTemplate_detour(DWORD unk, GW::Chat::ChatTemplate* info);
	GW::THook<OpenTemplate_t> OpenTemplate_hook;
	bool open_links = false;

	typedef GW::Chat::Color* (__fastcall *GetChannelColor_t)(GW::Chat::Color *color, GW::Chat::Channel chan);
	GW::Chat::Color* __fastcall SenderColor_detour(GW::Chat::Color *color, GW::Chat::Channel chan);
	GW::Chat::Color* __fastcall MessageColor_detour(GW::Chat::Color *color, GW::Chat::Channel chan);
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

	typedef void(__fastcall *WriteWhisper_t)(int, wchar*, wchar*);
	WriteWhisper_t WriteWhisper_addr = nullptr;
	GW::THook<WriteWhisper_t> WriteWhisper_hook;
	std::function<void(wchar[20], wchar[140])> WriteWhisper_callback;
	void __fastcall WriteWhisper_detour(int unk, wchar *from, wchar *msg) {
		if (WriteWhisper_callback) WriteWhisper_callback(from, msg);
		WriteWhisper_hook.Original()(unk, from, msg);
	}

	// This is used for more than just Chat stuff, but for now we only used it here.
	typedef void (GWCALL *SendUIMessage_t)(int id, void *param1, void *param2);
	SendUIMessage_t SendUIMessage = nullptr;

	typedef void (GWCALL *InitChatLog_t)(void);
	InitChatLog_t InitChatLog;
	GW::THook<InitChatLog_t> InitChatLog_hook;

	void GWCALL InitChatLog_detour(void) {
		// assert(ChatBufferAddr);
		ChatBuffer *buff = *ChatBufferAddr;
		if (!KeepChatHistory || !buff)
			InitChatLog_hook.Original()();
	}

	// PrintChatBuffer tell us when the chat is reprinted.
	// WriteChatBuffer is used to save messages timestamps.
	// PrintChat is used to chnage the message printed.
	typedef void (GWCALL *PrintChatLog_t)(void *ctx, int thiscall, int unk);
	typedef void (GWCALL *WriteChatLog_t)(Channel channel, wchar *encStr);
	typedef void (GWCALL *PrintChat_t)(void *ctx, int thiscall, Channel channel, wchar *str, int reprint);

	PrintChatLog_t PrintChatLog;
	WriteChatLog_t WriteChatLog;
	PrintChat_t    PrintChat;

	GW::THook<PrintChatLog_t> PrintChatLog_hook;
	GW::THook<WriteChatLog_t> WriteChatLog_hook;
	GW::THook<PrintChat_t>    PrintChat_hook;

	void GWCALL PrintChatLog_detour(void *ctx, int thiscall, int unk) {
		// assert(ChatBufferAddr);
		ChatBuffer *buff = *ChatBufferAddr;
		uint32_t first = buff->next;
		if (!buff->messages[first])
			first = 0;

		reprint_chat = true;
		reprint_index = first;
		PrintChatLog_hook.Original()(ctx, thiscall, unk);
		reprint_chat = false;
	}

	void GWCALL WriteChatLog_detour(Channel channel, wchar *encStr) {
		// assert(ChatBufferAddr);
		ChatBuffer *buff = *ChatBufferAddr;
		GetLocalTime(&Timestamps[buff->next]);
		WriteChatLog_hook.Original()(channel, encStr);
	}

	void GWCALL PrintChat_detour(void *ctx, int thiscall, Channel channel, wchar *str, int reprint) {
		// assert(ChatBufferAddr && 0 <= channel && channel < CHANNEL_COUNT);
		SYSTEMTIME *time = nullptr;
		ChatBuffer *buff = *ChatBufferAddr;

		if (!ShowTimestamps) {
			PrintChat_hook.Original()(ctx, thiscall, channel, str, reprint);
			return;
		}

		if (reprint_chat) {
			time = &Timestamps[reprint_index];
			reprint_index = (reprint_index + 1) % 256;
		} else {
			int tmp = buff->next;
			if (!tmp) tmp = 256;
			time = &Timestamps[tmp - 1];
		}

		// @Robustness, Buffer size, might create errors.
		wchar buffer[1024];
		if (ChannelThatParseColorTag[channel]) {
			if (time->wYear == 0) {
				wsprintf(buffer, L"\x108\x107<c=#%06x>[--:--] </c>\x01\x02%s", TimestampsColor, str);
			} else {
				wsprintf(buffer, L"\x108\x107<c=#%06x>[%02d:%02d] </c>\x01\x02%s", TimestampsColor, time->wHour, time->wMinute, str);
			}
		} else {
			if (time->wYear == 0) {
				wsprintf(buffer, L"\x108\x107[--:--] \x01\x02%s", TimestampsColor, str);
			} else {
				wsprintf(buffer, L"\x108\x107[%02d:%02d] \x01\x02%s", time->wHour, time->wMinute, str);
			}
		}
		PrintChat_hook.Original()(ctx, thiscall, channel, buffer, reprint);
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

void GW::Chat::SetSendChatCallback(std::function<void(GW::Chat::Channel chan, wchar_t msg[140])> callback) {
	if (::SendChat_hook.Empty()) {
		if (SendChat_addr == nullptr) Initialize();
		::SendChat_hook.Detour(SendChat_addr, SendChat_detour);
	}
	SendChat_callback = callback;
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
		SenderColor_hook.Detour(addr, ::SenderColor_detour);
	}
	Color old = ChatSenderColor[(int)chan];
	ChatSenderColor[(int)chan] = col;
	return old;
}

GW::Chat::Color GW::Chat::SetMessageColor(Channel chan, Color col) {
	if (MessageColor_hook.Empty()) {
		GetChannelColor_t addr = (GetChannelColor_t)0x00481570; // Need scan!
		MessageColor_hook.Detour(addr, ::MessageColor_detour);
	}
	Color old = ChatMessageColor[(int)chan];
	ChatMessageColor[(int)chan] = col;
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

	SendUIMessage = (SendUIMessage_t)0x00605AC0;
	printf("SendUIMessage Func = 0x%X\n", (DWORD)SendUIMessage);

	WriteWhisper_addr = (WriteWhisper_t)Scanner::Find("\x55\x8B\xEC\x51\x53\x89\x4D\xFC\x8B\x4D\x08\x56\x57\x8B", "xxxxxxxxxxxxxx", 0);
	printf("Write Whisper Func = 0x%X\n", (DWORD)WriteWhisper_addr);

	ChatBufferAddr = (ChatBuffer**)0x00D560F0; // Need scan!
	InitChatLog  = (InitChatLog_t)(0x007DE400); // Need scan!
	PrintChatLog = (PrintChatLog_t)(0x004A5C00); // Need scan!
	WriteChatLog = (WriteChatLog_t)(0x007DE520); // Need scan!
	PrintChat    = (PrintChat_t)(0x004A5880); // Need scan!

	InitChatLog_hook.Detour(InitChatLog, InitChatLog_detour);
	PrintChatLog_hook.Detour(PrintChatLog, PrintChatLog_detour);
	WriteChatLog_hook.Detour(WriteChatLog, WriteChatLog_detour);
	PrintChat_hook.Detour(PrintChat, PrintChat_detour);
}

void GW::Chat::RestoreHooks() {
	SendChat_hook.Retour();
	OpenTemplate_hook.Retour();
	SenderColor_hook.Retour();
	MessageColor_hook.Retour();
	ChatEvent_hook.Retour();
	LocalMessage_hook.Retour();
	WriteWhisper_hook.Retour();

	InitChatLog_hook.Retour();
	PrintChatLog_hook.Retour();
	WriteChatLog_hook.Retour();
	PrintChat_hook.Retour();
}

void GW::Chat::SendChat(char channel, const wchar *msg) {
	if (SendChat_addr == nullptr) Initialize();
	// assert(GW::SendChat);
	wchar buffer[140];

	// We could take 140 char long, but the chat only allow 120 ig.
	size_t len = wcslen(msg);
	len = len > 120 ? 120 : len;

	buffer[0] = channel;
	buffer[len + 1] = 0;
	for (size_t i = 0; i < len; i++)
		buffer[i + 1] = msg[i];

	SendChat_addr(buffer);
}

void GW::Chat::SendChat(char channel, const char *msg) {
	if (SendChat_addr == nullptr) Initialize();
	// assert(GW::SendChat);
	wchar buffer[140];

	size_t len = strlen(msg);
	len = len > 120 ? 120 : len;

	buffer[0] = channel;
	buffer[len + 1] = 0;
	for (size_t i = 0; i < len; i++)
		buffer[i + 1] = msg[i];

	SendChat_addr(buffer);
}

// Change to WriteChatF(Channel chan, const wchar *from, const wchar *frmt, ..)
// and       WriteChat(Channel chan, const wchar *from, const wchar *msg) 
/*
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

void GW::Chat::WriteChat(const wchar_t *from, const wchar_t *msg) {
	if (WriteWhisper_addr == nullptr) Initialize();
	WriteWhisper_addr(0, from, msg);
}
*/

void GW::Chat::WriteChat(Channel channel, const wchar *msg) {
	// assert(GW::SendUIMessage);
	if (SendUIMessage == nullptr) Initialize();

	size_t len = wcslen(msg);
	wchar *buffer = new wchar[len + 4];

	ChatMessage param;
	param.channel = (int)channel;
	param.message = buffer;

	buffer[0] = 0x108;
	buffer[1] = 0x107;
	for (size_t i = 0; i < len; i++)
		buffer[i + 2] = msg[i];

	buffer[len + 2] = 1;
	buffer[len + 3] = 0;

	SendUIMessage(0x1000007E, &param, NULL);
	delete[] buffer;
}

void GW::Chat::WriteChat(Channel channel, const char *msg) {
	// assert(GW::SendUIMessage);
	if (SendUIMessage == nullptr) Initialize();

	size_t len = strlen(msg);
	wchar *buffer = new wchar[len + 4];

	ChatMessage param;
	param.channel = (int)channel;
	param.message = buffer;

	buffer[0] = 0x108;
	buffer[1] = 0x107;
	for (size_t i = 0; i < len; i++)
		buffer[i + 2] = msg[i];

	buffer[len + 2] = 1;
	buffer[len + 3] = 0;

	SendUIMessage(0x1000007E, &param, NULL);
	delete[] buffer;
}

void GW::Chat::CreateCommand(std::wstring cmd, CmdCB callback) {
	if (::SendChat_hook.Empty()) {
		if (SendChat_addr == nullptr) Initialize();
		::SendChat_hook.Detour(SendChat_addr, SendChat_detour);
	}
	SlashCmdList[cmd] = callback;
}

void GW::Chat::DeleteCommand(std::wstring cmd) {
	SlashCmdList.erase(cmd);
}

std::vector<std::wstring> GW::Chat::SplitString(const std::wstring& str, wchar_t c) {
	std::vector<std::wstring> result;
	size_t size = str.size();
	size_t start = 0, end;
	for (size_t i = 0; i < size; i++) {
		if (str[i] == c) {
			end = i;
			result.push_back(str.substr(start, end - start));
			start = end + 1;
		}
	}
	if (start < size) result.push_back(str.substr(start, size));
	return result;
}

namespace {
	using namespace GW;

	GW::Chat::Channel GetChannel(wchar_t opcode) {
		switch (opcode) {
			case '!': return CHANNEL_ALL;
			case '@': return CHANNEL_GUILD;
			case '#': return CHANNEL_GROUP;
			case '$': return CHANNEL_TRADE;
			case '%': return CHANNEL_ALLIANCE;
			case '"': return CHANNEL_WHISPER;
			case '/': return CHANNEL_COMMAND;
			default:  return CHANNEL_UNKNOW;
		}
	}

	void __fastcall SendChat_detour(wchar_t *message) {
		if (*message == '/') {
			std::wstring msg = &message[1];
			std::wstring args;

			size_t index = msg.find(' ');
			std::wstring command = msg.substr(0, index);

			if (index == std::wstring::npos) {
				args = L"";
			} else {
				for (index += 1; index < msg.size(); index++)
					if (msg[index] != ' ') break;
				args = msg.substr(index);
			}

			auto callback = SlashCmdList.find(command);
			if (callback != SlashCmdList.end()) {
				callback->second(command, args);
				// Doesn't seem to have any reason to foward the function call to it's original.
				return;
			}
		}

		if (SendChat_callback) SendChat_callback(GetChannel(*message), &message[1]);
		SendChat_hook.Original()(message);
	}

	void __fastcall OpenTemplate_detour(DWORD unk, GW::Chat::ChatTemplate* info) {
		if (open_links
			&& info
			&& info->code.valid()
			&& info->name
			&& (!wcsncmp(info->name, L"http://", 7)
				|| !wcsncmp(info->name, L"https://", 8))) {
			ShellExecuteW(NULL, L"open", info->name, NULL, NULL, SW_SHOWNORMAL);
		} else {
			OpenTemplate_hook.Original()(unk, info);
		}
	}

	GW::Chat::Color* __fastcall SenderColor_detour(GW::Chat::Color *color, GW::Chat::Channel chan) {
		*color = ChatSenderColor[(int)chan];
		return color;
	};

	GW::Chat::Color* __fastcall MessageColor_detour(GW::Chat::Color *color, GW::Chat::Channel chan) {
		*color = ChatMessageColor[(int)chan];
		return color;
	};
}
