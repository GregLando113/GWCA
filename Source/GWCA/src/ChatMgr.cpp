#include "stdafx.h"

#include <ShellApi.h>

#include <GWCA/Constants/Constants.h>

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

#define GWCALL __fastcall

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

    bool ShowTimestamps = true;
    bool Timestamp_24hFormat = false;
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

    std::function<void (uint32_t, uint32_t, wchar_t *, void *)> ChatEvent_callback;
    typedef void(__fastcall *ChatEvent_pt)(uint32_t event_id, uint32_t type, wchar_t *info, void *unk);
    ChatEvent_pt RetChatEvent;
    ChatEvent_pt ChatEvent_Func;
    void __fastcall OnChatEvent(uint32_t event_id, uint32_t type, wchar_t *info, void *unk) {
        HookBase::EnterHook();
        if (ChatEvent_callback) ChatEvent_callback(event_id, type, info, unk);
        RetChatEvent(event_id, type, info, unk);
        HookBase::LeaveHook();
    }

    typedef Chat::Color* (__fastcall *GetChannelColor_pt)(Chat::Color *color, Chat::Channel chan);
    GetChannelColor_pt RetGetSenderColor;
    GetChannelColor_pt GetSenderColor_Func;
    Chat::Color* __fastcall OnGetSenderColor(Chat::Color *color, Chat::Channel chan);
    GetChannelColor_pt RetGetMessageColor;
    GetChannelColor_pt GetMessageColor_Func;
    Chat::Color* __fastcall OnGetMessageColor(Chat::Color *color, Chat::Channel chan);

    typedef void(__fastcall *LocalMessage_pt)(int channel, wchar_t *message);
    LocalMessage_pt LocalMessage_Func;
    LocalMessage_pt RetLocalMessage;
    std::function<bool (int, wchar_t *)> LocalMessage_callback;
    void __fastcall OnLocalMessage(int channel, wchar_t *message) {
        HookBase::EnterHook();
        if (LocalMessage_callback && LocalMessage_callback(channel, message))
            RetLocalMessage(channel, message);
        HookBase::LeaveHook();
    }

    std::function<void (Chat::Channel chan, wchar_t *msg)> SendChat_callback;
    typedef void(__fastcall *SendChat_pt)(wchar_t *message);
    SendChat_pt SendChat_Func;
    SendChat_pt RetSendChat;
    void __fastcall OnSendChat(wchar_t *_message);

    typedef void(__fastcall *OpenTemplate_pt)(uint32_t unk, Chat::ChatTemplate* info);
    OpenTemplate_pt RetOpenTemplate;
    OpenTemplate_pt OpenTemplate_Func;
    void __fastcall OnOpenTemplate(uint32_t unk, Chat::ChatTemplate* info);
    bool open_links = false;

    typedef void(__fastcall *WriteWhisper_pt)(uint32_t, wchar_t *, wchar_t *);
    WriteWhisper_pt RetWriteWhisper;
    WriteWhisper_pt WriteWhisper_Func;
    std::function<void (wchar_t *, wchar_t *)> WriteWhisper_callback;
    void __fastcall OnWriteWhisper(uint32_t unk, wchar_t *from, wchar_t *msg) {
        HookBase::EnterHook();
        if (WriteWhisper_callback)
            WriteWhisper_callback(from, msg);
        RetWriteWhisper(unk, from, msg);
        HookBase::LeaveHook();
    }

    typedef void (GWCALL *PrintChat_pt)(void *ctx, uint32_t thiscall,
        Chat::Channel channel, wchar_t *str, FILETIME timestamp, int reprint);
    PrintChat_pt RetPrintChat;
    PrintChat_pt PrintChat_Func;

    void GWCALL OnPrintChat(void *ctx, int thiscall,
        Chat::Channel channel, wchar_t *str, FILETIME timestamp, int reprint)
    {
        HookBase::EnterHook();
        assert(ChatBuffer_Addr && 0 <= channel && channel < Chat::CHANNEL_COUNT);

        if (!ShowTimestamps) {
            RetPrintChat(ctx, thiscall, channel, str, timestamp, reprint);
            HookBase::LeaveHook();
            return;
        }

        FILETIME   timestamp2;
        SYSTEMTIME localtime;

        FileTimeToLocalFileTime(&timestamp, &timestamp2);
        FileTimeToSystemTime(&timestamp2, &localtime);

        WORD hour = localtime.wHour;
        WORD minute = localtime.wMinute;

        if (Timestamp_24hFormat)
            hour %= 12;

        wchar_t buffer[1024];
        if (ChannelThatParseColorTag[channel]) {
            if (localtime.wYear == 0) {
                wsprintfW(buffer, L"\x108\x107<c=#%06x>[--:--] </c>\x01\x02%s", TimestampsColor, str);
            } else {
                wsprintfW(buffer, L"\x108\x107<c=#%06x>[%02d:%02d] </c>\x01\x02%s", (TimestampsColor & 0x00FFFFFF), hour, minute, str);
            }
        } else {
            if (localtime.wYear == 0) {
                wsprintfW(buffer, L"\x108\x107[--:--] \x01\x02%s", str);
            } else {
                wsprintfW(buffer, L"\x108\x107[%02d:%02d] \x01\x02%s", hour, minute, str);
            }
        }
        RetPrintChat(ctx, thiscall, channel, buffer, timestamp, reprint);
        HookBase::LeaveHook();
    }

    Chat::Channel GetChannel(wchar_t opcode) {
        switch (opcode) {
            case '!': return Chat::CHANNEL_ALL;
            case '@': return Chat::CHANNEL_GUILD;
            case '#': return Chat::CHANNEL_GROUP;
            case '$': return Chat::CHANNEL_TRADE;
            case '%': return Chat::CHANNEL_ALLIANCE;
            case '"': return Chat::CHANNEL_WHISPER;
            case '/': return Chat::CHANNEL_COMMAND;
            default:  return Chat::CHANNEL_UNKNOW;
        }
    }

    void __fastcall OnSendChat(wchar_t *message) {
        HookBase::EnterHook();
        if (*message == '/') {
            int argc;
            wchar_t **argv;
            argv = CommandLineToArgvW(message + 1, &argc);

            auto callback = SlashCmdList.find(argv[0]);
            if (callback != SlashCmdList.end()) {
                callback->second(message, argc, argv);
                // No reasons to foward the function call to it's original.
                LocalFree(argv);
                HookBase::LeaveHook();
                return;
            }
            LocalFree(argv);
        }

        if (SendChat_callback)
            SendChat_callback(GetChannel(*message), &message[1]);
        RetSendChat(message);
        HookBase::LeaveHook();
    }

    void __fastcall OnOpenTemplate(uint32_t unk, Chat::ChatTemplate* info) {
        HookBase::EnterHook();
        if (open_links
            && info
            && info->code.valid()
            && info->name
            && (!wcsncmp(info->name, L"http://", 7)
                || !wcsncmp(info->name, L"https://", 8))) {
            ShellExecuteW(NULL, L"open", info->name, NULL, NULL, SW_SHOWNORMAL);
        } else {
            RetOpenTemplate(unk, info);
        }
        HookBase::LeaveHook();
    }

    Chat::Color* __fastcall OnGetSenderColor(Chat::Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        *color = ChatSenderColor[(int)chan];
        HookBase::LeaveHook();
        return color;
    };

    Chat::Color* __fastcall OnGetMessageColor(Chat::Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        *color = ChatMessageColor[(int)chan];
        HookBase::LeaveHook();
        return color;
    };

    void Init() {
        ChatEvent_pt ChatEvent_Func = (ChatEvent_pt)Scanner::Find("\x83\xFB\x06\x1B", "xxxx", -0x28);
        printf("[SCAN] Chat Event = %p\n", ChatEvent_Func);

        GetChannelColor_pt GetSenderColor_Func = (GetChannelColor_pt)Scanner::Find(
            "\x56\x83\xFA\x0E\x8B\xF1\x0F\x87\x96", "xxxxxxxxx", 0);
        printf("[SCAN] GetSenderColor = %p\n", GetSenderColor_Func);

        GetChannelColor_pt GetMessageColor_Func = (GetChannelColor_pt)Scanner::Find(
            "\x83\xFA\x0E\x8B\xC1\x0F\x87\x83", "xxxxxxxx", 0);
        printf("[SCAN] GetMessageColor = %p\n", GetMessageColor_Func);

        LocalMessage_pt LocalMessage_Func = (LocalMessage_pt)Scanner::Find(
            "\x8D\x55\xF8\xB9\x7E\x00\x00\x10\x6A\x00", "xxxxxxxxxx", -59);
        printf("[SCAN] LocalMessage = %p\n", LocalMessage_Func);

        SendChat_Func = (SendChat_pt)Scanner::Find(
            "\x81\xEC\x1C\x01\x00\x00\x56\x8B\xF2", "xxxxxxxxx", -3);
        printf("[SCAN] SendChat = %p\n", SendChat_Func);

        OpenTemplate_Func = (OpenTemplate_pt)Scanner::Find(
            "\x53\x8B\xDA\x57\x8B\xF9\x8B\x43", "xxxxxxxx", 0);
        printf("[SCAN] OpenTemplate = %p\n", OpenTemplate_Func);

        WriteWhisper_Func = (WriteWhisper_pt)Scanner::Find(
            "\x83\xC6\x2E\x8B\xC6\x83\xC0\x03", "xxxxxxxx", -0x16);
        printf("[SCAN] WriteWhisper = %p\n", WriteWhisper_Func);

        PrintChat_Func = (PrintChat_pt)Scanner::Find(
            "\x83\xEC\x30\x56\x8B\xF1\x57\x81", "xxxxxxxx", -3);
        printf("[SCAN] PrintChat = %p\n", PrintChat_Func);

        {
            uintptr_t address = Scanner::Find(
                "\x8B\xF2\x85\xC0\x8B\xF9\x75\x00\x5F", "xxxxxxx?x", -6);
            printf("[SCAN] ChatBuffer_Addr = %p\n", (void *)address);
            if (Verify(address))
                ChatBuffer_Addr = *(uintptr_t *)address;
        }

        {
            uintptr_t address = Scanner::Find(
                "\xC3\xA1\x00\x00\x00\x00\x85\xC0\x74\x10\xE8", "xx????xxxxx", -8);
            printf("[SCAN] IsTyping_Addr = %p\n", (void *)address);
            if (Verify(address))
                IsTyping_Addr = *(uintptr_t *)address;
        }
    }

    void CreateHooks() {
        if (Verify(ChatEvent_Func))
            HookBase::CreateHook(ChatEvent_Func, OnChatEvent, (void **)&RetChatEvent);
        if (Verify(GetSenderColor_Func))
            HookBase::CreateHook(GetSenderColor_Func, OnGetSenderColor, (void **)&RetGetSenderColor);
        if (Verify(LocalMessage_Func))
            HookBase::CreateHook(LocalMessage_Func, OnLocalMessage, (void **)&RetLocalMessage);
        if (Verify(SendChat_Func))
            HookBase::CreateHook(SendChat_Func, OnSendChat, (void **)&RetSendChat);
        if (Verify(OpenTemplate_Func))
            HookBase::CreateHook(OpenTemplate_Func, OnOpenTemplate, (void **)&RetOpenTemplate);
        if (Verify(WriteWhisper_Func))
            HookBase::CreateHook(WriteWhisper_Func, OnWriteWhisper, (void **)&RetWriteWhisper);
        if (Verify(PrintChat_Func))
            HookBase::CreateHook(PrintChat_Func, OnPrintChat, (void **)&RetPrintChat);
    }

    void RemoveHooks() {
        if (ChatEvent_Func)
            HookBase::RemoveHook(ChatEvent_Func);
        if (GetSenderColor_Func)
            HookBase::RemoveHook(GetSenderColor_Func);
        if (LocalMessage_Func)
            HookBase::RemoveHook(LocalMessage_Func);
        if (SendChat_Func)
            HookBase::RemoveHook(SendChat_Func);
        if (OpenTemplate_Func)
            HookBase::RemoveHook(OpenTemplate_Func);
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
        NULL,           // exit_module
        ::CreateHooks,  // exit_module
        ::RemoveHooks,  // remove_hooks
    };

    void Chat::SetChatEventCallback(std::function<void (uint32_t, uint32_t, wchar_t *, void *)> callback) {
        ChatEvent_callback = callback;
    }

    void Chat::SetLocalMessageCallback(std::function<bool (int, wchar_t *)> callback) {
        LocalMessage_callback = callback;
    }

    void Chat::SetSendChatCallback(std::function<void(Chat::Channel chan, wchar_t *msg)> callback) {
        SendChat_callback = callback;
    }

    void Chat::SetOpenLinks(bool b) {
        open_links = b;
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
        case CHANNEL_ALLIANCE:
            *sender  = COLOR_RGB(0xFF, 0xC0, 0x60);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case CHANNEL_ALLIES:
            *sender  = COLOR_RGB(0x60, 0xA0, 0xFF);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case CHANNEL_GWCA1:
            *sender  = COLOR_RGB(0xC0, 0xD0, 0xFF);
            *message = COLOR_RGB(0xC0, 0xD0, 0xFF);
            break;
        case CHANNEL_ALL:
            *sender  = COLOR_RGB(0xFF, 0xFF, 0x80);
            *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
            break;
        case CHANNEL_GWCA2:
            *sender  = COLOR_RGB(0xCC, 0xCC, 0xCC);
            *message = COLOR_RGB(0xB0, 0xB0, 0xB0);
            break;
        case CHANNEL_MODERATOR:
            *sender  = COLOR_RGB(0xFF, 0x50, 0xDF);
            *message = COLOR_RGB(0x50, 0xFF, 0xDF);
            break;
        case CHANNEL_EMOTE:
            *sender  = COLOR_RGB(0xFF, 0xFF, 0xFF);
            *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
            break;
        case CHANNEL_WARNING:
            *sender  = COLOR_RGB(0xCC, 0xCC, 0xCC);
            *message = COLOR_RGB(0xCC, 0xCC, 0xCC);
            break;
        case CHANNEL_GWCA3:
            *sender  = COLOR_RGB(0xCC, 0xCC, 0xCC);
            *message = COLOR_RGB(0x50, 0xFF, 0xDF);
            break;
        case CHANNEL_GUILD:
            *sender  = COLOR_RGB(0x00, 0xFF, 0x60);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case CHANNEL_GLOBAL:
            *sender  = COLOR_RGB(0x80, 0xFF, 0x80);
            *message = COLOR_RGB(0x80, 0xFF, 0x80);
            break;
        case CHANNEL_GROUP:
            *sender  = COLOR_RGB(0x80, 0xC0, 0xFF);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        case CHANNEL_TRADE:
            *sender  = COLOR_RGB(0xFF, 0xC0, 0xC4);
            *message = COLOR_RGB(0xFF, 0xC4, 0xC0);
            break;
        case CHANNEL_ADVISORY:
            *sender  = COLOR_RGB(0xFF, 0x90, 0x20);
            *message = COLOR_RGB(0xFF, 0x90, 0x20);
            break;
        case CHANNEL_WHISPER:
            *sender  = COLOR_RGB(0x80, 0xC0, 0xFF);
            *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
            break;
        }
    }

    void Chat::SetWhisperCallback(std::function<void (wchar_t *, wchar_t *)> callback) {
        WriteWhisper_callback = callback;
    }

    bool Chat::GetIsTyping() {
        if (!Verify(IsTyping_Addr))
            return false;
        else
            return (*(uint32_t *)IsTyping_Addr == 1);
    }

    void Chat::SendChat(char channel, const wchar_t *msg) {
        assert(SendChat_Func);
        wchar_t buffer[140];

        // We could take 140 char long, but the chat only allow 120 ig.
        size_t len = wcslen(msg);
        len = len > 120 ? 120 : len;

        buffer[0] = channel;
        buffer[len + 1] = 0;
        for (size_t i = 0; i < len; i++)
            buffer[i + 1] = msg[i];

        SendChat_Func(buffer);
    }

    void Chat::SendChat(char channel, const char *msg) {
        assert(SendChat_Func);
        wchar_t buffer[140];

        size_t len = strlen(msg);
        len = len > 120 ? 120 : len;

        buffer[0] = channel;
        buffer[len + 1] = 0;
        for (size_t i = 0; i < len; i++)
            buffer[i + 1] = msg[i];

        SendChat_Func(buffer);
    }

    void Chat::SendChat(const wchar_t *from, const wchar_t *msg) {
        assert(SendChat_Func);
        wchar_t buffer[140];

        if (swprintf(buffer, 140, L"\"%s,%s", from, msg) < 140) {
            buffer[139] = 0;
            SendChat_Func(buffer);
        }
    }

    void Chat::SendChat(const char *from, const char *msg) {
        assert(SendChat_Func);
        wchar_t buffer[140];

        if (swprintf(buffer, 140, L"\"%S,%S", from, msg) < 140) {
            buffer[139] = 0;
            SendChat_Func(buffer);
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

    struct UIChatMessage {
        uint32_t channel;
        wchar_t *message;
        uint32_t channel2;
    };

    void Chat::WriteChat(Channel channel, const wchar_t *msg) {

        size_t len = wcslen(msg);
        wchar_t *buffer = new wchar_t[len + 4];

        UIChatMessage param;
        param.channel = channel;
        param.channel2 = channel;
        param.message = buffer;

        buffer[0] = 0x108;
        buffer[1] = 0x107;
        for (size_t i = 0; i < len; i++)
            buffer[i + 2] = msg[i];

        buffer[len + 2] = 1;
        buffer[len + 3] = 0;

        UI::SendUIMessage(UI::kWriteToChatLog, &param);
        delete[] buffer;
    }

    void Chat::WriteChat(Channel channel, const char *msg) {

        size_t len = strlen(msg);
        wchar_t *buffer = new wchar_t[len + 4];

        UIChatMessage param;
        param.channel = channel;
        param.channel2 = channel;
        param.message = buffer;

        buffer[0] = 0x108;
        buffer[1] = 0x107;
        for (size_t i = 0; i < len; i++)
            buffer[i + 2] = msg[i];

        buffer[len + 2] = 1;
        buffer[len + 3] = 0;

        UI::SendUIMessage(UI::kWriteToChatLog, &param);
        delete[] buffer;
    }

    void Chat::CreateCommand(std::wstring cmd, CmdCB callback) {
        SlashCmdList[cmd] = callback;
    }

    void Chat::DeleteCommand(std::wstring cmd) {
        SlashCmdList.erase(cmd);
    }

    void Chat::ToggleTimestamps(bool enable) {
        ShowTimestamps = enable;
    }

    void Chat::SetTimestampsFormat(bool use_24h) {
        Timestamp_24hFormat = use_24h;
    }

    void Chat::SetTimestampsColor(Color color) {
        TimestampsColor = color;
    }

} // namespace GW