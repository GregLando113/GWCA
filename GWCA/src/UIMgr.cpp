#include "..\Managers\UIMgr.h"

#include <cstdio>
#include <GWCA\Utilities\Scanner.h>
#include <GWCA\CtoSHeaders.h>
#include <GWCA\Managers\CtoSMgr.h>

namespace {
	typedef void __fastcall GwSendUIMessage_t(unsigned message, void* wParam, void* lParam);
	static GwSendUIMessage_t* g_uiSendMessage = nullptr;

	void UISendMessageInitialize() {
		auto uiMsg = GW::Scanner::Find(
			"\x8B\xDA\x1B\xF6\xF7\xDE\x4E\x83\xFF\x40\x73\x14\x68", 
			"xxxxxxxxxxxxx", -0xB);
		printf("[SCAN] UI::SendUIMessage = %p\n", (void *)uiMsg);
		g_uiSendMessage = (GwSendUIMessage_t*)uiMsg;
	}
}


void GW::UI::SendUIMessage(unsigned message, unsigned int wParam, int lParam)
{
	if (!g_uiSendMessage) {
		UISendMessageInitialize();
	}
	g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}

void GW::UI::SendUIMessage(unsigned message, void * wParam, void * lParam)
{
	if (!g_uiSendMessage) {
		UISendMessageInitialize();
	}
	g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}

GWCA_API void GW::UI::DrawOnCompass(unsigned sessionid, unsigned ptcount, CompassPoint pts[8])
{
	struct P037 {					// Used to send pings and drawings in the minimap. Related to StoC::P133
		const unsigned header = CtoGS_MSGDrawMap;
		unsigned sessionid = 0;		// unique for every player and shape. changes for every ping or shape.
		unsigned ptcount;			// number of points in the following array
		CompassPoint pts[8]; // in world coordinates divided by 100
		unsigned unk[8];
	};
	static P037 p = {};
	p.sessionid = sessionid;
	p.ptcount = ptcount;
	for (unsigned i = 0; i < ptcount; ++i)
		p.pts[i] = pts[i];
	CtoS::SendPacket(&p);
}
