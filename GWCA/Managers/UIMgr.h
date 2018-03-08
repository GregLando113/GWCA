#pragma once


namespace GW {
	namespace UI {

		enum UIMessage {
			kOpenWhisper = 0x10000000 | 0x8F
		};

		// SendMessage for Guild Wars UI messages, most UI interactions will use this.
		void SendUIMessage(unsigned message, unsigned int wParam = 0, int lParam = 0);
		void SendUIMessage(unsigned message, void* wParam = nullptr, void* lParam = nullptr);
	}
}