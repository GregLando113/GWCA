#pragma once


namespace GW {
	namespace UI {

		enum UIMessage : unsigned int {
			kShowAgentNameTag       = 0x10000000 | 0x19,
			kHideAgentNameTag       = 0x10000000 | 0x1A,
			kSetAgentNameTagAttribs = 0x10000000 | 0x1B,
			kWriteToChatLog         = 0x10000000 | 0x7E,
			kOpenWhisper            = 0x10000000 | 0x8F // wparam = wchar* name
		};

		// SendMessage for Guild Wars UI messages, most UI interactions will use this.
		void SendUIMessage(unsigned message, unsigned int wParam = 0, int lParam = 0);
		void SendUIMessage(unsigned message, void* wParam = nullptr, void* lParam = nullptr);
	}
}