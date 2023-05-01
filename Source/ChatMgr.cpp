#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/ChatMgr.h>

#define COLOR_ARGB(a, r, g, b) (GW::Chat::Color)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xff, r, g, b)

namespace {
    using namespace GW;



    bool ShowTimestamps = false;
    bool Timestamp_24hFormat = false;
    bool Timestamp_seconds = false;
    Chat::Color TimestampsColor = COLOR_RGB(0xff, 0xff, 0xff);

    std::unordered_map<std::wstring, Chat::CmdCB> SlashCmdList;

    // 08 01 07 01 [Time] 01 00 02 00
    // ChatBuffer **ChatBuffer_Addr;
    Chat::ChatBuffer** ChatBuffer_Addr = nullptr;
    uint32_t* IsTyping_FrameId = nullptr;

    // There is maybe more.
    // Though, we can probably fix this.
    bool ChannelThatParseColorTag[] = {
        true, true, true, true, true, true, true,
        false, // WARNING
        true, true, true, true, true,
        false, // ADVISORY
        true
    };

    std::map<Chat::Channel, Chat::Color> ChatSenderColor;
    std::map<Chat::Channel, Chat::Color> ChatMessageColor;

    void wstring_tolower(std::wstring& s)
    {
        for (size_t i = 0; i < s.size(); i++)
            s[i] = towlower(s[i]);
    }

    std::unordered_map<HookEntry *, Chat::SendChatCallback>     SendChat_callbacks;
    std::unordered_map<HookEntry *, Chat::LocalMessageCallback> LocalMessage_callbacks;
    std::unordered_map<HookEntry *, Chat::WhisperCallback>      Whisper_callbacks;
    std::unordered_map<HookEntry*, Chat::PrintChatCallback>     PrintChat_callbacks;
    std::unordered_map<HookEntry *, Chat::StartWhisperCallback> StartWhisper_callbacks;
    struct ChatLogCallbackEntry {
        int altitude;
        HookEntry* entry;
        Chat::ChatLogCallback callback;
    };
    std::vector<ChatLogCallbackEntry> ChatLog_callbacks;

    typedef Chat::Color* (__cdecl *GetChannelColor_pt)(Chat::Color *color, Chat::Channel chan);
    GetChannelColor_pt RetGetSenderColor;
    GetChannelColor_pt GetSenderColor_Func;
    Chat::Color* __cdecl OnGetSenderColor(Chat::Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        const auto it = ChatSenderColor.find(chan);
        if (it != ChatSenderColor.end()) {
            *color = it->second;
        }
        else {
            RetGetSenderColor(color, chan);
        }
        HookBase::LeaveHook();
        return color;
    };


    GetChannelColor_pt RetGetMessageColor;
    GetChannelColor_pt GetMessageColor_Func;
    Chat::Color* __cdecl OnGetMessageColor(Chat::Color *color, Chat::Channel chan) {
        HookBase::EnterHook();
        const auto it = ChatMessageColor.find(chan);
        if (it != ChatMessageColor.end()) {
            *color = it->second;
        }
        else {
            RetGetMessageColor(color, chan);
        }
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
            it.second(&status, GW::Chat::GetChannel(*message), &message[1]);
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

    typedef void(_cdecl* AddToChatLog_pt)(wchar_t* message, uint32_t channel);
    AddToChatLog_pt AddToChatLog_Func;
    AddToChatLog_pt RetAddToChatLog;
    void OnAddToChatLog(wchar_t* message, uint32_t channel) {
        GW::HookBase::EnterHook();
        HookStatus status;
        auto it = ChatLog_callbacks.begin();

        status.blocked = !add_next_message_to_chat_log;

        Chat::ChatMessage* logged_message = 0;
        // Pre callbacks
        while (it != ChatLog_callbacks.end()) {
            if (it->altitude > 0)
                break;
            it->callback(&status, message,channel, logged_message);
            ++status.altitude;
            it++;
        }

        if (!status.blocked) {
            GW::Chat::ChatBuffer* log = Chat::GetChatLog();
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
        if (ChannelThatParseColorTag[channel]) {
            swprintf(message_buffer, buf_len, L"\x108\x107<c=#%06x>%s </c>\x01\x02%s", (TimestampsColor & 0x00FFFFFF), time_buffer, *str_p);
        } else {
            swprintf(message_buffer, buf_len, L"\x108\x107%s \x01\x02%s", time_buffer, *str_p);
        }
        RetPrintChat(ctx, edx, channel, message_buffer, timestamp, reprint);
        HookBase::LeaveHook();
        delete[] message_buffer;
    }


    UI::UIInteractionCallback UICallback_AssignEditableText_Func = nullptr;
    UI::UIInteractionCallback UICallback_AssignEditableText_Ret = nullptr;
    // When a control is terminated ( message 0xB ) it doesn't clear the IsTyping_FrameId that we're using. Clear it manually.
    void OnUICallback_AssignEditableText(UI::InteractionMessage* message, void* wParam, void* lParam) {
        HookBase::EnterHook();
        if (message->message_id == 0xb && IsTyping_FrameId && *IsTyping_FrameId == message->action_type) {
            *IsTyping_FrameId = 0;
            //GWCA_INFO("IsTyping_FrameId manually cleared");
        }
        UICallback_AssignEditableText_Ret(message, wParam, lParam);
        HookBase::LeaveHook();
    }

    void Init() {
        GetSenderColor_Func = (GetChannelColor_pt)Scanner::Find("\xC7\x00\x60\xC0\xFF\xFF\x5D\xC3", "xxxxxxxx", -0x1C);
        GetMessageColor_Func = (GetChannelColor_pt)Scanner::Find("\xC7\x00\xB0\xB0\xB0\xFF\x5D\xC3", "xxxxxxxx", -0x27);

        // The last 4 bytes of the patterns are the "SendUIMessage" message id (i.e. 0x1000007E)
        LocalMessage_Func = (LocalMessage_pt)Scanner::Find("\x8D\x45\xF8\x6A\x00\x50\x68\x7E\x00\x00\x10", "xxxxxxxxxxx", -0x3D);
        SendChat_Func = (SendChat_pt)Scanner::Find("\x8D\x85\xE0\xFE\xFF\xFF\x50\x68\x1C\x01", "xxxxxxxxx", -0x3E);
        StartWhisper_Func = (StartWhisper_pt)GW::Scanner::Find("\xFC\x53\x56\x8B\xF1\x57\x6A\x05\xFF\x36\xE8", "xxxxxxxxxxx", -0xF);
        WriteWhisper_Func = (WriteWhisper_pt)Scanner::Find("\x83\xC4\x04\x8D\x58\x2E", "xxxxxx", -0x18);
        PrintChat_Func = (PrintChat_pt)Scanner::Find("\x3D\x00\x00\x00\x00\x73\x2B\x6A", "x??xxxxx", -0x46);
        AddToChatLog_Func = (AddToChatLog_pt)GW::Scanner::Find("\x40\x25\xff\x01\x00\x00", "xxxxxx", -0x97);
        ChatBuffer_Addr = *(Chat::ChatBuffer***)Scanner::Find("\x8B\x45\x08\x83\x7D\x0C\x07\x74", "xxxxxxxx", -4);




        uintptr_t address = Scanner::FindAssertion("p:\\code\\engine\\controls\\ctledit.cpp","charCount >= 1",0x37);
        if (address && Scanner::IsValidPtr(*(uintptr_t*) address))
            IsTyping_FrameId = *(uint32_t **)address;

        address = Scanner::Find("\x6a\x06\x68\x00\x03\x80\x00","xxxxxxx",-0x4);
        if (address && Scanner::IsValidPtr(*(uintptr_t*)address, Scanner::TEXT))
            UICallback_AssignEditableText_Func = *(UI::UIInteractionCallback*)address;

        GWCA_INFO("[SCAN] GetSenderColor = %p", GetSenderColor_Func);
        GWCA_INFO("[SCAN] GetMessageColor = %p", GetMessageColor_Func);
        GWCA_INFO("[SCAN] LocalMessage = %p", LocalMessage_Func);
        GWCA_INFO("[SCAN] SendChat = %p", SendChat_Func);
        GWCA_INFO("[SCAN] StartWhisper = %p", StartWhisper_Func);
        GWCA_INFO("[SCAN] WriteWhisper = %p", WriteWhisper_Func);
        GWCA_INFO("[SCAN] PrintChat = %p", PrintChat_Func);
        GWCA_INFO("[SCAN] AddToChatLog_Func = %p", AddToChatLog_Func);
        GWCA_INFO("[SCAN] ChatBuffer_Addr = %p", ChatBuffer_Addr);
        GWCA_INFO("[SCAN] IsTyping_FrameId = %p", IsTyping_FrameId);
        GWCA_INFO("[SCAN] UICallback_AssignEditableText_Func = %p", UICallback_AssignEditableText_Func);

#ifdef _DEBUG
        GWCA_ASSERT(GetSenderColor_Func);
        GWCA_ASSERT(GetMessageColor_Func);
        GWCA_ASSERT(LocalMessage_Func);
        GWCA_ASSERT(SendChat_Func);
        GWCA_ASSERT(StartWhisper_Func);
        GWCA_ASSERT(WriteWhisper_Func);
        GWCA_ASSERT(PrintChat_Func);
        GWCA_ASSERT(AddToChatLog_Func);
        GWCA_ASSERT(ChatBuffer_Addr);
        GWCA_ASSERT(IsTyping_FrameId);
        GWCA_ASSERT(UICallback_AssignEditableText_Func);
#endif

        HookBase::CreateHook(StartWhisper_Func, OnStartWhisper, (void**)& RetStartWhisper);
        HookBase::CreateHook(GetSenderColor_Func, OnGetSenderColor, (void **)&RetGetSenderColor);
        HookBase::CreateHook(GetMessageColor_Func, OnGetMessageColor, (void **)&RetGetMessageColor);
        HookBase::CreateHook(LocalMessage_Func, OnLocalMessage, (void **)&RetLocalMessage);
        HookBase::CreateHook(SendChat_Func, OnSendChat, (void **)&RetSendChat);
        HookBase::CreateHook(WriteWhisper_Func, OnWriteWhisper, (void **)&RetWriteWhisper);
        HookBase::CreateHook(PrintChat_Func, OnPrintChat, (void **)&RetPrintChat);
        HookBase::CreateHook(AddToChatLog_Func, OnAddToChatLog, (void**)&RetAddToChatLog);
        HookBase::CreateHook(UICallback_AssignEditableText_Func, OnUICallback_AssignEditableText, (void**)& UICallback_AssignEditableText_Ret);
    }

    void EnableHooks() {
        if (StartWhisper_Func)
            HookBase::EnableHooks(StartWhisper_Func);
        if (GetSenderColor_Func)
            HookBase::EnableHooks(GetSenderColor_Func);
        if (GetMessageColor_Func)
            HookBase::EnableHooks(GetMessageColor_Func);
        if (LocalMessage_Func)
            HookBase::EnableHooks(LocalMessage_Func);
        if (SendChat_Func)
            HookBase::EnableHooks(SendChat_Func);
        if (WriteWhisper_Func)
            HookBase::EnableHooks(WriteWhisper_Func);
        if (PrintChat_Func)
            HookBase::EnableHooks(PrintChat_Func);
        if (AddToChatLog_Func)
            HookBase::EnableHooks(AddToChatLog_Func);
        if (UICallback_AssignEditableText_Func)
            HookBase::EnableHooks(UICallback_AssignEditableText_Func);
    }
    void DisableHooks() {
        if (StartWhisper_Func)
            HookBase::DisableHooks(StartWhisper_Func);
        if (GetSenderColor_Func)
            HookBase::DisableHooks(GetSenderColor_Func);
        if (GetMessageColor_Func)
            HookBase::DisableHooks(GetMessageColor_Func);
        if (LocalMessage_Func)
            HookBase::DisableHooks(LocalMessage_Func);
        if (SendChat_Func)
            HookBase::DisableHooks(SendChat_Func);
        if (WriteWhisper_Func)
            HookBase::DisableHooks(WriteWhisper_Func);
        if (PrintChat_Func)
            HookBase::DisableHooks(PrintChat_Func);
        if (AddToChatLog_Func)
            HookBase::DisableHooks(AddToChatLog_Func);
        if(UICallback_AssignEditableText_Func)
            HookBase::DisableHooks(UICallback_AssignEditableText_Func);
    }

    void Exit() {
        HookBase::RemoveHook(StartWhisper_Func);
        HookBase::RemoveHook(GetSenderColor_Func);
        HookBase::RemoveHook(GetMessageColor_Func);
        HookBase::RemoveHook(LocalMessage_Func);
        HookBase::RemoveHook(SendChat_Func);
        HookBase::RemoveHook(WriteWhisper_Func);
        HookBase::RemoveHook(PrintChat_Func);
        HookBase::RemoveHook(AddToChatLog_Func);
        HookBase::RemoveHook(UICallback_AssignEditableText_Func);
    }
}

namespace GW {

    Module ChatModule = {
        "ChatModule",   // name
        NULL,           // param
        ::Init,         // init_module
        ::Exit,         // exit_module
        ::EnableHooks,           // enable_hooks
        ::DisableHooks, // disable_hooks
    };

    Chat::Channel Chat::GetChannel(char opcode) {
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
    Chat::Channel Chat::GetChannel(wchar_t opcode) {
        return GetChannel((char)opcode);
    }
    void Chat::RegisterSendChatCallback(
        HookEntry *entry,
        const SendChatCallback& callback)
    {
        SendChat_callbacks.insert({entry, callback});
    }

    void Chat::RemoveSendChatCallback(
        HookEntry* entry)
    {
        auto it = SendChat_callbacks.find(entry);
        if (it != SendChat_callbacks.end())
            SendChat_callbacks.erase(it);
    }

    void Chat::RegisterLocalMessageCallback(
        HookEntry *entry,
        const LocalMessageCallback& callback)
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
        const WhisperCallback& callback)
    {
        Whisper_callbacks.insert({entry, callback});
    }

    void Chat::RegisterPrintChatCallback(
        HookEntry* entry,
        const PrintChatCallback& callback)
        {
            PrintChat_callbacks.insert({ entry, callback });
        }
    void Chat::RegisterChatLogCallback(
        HookEntry* entry,
        const ChatLogCallback& callback,
        int altitude)
    {
        ChatLog_callbacks.push_back({ altitude, entry, callback });
    }
    void Chat::RemoveChatLogCallback(
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

    void Chat::RemoveWhisperCallback(
        HookEntry *entry)
    {
        auto it = Whisper_callbacks.find(entry);
        if (it != Whisper_callbacks.end())
            Whisper_callbacks.erase(it);
    }

    void Chat::RegisterStartWhisperCallback(
        HookEntry* entry,
        const StartWhisperCallback& callback)
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

    Chat::ChatBuffer* Chat::GetChatLog() {
        return *ChatBuffer_Addr;
    }

    void Chat::AddToChatLog(wchar_t* message, uint32_t channel) {
        if (AddToChatLog_Func)
            AddToChatLog_Func(message, channel);
    }

    Chat::Color Chat::SetSenderColor(Channel chan, Color col) {
        Color old = 0;
        GetChannelColors(chan, &old, 0);
        ChatSenderColor[chan] = col;
        return old;
    }

    Chat::Color Chat::SetMessageColor(Channel chan, Color col) {
        Color old = 0;
        GetChannelColors(chan, 0, &old);
        ChatMessageColor[chan] = col;
        return old;
    }

    void Chat::GetChannelColors(Channel chan, Color *sender, Color *message) {
        if (sender) OnGetSenderColor(sender, chan);
        if (message) OnGetMessageColor(message, chan);
    }

    void Chat::GetDefaultColors(Channel chan, Color *sender, Color *message) {
        RetGetMessageColor(message, chan);
        RetGetSenderColor(sender, chan);
    }

    bool Chat::GetIsTyping() {
        return IsTyping_FrameId && *IsTyping_FrameId != 0;
    }

    bool Chat::SendChat(char channel, const wchar_t *msg) {
        if (!(SendChat_Func && msg && *msg && GetChannel(channel) != Channel::CHANNEL_UNKNOW))
            return false;

        wchar_t buffer[140];

        // We could take 140 char long, but the chat only allow 120 ig.
        size_t len = wcslen(msg);
        len = len > 120 ? 120 : len;

        buffer[0] = static_cast<wchar_t>(channel);
        wcsncpy(&buffer[1], msg, len);
        buffer[len + 1] = 0;
        SendChat_Func(buffer, 0);
        return true;
    }

    bool Chat::SendChat(char channel, const char *msg) {
        wchar_t buffer[140];
        int written = swprintf(buffer, _countof(buffer), L"%S", msg);
        if (!(written > 0 && written < 140))
            return false;
        buffer[written] = 0;
        return SendChat(channel, buffer);
    }

    bool Chat::SendChat(const wchar_t *from, const wchar_t *msg) {
        wchar_t buffer[140];
        if (!(SendChat_Func && from && *from && msg && *msg))
            return false;
        int written = swprintf(buffer, _countof(buffer), L"\"%s,%s", from, msg);
        if (!(written > 0 && written < 140))
            return false;
        buffer[written] = 0;
        SendChat_Func(buffer, 0);
        return true;
    }

    bool Chat::SendChat(const char *from, const char *msg) {
        GWCA_ASSERT(SendChat_Func);
        wchar_t buffer[140];
        if (!(SendChat_Func && from && *from && msg && *msg))
            return false;
        int written = swprintf(buffer, _countof(buffer), L"\"%S,%S", from, msg);
        if (!(written > 0 && written < 140))
            return false;
        buffer[written] = 0;
        SendChat_Func(buffer, 0);
        return true;
    }

    // Change to WriteChatF(Channel chan, const wchar_t *from, const wchar_t *frmt, ..)
    // and       WriteChat(Channel chan, const wchar_t *from, const wchar_t *msg)
    
    void Chat::WriteChatF(Channel channel, const wchar_t* format, ...) {
        va_list vl;
        va_start(vl, format);
        size_t szbuf = vswprintf(NULL,0,format, vl) + 1;
        wchar_t* chat = new wchar_t[szbuf];
        vswprintf(chat, szbuf, format, vl);
        va_end(vl);

        WriteChat(channel, chat);
        delete[] chat;
    }



    void Chat::WriteChat(Channel channel, const wchar_t *message_unencoded, const wchar_t *sender_unencoded, bool transient) {
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
        if(sender_encoded)
            delete[] sender_encoded;
    }
    void Chat::WriteChatEnc(Channel channel, const wchar_t* message_encoded, const wchar_t* sender_encoded, bool transient) {
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

    void Chat::CreateCommand(const wchar_t* cmd, const CmdCB& callback)
    {
        std::wstring cpy{cmd};
        ::wstring_tolower(cpy);
        SlashCmdList[std::move(cpy)] = callback;
    }

    Chat::CmdCB Chat::GetCommand(const wchar_t* cmd)
    {
        std::wstring cpy{cmd};
        ::wstring_tolower(cpy);
        if (SlashCmdList.find(cpy) != SlashCmdList.end()) {
            return SlashCmdList[std::move(cpy)];
        }
        return nullptr;
    }

    void Chat::DeleteCommand(const wchar_t* cmd)
    {
        std::wstring cpy{cmd};
        ::wstring_tolower(cpy);
        SlashCmdList.erase(cpy);
    }

    void Chat::ToggleTimestamps(bool enable) {
        ShowTimestamps = enable;
        if (ShowTimestamps)
            UI::SetPreference(UI::FlagPreference::ShowChatTimestamps, 0);
    }

    void Chat::SetTimestampsFormat(bool use_24h, bool show_timestamp_seconds) {
        Timestamp_24hFormat = use_24h;
        Timestamp_seconds = show_timestamp_seconds;
    }

    void Chat::SetTimestampsColor(Color color) {
        TimestampsColor = color;
    }

} // namespace GW
