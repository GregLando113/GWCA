#include "..\Managers\UIMgr.h"

#include <cstdio>
#include <GWCA\Utilities\Scanner.h>
namespace {
	typedef void __fastcall GwSendUIMessage_t(unsigned message, void* wParam, void* lParam);
	static GwSendUIMessage_t* g_uiSendMessage = nullptr;

	void UISendMessageInitialize() {
		auto uiMsg = GW::Scanner::Find(
			"\x8B\xDA\x1B\xF6\xF7\xDE\x4E\x83\xFF\x40\x73\x14\x68", 
			"xxxxxxxxxxxxx", -0xB);
		printf("[SCAN] UI::SendUIMessage = %p\n", uiMsg);
		g_uiSendMessage = (GwSendUIMessage_t*)uiMsg;
	}
}


void GW::UI::SendUIMessage(UIMessage message, unsigned int wParam, int lParam)
{
	if (!g_uiSendMessage) {
		UISendMessageInitialize();
	}
	g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}

void GW::UI::SendUIMessage(UIMessage message, void * wParam, void * lParam)
{
	if (!g_uiSendMessage) {
		UISendMessageInitialize();
	}
	g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}
