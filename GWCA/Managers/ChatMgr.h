#pragma once
#include <string>
#include <functional>
#include <vector>
#include <map>

#include <GWCA\Utilities\Hooker.h>

namespace GW {

	namespace Chat {

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

		typedef DWORD Color;

		struct ChatTemplate {
			DWORD unk1[2];
			wchar_t* template_code; // Is actually part of a Array<wchar_t>
			DWORD templatecode_sizealloc; // Is actually part of a Array<wchar_t>
			DWORD templatecode_size; // Is actually part of a Array<wchar_t>
			DWORD unk_identifier; // Is actually part of a Array<wchar_t>
			wchar_t* template_name;
		};

		typedef std::function<bool(std::wstring& command, std::wstring& args)> Callback;

		void Initialize();
		void RestoreHooks();

		// Send a message to an in-game channel (! for all, @ for guild, etc)
		void SendChat(const wchar_t* msg, wchar_t channel);
		void SendChat(const char* msg, char channel);

		// Write to chat as a PM with printf style arguments.
		void WriteChatF(const wchar_t* from, const wchar_t* format, ...);

		// Simple write to chat as a PM
		void WriteChat(const wchar_t* from, const wchar_t* msg);

		// Emulates a message in a given channel.
		void WriteChat(Channel channel, const wchar_t *message);
		void WriteChat(Channel channel, const char* message);

		void RegisterCommand(const std::wstring& command, Callback callback);
		void DeleteCommand(const std::wstring& command);

		std::vector<std::wstring> SplitString(const std::wstring& str, wchar_t c = ' ');

		// SendChat callback can modify the msg before it is send.
		// Pay attention to not overflow the buffer.
		void SetSendChatCallback(std::function<
			void(Channel chan, wchar_t msg[139])>);

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
