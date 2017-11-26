#pragma once
#include <string>
#include <functional>
#include <vector>
#include <map>

#include <GWCA\Utilities\Hooker.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
	namespace Chat {
		using Color = DWORD;
		using wchar = wchar_t;

		enum Channel {
			CHANNEL_ALLIANCE	= 0,
			CHANNEL_ALLIES		= 1, // coop with two groups for instance.
			CHANNEL_GWCA1		= 2,
			CHANNEL_ALL			= 3,
			CHANNEL_GWCA2		= 4,
			CHANNEL_MODERATOR	= 5,
			CHANNEL_EMOTE		= 6,
			CHANNEL_WARNING		= 7, // shows in the middle of the screen and does not parse <c> tags
			CHANNEL_GWCA3		= 8,
			CHANNEL_GUILD		= 9,
			CHANNEL_GLOBAL		= 10,
			CHANNEL_GROUP		= 11,
			CHANNEL_TRADE		= 12,
			CHANNEL_ADVISORY	= 13,
			CHANNEL_WHISPER		= 14,
			CHANNEL_COUNT,

			// non-standard channel, but usefull.
			CHANNEL_COMMAND,
			CHANNEL_UNKNOW = -1
		};

		struct ChatTemplate {
			DWORD unk0;
			DWORD type; // 0 = build, 1 = equipement
			GW::Array<wchar> code;
			wchar *name;
		};
		
		// void SetChatChannelColor(Channel channel, Color sender, Color message);
		// void RegisterEvent(Event e);

		bool IsTyping();

		// Send a message to an in-game channel (! for all, @ for guild, etc)
		void SendChat(char channel, const wchar *msg);
		void SendChat(char channel, const char  *msg);

		// Emulates a message in a given channel.
		void WriteChat(Channel channel, const wchar *message);
		void WriteChat(Channel channel, const char  *message);

		void WriteChat(Channel channel, const wchar *sender, const wchar *msg);
		void WriteChat(Channel channel, const char  *sender, const char  *msg);

		void WriteChatF(Channel channel, const wchar *sender, const char *fmt, ...);

		typedef std::function<void(int argc, LPWSTR *argv)> CmdCB;
		void CreateCommand(std::wstring cmd, CmdCB callback);
		void DeleteCommand(std::wstring cmd);

		extern bool ShowTimestamps;
		extern bool KeepChatHistory;
		extern bool Timestamp_24hFormat;
		extern Color TimestampsColor;

		void Initialize();
		void RestoreHooks();

		// SendChat callback can modify the msg before it is send.
		// Pay attention to not overflow the buffer.
		void SetSendChatCallback(std::function<
			void(Channel chan, wchar_t msg[140])>);

		void SetOpenLinks(bool b);

		Color SetSenderColor(Channel chan, Color col);
		Color SetMessageColor(Channel chan, Color col);

		void SetChatEventCallback(std::function<
			void(DWORD, DWORD, wchar_t*, void*)> callback);

		void SetLocalMessageCallback(std::function<
			bool(int, wchar_t*)> callback);

		void SetWhisperCallback(std::function<
			void(const wchar_t[20], const wchar_t[140])> callback);
	};
}
