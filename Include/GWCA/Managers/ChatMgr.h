#pragma once
#include <GWCA/Constants/Chat.h>
#include <string>
#include <functional>


#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {
    struct Module;
    extern Module ChatModule;

    namespace Chat {
#pragma warning(push)
#pragma warning(disable: 4200)
        struct ChatMessage {
            Chat::Channel channel;
            uint32_t unk1;
            FILETIME timestamp;
            wchar_t message[0];
        };
#pragma warning(pop)

        const size_t CHAT_LOG_LENGTH = 0x200;
        struct ChatBuffer {
            uint32_t next;
            uint32_t unk1;
            uint32_t unk2;
            ChatMessage* messages[CHAT_LOG_LENGTH];
        };

        // void SetChatChannelColor(Channel channel, Color sender, Color message);
        // void RegisterEvent(Event e);

        GWCA_API Chat::Channel GetChannel(wchar_t opcode);

        GWCA_API ChatBuffer* GetChatLog();

        // Adds a message to chat log, bypassing chat window.
        GWCA_API void AddToChatLog(wchar_t* message, uint32_t channel);

        GWCA_API bool GetIsTyping();

        // Send a message to an in-game channel (! for all, @ for guild, etc)
        GWCA_API void SendChat(char channel, const wchar_t* msg);
        GWCA_API void SendChat(char channel, const char* msg);

        GWCA_API void SendChat(const wchar_t* from, const wchar_t* msg);
        GWCA_API void SendChat(const char* from, const char* msg);

        // Write to chat box, passing in unencoded message and optional unencoded sender. transient = true to bypass chat log.
        GWCA_API void WriteChat(Chat::Channel channel, const wchar_t* message, const wchar_t* sender = nullptr, bool transient = false);
        // Write to chat box, passing in encoded message and optional encoded sender. transient = true to bypass chat log.
        GWCA_API void WriteChatEnc(Chat::Channel channel, const wchar_t* message, const wchar_t* sender = nullptr, bool transient = false);


        typedef std::function<void(const wchar_t*, int, wchar_t**)> CmdCB;
        GWCA_API void CreateCommand(std::wstring cmd, CmdCB callback);
        GWCA_API void DeleteCommand(std::wstring cmd);

        GWCA_API void ToggleTimestamps(bool enable);
        GWCA_API void SetTimestampsFormat(bool use_24h, bool show_timestamp_seconds = false);
        GWCA_API void SetTimestampsColor(Color color);

        GWCA_API Color SetSenderColor(Chat::Channel chan, Color col);
        GWCA_API Color SetMessageColor(Chat::Channel chan, Color col);
        GWCA_API void  GetChannelColors(Chat::Channel chan, Color* sender, Color* message);
        GWCA_API void  GetDefaultColors(Chat::Channel chan, Color* sender, Color* message);

        // SendChat callback can modify the msg before it is send.
        // Pay attention to not overflow the buffer.
        typedef HookCallback<Chat::Channel, wchar_t*> SendChatCallback;
        GWCA_API void RegisterSendChatCallback(
            HookEntry* entry,
            SendChatCallback callback);

        GWCA_API void RemoveSendChatCallback(
            HookEntry* entry);

        typedef HookCallback<uint32_t, uint32_t, wchar_t*, void*> ChatEventCallback;
        GWCA_API void RegisterChatEventCallback(
            HookEntry* entry,
            ChatEventCallback callback);

        GWCA_API void RemoveChatEventCallback(
            HookEntry* entry);

        typedef HookCallback<Chat::Channel, wchar_t*> LocalMessageCallback;
        GWCA_API void RegisterLocalMessageCallback(
            HookEntry* entry,
            LocalMessageCallback callback);

        GWCA_API void RemoveLocalMessageCallback(
            HookEntry* entry);

        typedef HookCallback<wchar_t*, wchar_t*> WhisperCallback;
        GWCA_API void RegisterWhisperCallback(
            HookEntry* entry,
            WhisperCallback callback);

        GWCA_API void RemoveRegisterWhisperCallback(
            HookEntry* entry);

        typedef HookCallback<Chat::Channel, wchar_t**, FILETIME, int> PrintChatCallback;
        GWCA_API void RegisterPrintChatCallback(
            HookEntry* entry,
            PrintChatCallback callback);

        typedef HookCallback<wchar_t*> StartWhisperCallback;
        GWCA_API void RegisterStartWhisperCallback(
            HookEntry* entry,
            StartWhisperCallback callback);

        GWCA_API void RemoveStartWhisperCallback(
            HookEntry* entry);

        typedef HookCallback<wchar_t*, Chat::Channel, ChatMessage*> ChatLogCallback;
        GWCA_API void RegisterChatLogCallback(
            HookEntry* entry,
            ChatLogCallback callback,
            int altitude = -0x8000);
        GWCA_API void RemoveChatLogCallback(HookEntry* entry);
    };
}
