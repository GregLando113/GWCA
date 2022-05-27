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

#define COLOR_ARGB(a, r, g, b) (GW::Color)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xff, r, g, b)

namespace {
    using namespace GW;
    using namespace Chat;

    bool ShowTimestamps = false;
    bool Timestamp_24hFormat = false;
    bool Timestamp_seconds = false;
    Color TimestampsColor = COLOR_RGB(0xff, 0xff, 0xff);

    std::unordered_map<std::wstring, CmdCB> SlashCmdList;

    // 08 01 07 01 [Time] 01 00 02 00
    // ChatBuffer **ChatBuffer_Addr;
    ChatBuffer** ChatBuffer_Addr;
    uintptr_t IsTyping_Addr;

    // There is maybe more.
    // Though, we can probably fix this.
    bool DoesChannelParseColorTag(Chat::Channel in) {
        switch (in) {
        case Chat::Channel::CHANNEL_WARNING:
        case Chat::Channel::CHANNEL_ADVISORY:
            return false;
        }
        return true;
    }



    Color ChatSenderColor[] = {
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

    Color ChatMessageColor[] = {
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

    std::unordered_map<HookEntry *, SendChatCallback>     SendChat_callbacks;
    std::unordered_map<HookEntry *, ChatEventCallback>    ChatEvent_callbacks;
    std::unordered_map<HookEntry *, LocalMessageCallback> LocalMessage_callbacks;
    std::unordered_map<HookEntry *, WhisperCallback>      Whisper_callbacks;
    std::unordered_map<HookEntry*, PrintChatCallback>     PrintChat_callbacks;
    std::unordered_map<HookEntry *, StartWhisperCallback> StartWhisper_callbacks;
    struct ChatLogCallbackEntry {
        int altitude;
        HookEntry* entry;
        ChatLogCallback callback;
    };
    std::vector<ChatLogCallbackEntry> ChatLog_callbacks;

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

    typedef Color* (__cdecl *GetChannelColor_pt)(Color *color, Chat::Channel chan);
    GetChannelColor_pt RetGetSenderColor;
    GetChannelColor_pt GetSenderColor_Func;
    Color* __cdecl OnGetSenderColor(Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        *color = ChatSenderColor[(int)chan];
        HookBase::LeaveHook();
        return color;
    };


    GetChannelColor_pt RetGetMessageColor;
    GetChannelColor_pt GetMessageColor_Func;
    Color* __cdecl OnGetMessageColor(Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        *color = ChatMessageColor[(int)chan];
        HookBase::LeaveHook();
        return color;
    };

    typedef void(__cdecl *LocalMessage_pt)(Chat::Channel channel, wchar_t *message);
    LocalMessage_pt LocalMessage_Func;
    LocalMessage_pt RetLocalMessage;
    void __cdecl OnLocalMessage(Chat::Channel channel, wchar_t *message) {
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

    typedef void(__cdecl *SendChat_pt)(wchar_t *message, AgentID agent_id);
    SendChat_pt SendChat_Func;
    SendChat_pt RetSendChat;
    void __cdecl OnSendChat(wchar_t *message, AgentID agent_id = AgentID::None) {
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

    bool add_next_message_to_chat_log = true;

    typedef void(_cdecl* AddToChatLog_pt)(wchar_t* message, Chat::Channel channel);
    AddToChatLog_pt AddToChatLog_Func;
    AddToChatLog_pt RetAddToChatLog;
    void OnAddToChatLog(wchar_t* message, Chat::Channel channel) {
        GW::HookBase::EnterHook();
        HookStatus status;
        auto it = ChatLog_callbacks.begin();

        status.blocked = !add_next_message_to_chat_log;

        ChatMessage* logged_message = 0;
        // Pre callbacks
        while (it != ChatLog_callbacks.end()) {
            if (it->altitude > 0)
                break;
            it->callback(&status, message,channel, logged_message);
            ++status.altitude;
            it++;
        }

        if (!status.blocked) {
            ChatBuffer* log = GetChatLog();
            size_t log_idx = log ? log->next : 0;
            RetAddToChatLog(message, channel);
            if(log && log->next != log_idx)
                logged_message = log->messages[log_idx];
        }

        // Post callbacks
        while (it != ChatLog_callbacks.end()) {
            it->callback(&status, message, channel, logged_message);
            ++status.altitude;
            it++;
        }
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
        GWCA_ASSERT(ChatBuffer_Addr && 0 <= (uint32_t)channel && channel < Chat::Channel::CHANNEL_COUNT);
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


        wchar_t time_buffer[29];
        if (localtime.wYear == 0) {
            Timestamp_seconds ? std::memcpy(time_buffer, L"[lbracket]--:--:--[rbracket]", sizeof(wchar_t) * 29)
                              : std::memcpy(time_buffer, L"[lbracket]--:--[rbracket]", sizeof(wchar_t) * 26);
        }
        else {
            if(Timestamp_seconds)
                swprintf(time_buffer, 29, L"[lbracket]%02d:%02d:%02d[rbracket]", hour, minute, second);
            else
                swprintf(time_buffer, 29, L"[lbracket]%02d:%02d[rbracket]", hour, minute);
        }
        wchar_t* message_buffer;
        size_t buf_len = 21 + 29 + wcslen(*str_p);
        message_buffer = new wchar_t[buf_len];
        if (DoesChannelParseColorTag(channel)) {
            swprintf(message_buffer, buf_len, L"\x108\x107<c=#%06x>%s </c>\x01\x02%s", (TimestampsColor & 0x00FFFFFF), time_buffer, *str_p);
        } else {
            swprintf(message_buffer, buf_len, L"\x108\x107%s \x01\x02%s", time_buffer, *str_p);
        }
        RetPrintChat(ctx, edx, channel, message_buffer, timestamp, reprint);
        HookBase::LeaveHook();
        delete[] message_buffer;
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
            AddToChatLog_Func = (AddToChatLog_pt)GW::Scanner::Find(
                "\x40\x25\xff\x01\x00\x00", "xxxxxx", -0x97);
            GWCA_INFO("[SCAN] AddToChatLog_Func = %p\n", AddToChatLog_Func);
        }

        {
            ChatBuffer_Addr = *(ChatBuffer***)Scanner::Find(
                "\x8B\x45\x08\x83\x7D\x0C\x07\x74", "xxxxxxxx", -4);
            GWCA_INFO("[SCAN] ChatBuffer_Addr = %p\n", (void *)ChatBuffer_Addr);
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
        if (Verify(AddToChatLog_Func))
            HookBase::CreateHook(AddToChatLog_Func, OnAddToChatLog, (void**)&RetAddToChatLog);
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
        if (AddToChatLog_Func)
            HookBase::RemoveHook(AddToChatLog_Func);
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
    namespace Chat {
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
        void RegisterSendChatCallback(
            HookEntry* entry,
            SendChatCallback callback)
        {
            SendChat_callbacks.insert({ entry, callback });
        }

        void RemoveSendChatCallback(
            HookEntry* entry)
        {
            auto it = SendChat_callbacks.find(entry);
            if (it != SendChat_callbacks.end())
                SendChat_callbacks.erase(it);
        }

        void RegisterChatEventCallback(
            HookEntry* entry,
            ChatEventCallback callback)
        {
            ChatEvent_callbacks.insert({ entry, callback });
        }

        void RemoveChatEventCallback(
            HookEntry* entry)
        {
            auto it = ChatEvent_callbacks.find(entry);
            if (it != ChatEvent_callbacks.end())
                ChatEvent_callbacks.erase(it);
        }

        void RegisterLocalMessageCallback(
            HookEntry* entry,
            LocalMessageCallback callback)
        {
            LocalMessage_callbacks.insert({ entry, callback });
        }

        void RemoveLocalMessageCallback(
            HookEntry* entry)
        {
            auto it = LocalMessage_callbacks.find(entry);
            if (it != LocalMessage_callbacks.end())
                LocalMessage_callbacks.erase(it);
        }

        void RegisterWhisperCallback(
            HookEntry* entry,
            WhisperCallback callback)
        {
            Whisper_callbacks.insert({ entry, callback });
        }

        void RegisterPrintChatCallback(
            HookEntry* entry,
            PrintChatCallback callback)
        {
            PrintChat_callbacks.insert({ entry, callback });
        }
        void RegisterChatLogCallback(
            HookEntry* entry,
            ChatLogCallback callback,
            int altitude)
        {
            ChatLog_callbacks.push_back({ altitude, entry, callback });
        }
        void RemoveChatLogCallback(
            HookEntry* entry)
        {
            auto it = ChatLog_callbacks.begin();
            while (it != ChatLog_callbacks.end()) {
                if (it->entry == entry) {
                    ChatLog_callbacks.erase(it);
                    break;
                }
                it++;
            }
        }

        void RemoveRegisterWhisperCallback(
            HookEntry* entry)
        {
            auto it = Whisper_callbacks.find(entry);
            if (it != Whisper_callbacks.end())
                Whisper_callbacks.erase(it);
        }

        void RegisterStartWhisperCallback(
            HookEntry* entry,
            StartWhisperCallback callback)
        {
            StartWhisper_callbacks.insert({ entry, callback });
        }

        void RemoveStartWhisperCallback(
            HookEntry* entry)
        {
            auto it = StartWhisper_callbacks.find(entry);
            if (it != StartWhisper_callbacks.end())
                StartWhisper_callbacks.erase(it);
        }

        ChatBuffer* GetChatLog() {
            return *ChatBuffer_Addr;
        }

        void AddToChatLog(wchar_t* message, Chat::Channel channel) {
            if (AddToChatLog_Func)
                AddToChatLog_Func(message, channel);
        }

        GW::Color SetSenderColor(Chat::Channel chan, GW::Color col) {
            GW::Color old = ChatSenderColor[(int)chan];
            ChatSenderColor[(int)chan] = col;
            return old;
        }

        GW::Color SetMessageColor(Chat::Channel chan, GW::Color col) {
            GW::Color old = ChatMessageColor[(int)chan];
            ChatMessageColor[(int)chan] = col;
            return old;
        }

        void GetChannelColors(Chat::Channel chan, GW::Color* sender, GW::Color* message) {
            *sender = ChatSenderColor[(int)chan];
            *message = ChatMessageColor[(int)chan];
        }

        void GetDefaultColors(Chat::Channel chan, GW::Color* sender, GW::Color* message) {
            switch (chan) {
            case Chat::Channel::CHANNEL_ALLIANCE:
                *sender = COLOR_RGB(0xFF, 0xC0, 0x60);
                *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
                break;
            case Chat::Channel::CHANNEL_ALLIES:
                *sender = COLOR_RGB(0x60, 0xA0, 0xFF);
                *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
                break;
            case Chat::Channel::CHANNEL_GWCA1:
                *sender = COLOR_RGB(0xC0, 0xD0, 0xFF);
                *message = COLOR_RGB(0xC0, 0xD0, 0xFF);
                break;
            case Chat::Channel::CHANNEL_ALL:
                *sender = COLOR_RGB(0xFF, 0xFF, 0x80);
                *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
                break;
            case Chat::Channel::CHANNEL_GWCA2:
                *sender = COLOR_RGB(0xCC, 0xCC, 0xCC);
                *message = COLOR_RGB(0xB0, 0xB0, 0xB0);
                break;
            case Chat::Channel::CHANNEL_MODERATOR:
                *sender = COLOR_RGB(0xFF, 0x50, 0xDF);
                *message = COLOR_RGB(0x50, 0xFF, 0xDF);
                break;
            case Chat::Channel::CHANNEL_EMOTE:
                *sender = COLOR_RGB(0xFF, 0xFF, 0xFF);
                *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
                break;
            case Chat::Channel::CHANNEL_WARNING:
                *sender = COLOR_RGB(0xCC, 0xCC, 0xCC);
                *message = COLOR_RGB(0xCC, 0xCC, 0xCC);
                break;
            case Chat::Channel::CHANNEL_GWCA3:
                *sender = COLOR_RGB(0xCC, 0xCC, 0xCC);
                *message = COLOR_RGB(0x50, 0xFF, 0xDF);
                break;
            case Chat::Channel::CHANNEL_GUILD:
                *sender = COLOR_RGB(0x00, 0xFF, 0x60);
                *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
                break;
            case Chat::Channel::CHANNEL_GLOBAL:
                *sender = COLOR_RGB(0x80, 0xFF, 0x80);
                *message = COLOR_RGB(0x80, 0xFF, 0x80);
                break;
            case Chat::Channel::CHANNEL_GROUP:
                *sender = COLOR_RGB(0x80, 0xC0, 0xFF);
                *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
                break;
            case Chat::Channel::CHANNEL_TRADE:
                *sender = COLOR_RGB(0xFF, 0xC0, 0xC4);
                *message = COLOR_RGB(0xFF, 0xC4, 0xC0);
                break;
            case Chat::Channel::CHANNEL_ADVISORY:
                *sender = COLOR_RGB(0xFF, 0x90, 0x20);
                *message = COLOR_RGB(0xFF, 0x90, 0x20);
                break;
            case Chat::Channel::CHANNEL_WHISPER:
                *sender = COLOR_RGB(0x80, 0xC0, 0xFF);
                *message = COLOR_RGB(0xE0, 0xE0, 0xE0);
                break;
            default:
                *sender = COLOR_RGB(0xFF, 0xFF, 0x80);
                *message = COLOR_RGB(0xFF, 0xFF, 0xFF);
            }
        }

        bool GetIsTyping() {
            if (!Verify(IsTyping_Addr))
                return false;
            else
                return (*(uint32_t*)IsTyping_Addr == 1);
        }

        void SendChat(char channel, const wchar_t* msg) {
            GWCA_ASSERT(SendChat_Func);
            wchar_t buffer[140];

            // We could take 140 char long, but the chat only allow 120 ig.
            size_t len = wcslen(msg);
            len = len > 120 ? 120 : len;

            buffer[0] = static_cast<wchar_t>(channel);
            buffer[len + 1] = 0;
            for (size_t i = 0; i < len; i++)
                buffer[i + 1] = static_cast<wchar_t>(msg[i]);

            OnSendChat(buffer);
        }

        void SendChat(char channel, const char* msg) {
            GWCA_ASSERT(SendChat_Func);
            wchar_t buffer[140];

            size_t len = strlen(msg);
            len = len > 120 ? 120 : len;

            buffer[0] = static_cast<wchar_t>(channel);
            buffer[len + 1] = 0;
            for (size_t i = 0; i < len; i++)
                buffer[i + 1] = static_cast<wchar_t>(msg[i]);

            OnSendChat(buffer);
        }

        void SendChat(const wchar_t* from, const wchar_t* msg) {
            GWCA_ASSERT(SendChat_Func);
            wchar_t buffer[140];

            if (swprintf(buffer, 140, L"\"%s,%s", from, msg) < 140) {
                buffer[139] = 0;
                OnSendChat(buffer);
            }
        }

        void SendChat(const char* from, const char* msg) {
            GWCA_ASSERT(SendChat_Func);
            wchar_t buffer[140];

            if (swprintf(buffer, 140, L"\"%S,%S", from, msg) < 140) {
                buffer[139] = 0;
                OnSendChat(buffer);
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



        void WriteChat(Chat::Channel channel, const wchar_t* message_unencoded, const wchar_t* sender_unencoded, bool transient) {
            size_t len = wcslen(message_unencoded) + 4;
            wchar_t* message_encoded = new wchar_t[len];
            GWCA_ASSERT(swprintf(message_encoded, len, L"\x108\x107%s\x1", message_unencoded) >= 0);
            wchar_t* sender_encoded = 0;
            if (sender_unencoded) {
                len = wcslen(sender_unencoded) + 4;
                sender_encoded = new wchar_t[len];
                GWCA_ASSERT(swprintf(sender_encoded, len, L"\x108\x107%s\x1", sender_unencoded) >= 0);
            }
            WriteChatEnc(channel, message_encoded, sender_encoded, transient);
            delete[] message_encoded;
            if (sender_encoded)
                delete[] sender_encoded;
        }
        void WriteChatEnc(Chat::Channel channel, const wchar_t* message_encoded, const wchar_t* sender_encoded, bool transient) {
            UI::UIChatMessage param;
            param.channel = param.channel2 = channel;
            param.message = (wchar_t*)message_encoded;
            bool delete_message = false;
            if (sender_encoded) {
                // If message contains link (<a=1>), manually create the message string
                const wchar_t* format = L"\x76b\x10a%s\x1\x10b%s\x1";
                size_t len = wcslen(message_encoded) + wcslen(sender_encoded) + 6;
                bool has_link_in_message = wcsstr(message_encoded, L"<a=1>") != 0;
                bool has_markup = has_link_in_message || wcsstr(message_encoded, L"<c=") != 0;
                if (has_markup) {
                    // NB: When not using this method, any skill templates etc are NOT rendered by the game
                    if (has_link_in_message) {
                        format = L"\x108\x107<a=2>\x1\x2%s\x2\x108\x107</a>\x1\x2\x108\x107: \x1\x2%s";
                    }
                    else {
                        format = L"\x108\x107<a=1>\x1\x2%s\x2\x108\x107</a>\x1\x2\x108\x107: \x1\x2%s";
                    }
                    len += 19;
                }
                param.message = new wchar_t[len];
                delete_message = true;
                GWCA_ASSERT(swprintf(param.message, len, format, sender_encoded, message_encoded) >= 0);
            }
            add_next_message_to_chat_log = !transient;
            UI::SendUIMessage(UI::UIMessage::kWriteToChatLog, &param);
            add_next_message_to_chat_log = true;
            if (delete_message)
                delete[] param.message;
        }

        void CreateCommand(std::wstring cmd, CmdCB callback) {
            ::wstring_tolower(cmd);
            SlashCmdList[cmd] = callback;
        }

        void DeleteCommand(std::wstring cmd) {
            ::wstring_tolower(cmd);
            SlashCmdList.erase(cmd);
        }

        void ToggleTimestamps(bool enable) {
            ShowTimestamps = enable;
            if (ShowTimestamps)
                UI::SetPreference(UI::Preference::ShowChatTimestamps, 0);
        }

        void SetTimestampsFormat(bool use_24h, bool show_timestamp_seconds) {
            Timestamp_24hFormat = use_24h;
            Timestamp_seconds = show_timestamp_seconds;
        }

        void SetTimestampsColor(Color color) {
            TimestampsColor = color;
        }
    }
} // namespace GW
