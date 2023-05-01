#pragma once

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct Module;
    extern Module ChatModule;

    namespace Chat {
        typedef uint32_t Color;
#pragma warning(push)
#pragma warning(disable: 4200)
        struct ChatMessage {
            uint32_t channel;
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

        enum Channel : int {
            CHANNEL_ALLIANCE = 0,
            CHANNEL_ALLIES = 1, // coop with two groups for instance.
            CHANNEL_GWCA1 = 2,
            CHANNEL_ALL = 3,
            CHANNEL_GWCA2 = 4,
            CHANNEL_MODERATOR = 5,
            CHANNEL_EMOTE = 6,
            CHANNEL_WARNING = 7, // shows in the middle of the screen and does not parse <c> tags
            CHANNEL_GWCA3 = 8,
            CHANNEL_GUILD = 9,
            CHANNEL_GLOBAL = 10,
            CHANNEL_GROUP = 11,
            CHANNEL_TRADE = 12,
            CHANNEL_ADVISORY = 13,
            CHANNEL_WHISPER = 14,
            CHANNEL_COUNT,

            // non-standard channel, but usefull.
            CHANNEL_COMMAND,
            CHANNEL_UNKNOW = -1
        };

        // void SetChatChannelColor(Channel channel, Color sender, Color message);
        // void RegisterEvent(Event e);

        GWCA_API Chat::ChatBuffer* GetChatLog();

        // Adds a message to chat log, bypassing chat window.
        GWCA_API void AddToChatLog(wchar_t* message, uint32_t channel);

        GWCA_API bool GetIsTyping();

        // Send a message to an in-game channel (! for all, @ for guild, etc)
        GWCA_API bool SendChat(char channel, const wchar_t* msg);
        GWCA_API bool SendChat(char channel, const char* msg);

        GWCA_API bool SendChat(const wchar_t* from, const wchar_t* msg);
        GWCA_API bool SendChat(const char* from, const char* msg);

        // Write unencoded non-transient message to chat with no sender using print format
        GWCA_API void WriteChatF(Channel channel, const wchar_t* format, ...);
        // Write to chat box, passing in unencoded message and optional unencoded sender. transient = true to bypass chat log.
        GWCA_API void WriteChat(Channel channel, const wchar_t* message, const wchar_t* sender = nullptr, bool transient = false);
        // Write to chat box, passing in encoded message and optional encoded sender. transient = true to bypass chat log.
        GWCA_API void WriteChatEnc(Channel channel, const wchar_t* message, const wchar_t* sender = nullptr, bool transient = false);


        typedef std::function<void(const wchar_t*, int, wchar_t**)> CmdCB;
        GWCA_API void CreateCommand(const wchar_t* cmd, const CmdCB& callback);
        GWCA_API CmdCB GetCommand(const wchar_t* cmd);
        GWCA_API void DeleteCommand(const wchar_t* cmd);

        GWCA_API void ToggleTimestamps(bool enable);
        GWCA_API void SetTimestampsFormat(bool use_24h, bool show_timestamp_seconds = false);
        GWCA_API void SetTimestampsColor(Color color);

        GWCA_API Color SetSenderColor(Channel chan, Color col);
        GWCA_API Color SetMessageColor(Channel chan, Color col);
        GWCA_API void  GetChannelColors(Channel chan, Color* sender, Color* message);
        GWCA_API void  GetDefaultColors(Channel chan, Color* sender, Color* message);

        GWCA_API Channel GetChannel(char opcode);
        GWCA_API Channel GetChannel(wchar_t opcode);
        // SendChat callback can modify the msg before it is send.
        // Pay attention to not overflow the buffer.
        typedef HookCallback<Channel, wchar_t*> SendChatCallback;
        GWCA_API void RegisterSendChatCallback(
            HookEntry* entry,
            const SendChatCallback& callback);

        GWCA_API void RemoveSendChatCallback(
            HookEntry* entry);

        typedef HookCallback<int, wchar_t*> LocalMessageCallback;
        GWCA_API void RegisterLocalMessageCallback(
            HookEntry* entry,
            const LocalMessageCallback& callback);

        GWCA_API void RemoveLocalMessageCallback(
            HookEntry* entry);

        typedef HookCallback<wchar_t*, wchar_t*> WhisperCallback;
        GWCA_API void RegisterWhisperCallback(
            HookEntry* entry,
            const WhisperCallback& callback);

        GWCA_API void RemoveWhisperCallback(
            HookEntry* entry);

        typedef HookCallback<Channel, wchar_t**, FILETIME, int> PrintChatCallback;
        GWCA_API void RegisterPrintChatCallback(
            HookEntry* entry,
            const PrintChatCallback& callback);

        typedef HookCallback<wchar_t*> StartWhisperCallback;
        GWCA_API void RegisterStartWhisperCallback(
            HookEntry* entry,
            const StartWhisperCallback& callback);

        GWCA_API void RemoveStartWhisperCallback(
            HookEntry* entry);

        typedef HookCallback<wchar_t*, uint32_t, ChatMessage*> ChatLogCallback;
        GWCA_API void RegisterChatLogCallback(
            HookEntry* entry,
            const ChatLogCallback& callback,
            int altitude = -0x8000);
        GWCA_API void RemoveChatLogCallback(HookEntry* entry);
    };
}
