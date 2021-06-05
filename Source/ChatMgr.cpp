#include "stdafx.h"

#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/MemoryMgr.h>

#define COLOR_ARGB(a, r, g, b) (GW::Chat::Color)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xff, r, g, b)

namespace {
    using namespace GW;

#pragma warning(push)
#pragma warning(disable: 4200)
    struct ChatMessage {
        uint32_t channel;
        uint32_t unk1;
        FILETIME timestamp;
        wchar_t  message[0];
    };
#pragma warning(pop)

    const size_t CHAT_LOG_LENGTH = 0x200;
    struct ChatBuffer {
        uint32_t next;
        uint32_t unk1;
        uint32_t unk2;
        ChatMessage *messages[CHAT_LOG_LENGTH];
    };

    bool ShowTimestamps = false;
    bool Timestamp_24hFormat = false;
    bool Timestamp_seconds = false;
    Chat::Color TimestampsColor = COLOR_RGB(0xff, 0xff, 0xff);

    std::unordered_map<std::wstring, Chat::CmdCB> SlashCmdList;

    // 08 01 07 01 [Time] 01 00 02 00
    // ChatBuffer **ChatBuffer_Addr;
    uintptr_t ChatBuffer_Addr;
    uintptr_t IsTyping_Addr;

    // There is maybe more.
    // Though, we can probably fix this.
    bool ChannelThatParseColorTag[] = {
        true, true, true, true, true, true, true,
        false, // WARNING
        true, true, true, true, true,
        false, // ADVISORY
        true
    };

    Chat::Channel GetChannel(wchar_t opcode) {
        switch (opcode) {
            case '!': return Chat::Channel::CHANNEL_ALL;
            case '@': return Chat::Channel::CHANNEL_GUILD;
            case '#': return Chat::Channel::CHANNEL_GROUP;
            case '$': return Chat::Channel::CHANNEL_TRADE;
            case '%': return Chat::Channel::CHANNEL_ALLIANCE;
            case '"': return Chat::Channel::CHANNEL_WHISPER;
            case '/': return Chat::Channel::CHANNEL_COMMAND;
            default:  return Chat::Channel::CHANNEL_UNKNOW;
        }
    }

    Chat::Color ChatSenderColor[] = {
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

    Chat::Color ChatMessageColor[] = {
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

    void wstring_tolower(std::wstring& s)
    {
        for (size_t i = 0; i < s.size(); i++)
            s[i] = towlower(s[i]);
    }

    std::unordered_map<HookEntry *, Chat::SendChatCallback>     SendChat_callbacks;
    std::unordered_map<HookEntry *, Chat::ChatEventCallback>    ChatEvent_callbacks;
    std::unordered_map<HookEntry *, Chat::LocalMessageCallback> LocalMessage_callbacks;
    std::unordered_map<HookEntry *, Chat::WhisperCallback>      Whisper_callbacks;
    std::unordered_map<HookEntry*, Chat::PrintChatCallback>     PrintChat_callbacks;
    std::unordered_map<HookEntry *, Chat::StartWhisperCallback> StartWhisper_callbacks;

    typedef void(__cdecl *ChatEvent_pt)(uint32_t event_id, uint32_t type, wchar_t *info, void *unk);
    ChatEvent_pt RetChatEvent;
    ChatEvent_pt ChatEvent_Func;
    void __cdecl OnChatEvent(uint32_t event_id, uint32_t type, wchar_t *info, void *unk) {
        HookBase::EnterHook();
        HookStatus status;
        for (auto& it : ChatEvent_callbacks) {
            it.second(&status, event_id, type, info, unk);
            ++status.altitude;
        }
        if (!status.blocked)
            RetChatEvent(event_id, type, info, unk);
        HookBase::LeaveHook();
    }

    typedef Chat::Color* (__cdecl *GetChannelColor_pt)(Chat::Color *color, Chat::Channel chan);
    GetChannelColor_pt RetGetSenderColor;
    GetChannelColor_pt GetSenderColor_Func;
    Chat::Color* __cdecl OnGetSenderColor(Chat::Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        *color = ChatSenderColor[(int)chan];
        HookBase::LeaveHook();
        return color;
    };


    GetChannelColor_pt RetGetMessageColor;
    GetChannelColor_pt GetMessageColor_Func;
    Chat::Color* __cdecl OnGetMessageColor(Chat::Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        *color = ChatMessageColor[(int)chan];
        HookBase::LeaveHook();
        return color;
    };

    typedef void(__cdecl *LocalMessage_pt)(int channel, wchar_t *message);
    LocalMessage_pt LocalMessage_Func;
    LocalMessage_pt RetLocalMessage;
    void __cdecl OnLocalMessage(int channel, wchar_t *message) {
        HookBase::EnterHook();
        HookStatus status;
        for (auto& it : LocalMessage_callbacks) {
            it.second(&status, channel, message);
            ++status.altitude;
        }
        if (!status.blocked)
            RetLocalMessage(channel, message);
        HookBase::LeaveHook();
    }

    typedef void(__cdecl *SendChat_pt)(wchar_t *message, uint32_t agent_id);
    SendChat_pt SendChat_Func;
    SendChat_pt RetSendChat;
    void __cdecl OnSendChat(wchar_t *message, uint32_t agent_id) {
        HookBase::EnterHook();
        if (*message == '/') {
            int argc;
            wchar_t **argv;
            argv = CommandLineToArgvW(message + 1, &argc);
            std::wstring cmd = argv[0];
            ::wstring_tolower(cmd);

            auto callback = SlashCmdList.find(cmd);
            if (callback != SlashCmdList.end()) {
                callback->second(message, argc, argv);
                // No reasons to foward the function call to it's original.
                LocalFree(argv);
                HookBase::LeaveHook();
                return;
            }
            LocalFree(argv);
        }

        HookStatus status;
        for (auto& it : SendChat_callbacks) {
            it.second(&status, GetChannel(*message), &message[1]);
            ++status.altitude;
        }
        if (!status.blocked)
            RetSendChat(message, agent_id);
        HookBase::LeaveHook();
    }

    typedef void(__cdecl *WriteWhisper_pt)(uint32_t, wchar_t *, wchar_t *);
    WriteWhisper_pt RetWriteWhisper;
    WriteWhisper_pt WriteWhisper_Func;
    void __cdecl OnWriteWhisper(uint32_t unk, wchar_t *from, wchar_t *msg) {
        HookBase::EnterHook();
        HookStatus status;
        for (auto& it : Whisper_callbacks) {
            it.second(&status, from, msg);
            ++status.altitude;
        }
        if (!status.blocked)
            RetWriteWhisper(unk, from, msg);
        HookBase::LeaveHook();
    }

    typedef void(__fastcall* StartWhisper_pt)(void* ctx, uint32_t edx, wchar_t* name);
    StartWhisper_pt StartWhisper_Func;
    StartWhisper_pt RetStartWhisper;
    void __fastcall OnStartWhisper(void* ctx, uint32_t edx, wchar_t* name) {
        GW::HookBase::EnterHook();
        HookStatus status;
        for (auto& it : StartWhisper_callbacks) {
            it.second(&status, name);
            ++status.altitude;
        }
        if (!status.blocked)
            RetStartWhisper(ctx, edx, name);
        GW::HookBase::LeaveHook();
    }

    typedef void (__fastcall *PrintChat_pt)(void *ctx, uint32_t edx,
        Chat::Channel channel, wchar_t *str, FILETIME timestamp, int reprint);
    PrintChat_pt RetPrintChat;
    PrintChat_pt PrintChat_Func;
    void* PrintChat_Context = nullptr;
    // Used for fetching PrintChat_Context in chat window
    const wchar_t* PrintChat_Context_Sample_String = L"\x108\x107\x200B\x1";
    void __fastcall OnPrintChat(void *ctx, uint32_t edx,
        Chat::Channel channel, wchar_t *str, FILETIME timestamp, int reprint)
    {
        HookBase::EnterHook();
        GWCA_ASSERT(ChatBuffer_Addr && 0 <= channel && channel < Chat::Channel::CHANNEL_COUNT);
        PrintChat_Context = ctx;
        if (channel == Chat::Channel::CHANNEL_GWCA1 && wcscmp(str, PrintChat_Context_Sample_String) == 0) {
            HookBase::LeaveHook();
            return; // Spoof packet from GetChatWindowContext();
        }
		HookStatus status;
        wchar_t** str_p = &str;
		for (auto& it : PrintChat_callbacks) {
			it.second(&status, channel, str_p, timestamp, reprint);
			++status.altitude;
		}
		if (status.blocked) {
			//RetPrintChat(ctx, edx, channel, str, timestamp, reprint);
			HookBase::LeaveHook();
			return;
		}

        if (!ShowTimestamps) {
            RetPrintChat(ctx, edx, channel, *str_p, timestamp, reprint);
            HookBase::LeaveHook();
            return;
        }

        FILETIME   timestamp2;
        SYSTEMTIME localtime;

        FileTimeToLocalFileTime(&timestamp, &timestamp2);
        FileTimeToSystemTime(&timestamp2, &localtime);

        WORD hour = localtime.wHour;
        WORD minute = localtime.wMinute;
        WORD second = localtime.wSecond;

        if(!Timestamp_24hFormat)
            hour %= 12;

        wchar_t buffer[1024];
        wchar_t t_buffer[16];
        if (localtime.wYear == 0) {
            wsprintfW(t_buffer, Timestamp_seconds ? L"[--:--:--]" : L"[--:--]");
        }
        else {
            if(Timestamp_seconds)
                wsprintfW(t_buffer, L"[%02d:%02d:%02d]", hour, minute, second);
            else
                wsprintfW(t_buffer, L"[%02d:%02d]", hour, minute);
        }
        if (ChannelThatParseColorTag[channel]) {
            if (localtime.wYear == 0) {
                wsprintfW(buffer, L"\x108\x107<c=#%06x>[--:--] </c>\x01\x02%s", TimestampsColor, *str_p);
            } else {
                wsprintfW(buffer, L"\x108\x107<c=#%06x>%s </c>\x01\x02%s", (TimestampsColor & 0x00FFFFFF), t_buffer, *str_p);
            }
        } else {
            if (localtime.wYear == 0) {
                wsprintfW(buffer, L"\x108\x107[--:--] \x01\x02%s", *str_p);
            } else {
                wsprintfW(buffer, L"\x108\x107%s \x01\x02%s", t_buffer, *str_p);
            }
        }
        RetPrintChat(ctx, edx, channel, buffer, timestamp, reprint);
        HookBase::LeaveHook();
    }
    // Function to get chat window context. This is massively dodgy and the context should be found via offsets, but we don't have the functions mapped yet.
    void* GetChatWindowContext() {
        // This is caught (and blocked) in OnPrintChat above.
        if(!PrintChat_Context)
            Chat::WriteChatEnc(Chat::Channel::CHANNEL_GWCA1, PrintChat_Context_Sample_String);
        // @Cleanup: The context is reset on map change; this is not a massive issue because the game re-calls OnPrintChat before we have a chance
        return PrintChat_Context;
    }

    void Init() {
        ChatEvent_Func = (ChatEvent_pt)Scanner::Find("\x83\xFB\x06\x1B", "xxxx", -0x2A);
        GWCA_INFO("[SCAN] Chat Event = %p\n", ChatEvent_Func);

        GetSenderColor_Func = (GetChannelColor_pt)Scanner::Find(
            "\xC7\x00\x60\xC0\xFF\xFF\x5D\xC3", "xxxxxxxx", -0x1C);
        GWCA_INFO("[SCAN] GetSenderColor = %p\n", GetSenderColor_Func);

        GetMessageColor_Func = (GetChannelColor_pt)Scanner::Find(
            "\xC7\x00\xB0\xB0\xB0\xFF\x5D\xC3", "xxxxxxxx", -0x27);
        GWCA_INFO("[SCAN] GetMessageColor = %p\n", GetMessageColor_Func);

        // The last 4 bytes of the patterns are the "SendUIMessage" message id (i.e. 0x1000007E)
        LocalMessage_Func = (LocalMessage_pt)Scanner::Find(
            "\x8D\x45\xF8\x6A\x00\x50\x68\x7E\x00\x00\x10", "xxxxxxxxxxx", -0x3D);
        GWCA_INFO("[SCAN] LocalMessage = %p\n", LocalMessage_Func);

        SendChat_Func = (SendChat_pt)Scanner::Find(
            "\x8D\x85\xE0\xFE\xFF\xFF\x50\x68\x1C\x01", "xxxxxxxxx", -0x3E);
        GWCA_INFO("[SCAN] SendChat = %p\n", SendChat_Func);

        StartWhisper_Func = (StartWhisper_pt)GW::Scanner::Find(
            "\xFC\x53\x56\x8B\xF1\x57\x6A\x05\xFF\x36\xE8", "xxxxxxxxxxx", -0xF);
        GWCA_INFO("[SCAN] StartWhisper = %p\n", StartWhisper_Func);

        WriteWhisper_Func = (WriteWhisper_pt)Scanner::Find(
            "\x83\xC4\x04\x8D\x58\x2E", "xxxxxx", -0x18);
        GWCA_INFO("[SCAN] WriteWhisper = %p\n", WriteWhisper_Func);

        PrintChat_Func = (PrintChat_pt)Scanner::Find(
            "\x3D\x00\x00\x00\x00\x73\x2B\x6A", "x??xxxxx", -0x46);
        GWCA_INFO("[SCAN] PrintChat = %p\n", PrintChat_Func);

        {
            uintptr_t address = Scanner::Find(
                "\x8B\x45\x08\x83\x7D\x0C\x07\x74", "xxxxxxxx", -4);
            GWCA_INFO("[SCAN] ChatBuffer_Addr = %p\n", (void *)address);
            if (Verify(address))
                ChatBuffer_Addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\x08\xFF\xD0\xC7\x05\x00\x00\x00\x00\x01", "xxxxx????x", +5);
            GWCA_INFO("[SCAN] IsTyping_Addr = %p\n", (void *)address);
            if (Verify(address))
                IsTyping_Addr = *(uintptr_t *)address;
        }

        if (Verify(StartWhisper_Func))
            HookBase::CreateHook(StartWhisper_Func, OnStartWhisper, (void**)& RetStartWhisper);
        if (Verify(ChatEvent_Func))
            HookBase::CreateHook(ChatEvent_Func, OnChatEvent, (void **)&RetChatEvent);
        if (Verify(GetSenderColor_Func))
            HookBase::CreateHook(GetSenderColor_Func, OnGetSenderColor, (void **)&RetGetSenderColor);
        if (Verify(GetMessageColor_Func))
            HookBase::CreateHook(GetMessageColor_Func, OnGetMessageColor, (void **)&RetGetMessageColor);
        if (Verify(LocalMessage_Func))
            HookBase::CreateHook(LocalMessage_Func, OnLocalMessage, (void **)&RetLocalMessage);
        if (Verify(SendChat_Func))
            HookBase::CreateHook(SendChat_Func, OnSendChat, (void **)&RetSendChat);
        if (Verify(WriteWhisper_Func))
            HookBase::CreateHook(WriteWhisper_Func, OnWriteWhisper, (void **)&RetWriteWhisper);
        if (Verify(PrintChat_Func))
            HookBase::CreateHook(PrintChat_Func, OnPrintChat, (void **)&RetPrintChat);
    }

    void Exit() {
        if(StartWhisper_Func)
            HookBase::RemoveHook(StartWhisper_Func);
        if (ChatEvent_Func)
            HookBase::RemoveHook(ChatEvent_Func);
        if (GetSenderColor_Func)
            HookBase::RemoveHook(GetSenderColor_Func);
        if (GetMessageColor_Func)
            HookBase::RemoveHook(GetMessageColor_Func);
        if (LocalMessage_Func)
            HookBase::RemoveHook(LocalMessage_Func);
        if (SendChat_Func)
            HookBase::RemoveHook(SendChat_Func);
        if (WriteWhisper_Func)
            HookBase::RemoveHook(WriteWhisper_Func);
        if (PrintChat_Func)
            HookBase::RemoveHook(PrintChat_Func);
    }
}

namespace GW {

    Module ChatModule = {
        "ChatModule",   // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,         // exit_module
        NULL,           // enable_hooks
        NULL,           // disable_hooks
    };

    void Chat::RegisterSendChatCallback(
        HookEntry *entry,
        SendChatCallback callback)
    {
        SendChat_callbacks.insert({entry, callback});
    }

    void Chat::RemoveSendChatCallback(
        HookEntry *entry)
    {
        auto it = SendChat_callbacks.find(entry);
        if (it != SendChat_callbacks.end())
            SendChat_callbacks.erase(it);
    }

    void Chat::RegisterChatEventCallback(
        HookEntry *entry,
        ChatEventCallback callback)
    {
        ChatEvent_callbacks.insert({entry, callback});
    }

    void Chat::RemoveChatEventCallback(
        HookEntry *entry)
    {
        auto it = ChatEvent_callbacks.find(entry);
        if (it != ChatEvent_callbacks.end())
            ChatEvent_callbacks.erase(it);
    }

    void Chat::RegisterLocalMessageCallback(
        HookEntry *entry,
        LocalMessageCallback callback)
    {
        LocalMessage_callbacks.insert({entry, callback});
    }

    void Chat::RemoveLocalMessageCallback(
        HookEntry *entry)
    {
        auto it = LocalMessage_callbacks.find(entry);
        if (it != LocalMessage_callbacks.end())
            LocalMessage_callbacks.erase(it);
    }

    void Chat::RegisterWhisperCallback(
        HookEntry *entry,
        WhisperCallback callback)
    {
        Whisper_callbacks.insert({entry, callback});
    }

    void Chat::RegisterPrintChatCallback(
        HookEntry* entry,
        PrintChatCallback callback)
        {
            PrintChat_callbacks.insert({ entry, callback });
        }

    void Chat::RemoveRegisterWhisperCallback(
        HookEntry *entry)
    {
        auto it = Whisper_callbacks.find(entry);
        if (it != Whisper_callbacks.end())
            Whisper_callbacks.erase(it);
    }

    void Chat::RegisterStartWhisperCallback(
        HookEntry* entry,
        StartWhisperCallback callback)
    {
        StartWhisper_callbacks.insert({entry, callback});
    }

    void Chat::RemoveStartWhisperCallback(
        HookEntry *entry)
    {
        auto it = StartWhisper_callbacks.find(entry);
        if (it != StartWhisper_callbacks.end())
            StartWhisper_callbacks.erase(it);
    }

    Chat::Color Chat::SetSenderColor(Channel chan, Color col) {
        Color old = ChatSenderColor[(int)chan];
        ChatSenderColor[(int)chan] = col;
        return old;
    }

    Chat::Color Chat::SetMessageColor(Channel chan, Color col) {
        Color old = ChatMessageColor[(int)chan];
        ChatMessageColor[(int)chan] = col;
        return old;
    }

    void Chat::GetChannelColors(Channel chan, Color *sender, Color *message) {
        *sender  = ChatSenderColor[chan];
        *message = ChatMessageColor[chan];
    }

    void Chat::GetDefaultColors(Channel chan, Color *sender, Color *message) {
        switch (chan) {
        case Channel::CHANNEL_ALLIANCE:
            *sender  = COLOR_RGB(0xFF, 0xC0, 0x60);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case Channel::CHANNEL_ALLIES:
            *sender  = COLOR_RGB(0x60, 0xA0, 0xFF);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case Channel::CHANNEL_GWCA1:
            *sender  = COLOR_RGB(0xC0, 0xD0, 0xFF);
            *message = COLOR_RGB(0xC0, 0xD0, 0xFF);
            break;
        case Channel::CHANNEL_ALL:
            *sender  = COLOR_RGB(0xFF, 0xFF, 0x80);
            *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
            break;
        case Channel::CHANNEL_GWCA2:
            *sender  = COLOR_RGB(0xCC, 0xCC, 0xCC);
            *message = COLOR_RGB(0xB0, 0xB0, 0xB0);
            break;
        case Channel::CHANNEL_MODERATOR:
            *sender  = COLOR_RGB(0xFF, 0x50, 0xDF);
            *message = COLOR_RGB(0x50, 0xFF, 0xDF);
            break;
        case Channel::CHANNEL_EMOTE:
            *sender  = COLOR_RGB(0xFF, 0xFF, 0xFF);
            *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
            break;
        case Channel::CHANNEL_WARNING:
            *sender  = COLOR_RGB(0xCC, 0xCC, 0xCC);
            *message = COLOR_RGB(0xCC, 0xCC, 0xCC);
            break;
        case Channel::CHANNEL_GWCA3:
            *sender  = COLOR_RGB(0xCC, 0xCC, 0xCC);
            *message = COLOR_RGB(0x50, 0xFF, 0xDF);
            break;
        case Channel::CHANNEL_GUILD:
            *sender  = COLOR_RGB(0x00, 0xFF, 0x60);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case Channel::CHANNEL_GLOBAL:
            *sender  = COLOR_RGB(0x80, 0xFF, 0x80);
            *message = COLOR_RGB(0x80, 0xFF, 0x80);
            break;
        case Channel::CHANNEL_GROUP:
            *sender  = COLOR_RGB(0x80, 0xC0, 0xFF);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case Channel::CHANNEL_TRADE:
            *sender  = COLOR_RGB(0xFF, 0xC0, 0xC4);
            *message = COLOR_RGB(0xFF, 0xC4, 0xC0);
            break;
        case Channel::CHANNEL_ADVISORY:
            *sender  = COLOR_RGB(0xFF, 0x90, 0x20);
            *message = COLOR_RGB(0xFF, 0x90, 0x20);
            break;
        case Channel::CHANNEL_WHISPER:
            *sender  = COLOR_RGB(0x80, 0xC0, 0xFF);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        default:
            *sender = COLOR_RGB(0xFF, 0xFF, 0x80);
            *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
        }
    }

    bool Chat::GetIsTyping() {
        if (!Verify(IsTyping_Addr))
            return false;
        else
            return (*(uint32_t *)IsTyping_Addr == 1);
    }

    void Chat::SendChat(char channel, const wchar_t *msg) {
        GWCA_ASSERT(SendChat_Func);
        wchar_t buffer[140];

        // We could take 140 char long, but the chat only allow 120 ig.
        size_t len = wcslen(msg);
        len = len > 120 ? 120 : len;

        buffer[0] = static_cast<wchar_t>(channel);
        buffer[len + 1] = 0;
        for (size_t i = 0; i < len; i++)
            buffer[i + 1] = static_cast<wchar_t>(msg[i]);

        OnSendChat(buffer, 0);
    }

    void Chat::SendChat(char channel, const char *msg) {
        GWCA_ASSERT(SendChat_Func);
        wchar_t buffer[140];

        size_t len = strlen(msg);
        len = len > 120 ? 120 : len;

        buffer[0] = static_cast<wchar_t>(channel);
        buffer[len + 1] = 0;
        for (size_t i = 0; i < len; i++)
            buffer[i + 1] = static_cast<wchar_t>(msg[i]);

        OnSendChat(buffer, 0);
    }

    void Chat::SendChat(const wchar_t *from, const wchar_t *msg) {
        GWCA_ASSERT(SendChat_Func);
        wchar_t buffer[140];

        if (swprintf(buffer, 140, L"\"%s,%s", from, msg) < 140) {
            buffer[139] = 0;
            OnSendChat(buffer, 0);
        }
    }

    void Chat::SendChat(const char *from, const char *msg) {
        GWCA_ASSERT(SendChat_Func);
        wchar_t buffer[140];

        if (swprintf(buffer, 140, L"\"%S,%S", from, msg) < 140) {
            buffer[139] = 0;
            OnSendChat(buffer, 0);
        }
    }

    // Change to WriteChatF(Channel chan, const wchar_t *from, const wchar_t *frmt, ..)
    // and       WriteChat(Channel chan, const wchar_t *from, const wchar_t *msg) 
    /*
    void Chat::WriteChatF(const wchar_t* from, const wchar_t* format, ...) {
        va_list vl;
        va_start(vl, format);
        size_t szbuf = _vscwprintf(format, vl) + 1;
        wchar_t* chat = new wchar_t[szbuf];
        vswprintf_s(chat, szbuf, format, vl);
        va_end(vl);
        
        WriteChat(from, chat);
        delete[] chat;
    }

    void Chat::WriteChat(const wchar_t *from, const wchar_t *msg) {
        if (WriteWhisper_addr == nullptr) Initialize();
        WriteWhisper_addr(0, from, msg);
    }
    */



    void Chat::WriteChat(Channel channel, const wchar_t *msg, const wchar_t *sender,bool transient) {

        size_t len = wcslen(msg);
        wchar_t *buffer = new wchar_t[len + 4];
        size_t written = 0;
        buffer[written++] = 0x108;
        buffer[written++] = 0x107;
        for (size_t i = 0; i < len; i++)
            buffer[written++] = msg[i];
        buffer[written++] = 1;
        buffer[written++] = 0;
        WriteChatEnc(channel, buffer, sender, transient);
        delete[] buffer;
    }
    void Chat::WriteChatEnc(Channel channel, const wchar_t* msg, const wchar_t* sender, bool transient) {
        static wchar_t* new_message;
        UI::UIChatMessage param;
        param.channel = channel;
        param.channel2 = channel;
        param.message = (wchar_t*)msg;
        if (sender) {
            size_t msg_len = wcslen(msg);
            size_t sender_len = wcslen(sender);
            size_t written = 0;
            new_message = new wchar_t[msg_len + sender_len + 9];
            param.message = new_message;
            param.message[written++] = 0x76b;
            param.message[written++] = 0x10a;
            param.message[written++] = 0x108;
            param.message[written++] = 0x107;
            for (size_t i = 0; i < sender_len; i++)
                param.message[written++] = sender[i];
            param.message[written++] = 0x1;
            param.message[written++] = 0x1;
            param.message[written++] = 0x10b;
            for (size_t i = 0; i < msg_len; i++)
                param.message[written++] = msg[i];
            param.message[written++] = 0x1;
            param.message[written++] = 0;
        }
        if (transient) {
            // NB: Due to the noddy way we try to get chat window context, it may not be available even when we try to get it. Silent fail.
            if (GetChatWindowContext()) {
                SYSTEMTIME now;
                GetSystemTime(&now);
                FILETIME now_ft;
                GWCA_ASSERT(SystemTimeToFileTime(&now, &now_ft));
                OnPrintChat(GetChatWindowContext(), 0, channel, param.message, now_ft, 0);
            }
        }
        else {
            UI::SendUIMessage(UI::kWriteToChatLog, &param);
        }
        if (sender) 
            delete[] new_message;
    }

    void Chat::CreateCommand(std::wstring cmd, CmdCB callback) {
        ::wstring_tolower(cmd);
        SlashCmdList[cmd] = callback;
    }

    void Chat::DeleteCommand(std::wstring cmd) {
        ::wstring_tolower(cmd);
        SlashCmdList.erase(cmd);
    }

    void Chat::ToggleTimestamps(bool enable) {
        ShowTimestamps = enable;
        if (ShowTimestamps)
            UI::SetCheckboxPreference(UI::CheckboxPreference::CheckboxPreference_ShowChatTimestamps, 0);
    }

    void Chat::SetTimestampsFormat(bool use_24h, bool show_timestamp_seconds) {
        Timestamp_24hFormat = use_24h;
        Timestamp_seconds = show_timestamp_seconds;
    }

    void Chat::SetTimestampsColor(Color color) {
        TimestampsColor = color;
    }

} // namespace GW
