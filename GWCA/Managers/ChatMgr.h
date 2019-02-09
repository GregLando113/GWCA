#pragma once

namespace GW {
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

        struct ChatTemplate {
            uint32_t        unk0;
            uint32_t        type; // 0 = build, 1 = equipement
            Array<wchar_t>  code;
            wchar_t        *name;
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

        typedef std::function<void(const wchar_t *msg, int argc, wchar_t **argv)> CmdCB;
        GWCA_API void CreateCommand(std::wstring cmd, CmdCB callback);
        GWCA_API void DeleteCommand(std::wstring cmd);

        // @Cleanup:
        // move those out
        extern bool ShowTimestamps;
        // extern bool KeepChatHistory; @Deprecated
        extern bool Timestamp_24hFormat;
        extern Color TimestampsColor;

        GWCA_API void Initialize();
        GWCA_API void RestoreHooks();

        // SendChat callback can modify the msg before it is send.
        // Pay attention to not overflow the buffer.
        GWCA_API void SetSendChatCallback(
            std::function<void (Channel chan, wchar_t *msg)>);

        GWCA_API void SetOpenLinks(bool b);

        GWCA_API Color SetSenderColor(Channel chan, Color col);
        GWCA_API Color SetMessageColor(Channel chan, Color col);
        GWCA_API void  GetChannelColors(Channel chan, Color *sender, Color *message);
        GWCA_API void  GetDefaultColors(Channel chan, Color *sender, Color *message);

        GWCA_API void SetChatEventCallback(std::function<
            void (uint32_t, uint32_t, wchar_t *, void *)> callback);

        GWCA_API void SetLocalMessageCallback(std::function<
            bool (int, wchar_t *)> callback);

        GWCA_API void SetWhisperCallback(std::function<
            void(wchar_t *, wchar_t *)> callback);
    };
}
