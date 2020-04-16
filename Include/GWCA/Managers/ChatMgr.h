#pragma once


#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/GameContainers/Array.h>

#include <cstdint>
#include <functional>
#include <string>



namespace GW {
    struct Module;
    extern Module ChatModule;

    namespace Chat {
        typedef uint32_t Color;

        enum Channel {
            CHANNEL_ALLIANCE    = 0,
            CHANNEL_ALLIES      = 1, // coop with two groups for instance.
            CHANNEL_GWCA1       = 2,
            CHANNEL_ALL         = 3,
            CHANNEL_GWCA2       = 4,
            CHANNEL_MODERATOR   = 5,
            CHANNEL_EMOTE       = 6,
            CHANNEL_WARNING     = 7, // shows in the middle of the screen and does not parse <c> tags
            CHANNEL_GWCA3       = 8,
            CHANNEL_GUILD       = 9,
            CHANNEL_GLOBAL      = 10,
            CHANNEL_GROUP       = 11,
            CHANNEL_TRADE       = 12,
            CHANNEL_ADVISORY    = 13,
            CHANNEL_WHISPER     = 14,
            CHANNEL_COUNT,

            // non-standard channel, but usefull.
            CHANNEL_COMMAND,
            CHANNEL_UNKNOW = -1
        };
        
        // void SetChatChannelColor(Channel channel, Color sender, Color message);
        // void RegisterEvent(Event e);

        GWCA_API bool GetIsTyping();

        // Send a message to an in-game channel (! for all, @ for guild, etc)
        GWCA_API void SendChat(char channel, const wchar_t *msg);
        GWCA_API void SendChat(char channel, const char    *msg);

        GWCA_API void SendChat(const wchar_t *from, const wchar_t *msg);
        GWCA_API void SendChat(const char    *from, const char    *msg);

        // Emulates a message in a given channel.
        GWCA_API void WriteChat(Channel channel, const wchar_t *message);
        GWCA_API void WriteChat(Channel channel, const char    *message);

        GWCA_API void WriteChat(Channel channel, const wchar_t *sender, const wchar_t *msg);
        GWCA_API void WriteChat(Channel channel, const char    *sender, const char    *msg);

        GWCA_API void WriteChatF(Channel channel, const wchar_t *sender, const char *fmt, ...);

        typedef std::function<void (const wchar_t *, int, wchar_t **)> CmdCB;
        GWCA_API void CreateCommand(std::wstring cmd, CmdCB callback);
        GWCA_API void DeleteCommand(std::wstring cmd);

        GWCA_API void ToggleTimestamps(bool enable);
        GWCA_API void SetTimestampsFormat(bool use_24h, bool show_timestamp_seconds = false);
        GWCA_API void SetTimestampsColor(Color color);

        GWCA_API Color SetSenderColor(Channel chan, Color col);
        GWCA_API Color SetMessageColor(Channel chan, Color col);
        GWCA_API void  GetChannelColors(Channel chan, Color *sender, Color *message);
        GWCA_API void  GetDefaultColors(Channel chan, Color *sender, Color *message);

        // SendChat callback can modify the msg before it is send.
        // Pay attention to not overflow the buffer.
        typedef HookCallback<Channel, wchar_t *> SendChatCallback;
        GWCA_API void RegisterSendChatCallback(
            HookEntry *entry,
            SendChatCallback callback);

        GWCA_API void RemoveSendChatCallback(
            HookEntry *entry);

        typedef HookCallback<uint32_t, uint32_t, wchar_t *, void *> ChatEventCallback;
        GWCA_API void RegisterChatEventCallback(
            HookEntry *entry,
            ChatEventCallback callback);

        GWCA_API void RemoveChatEventCallback(
            HookEntry *entry);

        typedef HookCallback<int, wchar_t *> LocalMessageCallback;
        GWCA_API void RegisterLocalMessageCallback(
            HookEntry *entry,
            LocalMessageCallback callback);

        GWCA_API void RemoveLocalMessageCallback(
            HookEntry *entry);

        typedef HookCallback<wchar_t *, wchar_t *> WhisperCallback;
        GWCA_API void RegisterWhisperCallback(
            HookEntry *entry,
            WhisperCallback callback);

        GWCA_API void RemoveRegisterWhisperCallback(
            HookEntry *entry);

        typedef HookCallback<wchar_t*> StartWhisperCallback;
        GWCA_API void RegisterStartWhisperCallback(
            HookEntry* entry,
            StartWhisperCallback callback);

        GWCA_API void RemoveStartWhisperCallback(
            HookEntry *entry);
    };
}
