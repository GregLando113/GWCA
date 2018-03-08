#pragma once


namespace GW {
	namespace UI {

		enum UIMessage : unsigned int {
			kOpenWhisper = 0x10000000 | 0x8F
		};

		// SendMessage for Guild Wars UI messages, most UI interactions will use this.
		void SendUIMessage(UIMessage message, unsigned int wParam = 0, int lParam = 0);
		void SendUIMessage(UIMessage message, void* wParam = nullptr, void* lParam = nullptr);
	}
}