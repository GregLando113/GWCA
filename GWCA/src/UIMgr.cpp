#include <GWCA/Managers/UIMgr.h>

#include <stdio.h>

#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Managers/GameThreadMgr.h>

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Managers/CtoSMgr.h>

namespace {
	typedef void __fastcall GwSendUIMessage_t(unsigned message, void* wParam, void* lParam);
	GwSendUIMessage_t* g_uiSendMessage = nullptr;

	typedef void __fastcall GwLoadSettings_t(uint32_t size, uint8_t *data);
	GwLoadSettings_t *GwLoadSettings = nullptr;

	GW::UI::ArrayByte *GwSettings = nullptr;

	DWORD *gw_ui_drawn = nullptr;
	DWORD *gw_shift_screen = nullptr;

	void Initialize() {
		g_uiSendMessage = (GwSendUIMessage_t *)GW::Scanner::Find(
			"\x8B\xDA\x1B\xF6\xF7\xDE\x4E\x83\xFF\x40\x73\x14\x68", 
			"xxxxxxxxxxxxx", -0xB);
		printf("[SCAN] UI::SendUIMessage = %p\n", g_uiSendMessage);

		GwLoadSettings = (GwLoadSettings_t *)GW::Scanner::Find("\x33\xC9\xE8\x00\x00\x00\x00\x5F\x5E\xE9", "xxx????xxx", -37);
		printf("[SCAN] UI::LoadSettings = %p\n", GwLoadSettings);

		{
			uintptr_t temp = GW::Scanner::Find("\x57\x77\x04\x8B\xF8\xEB\x18", "xxxxxxx", +15);
			printf("[SCAN] UI::GwSettings = %p\n", (void *)temp);
			GwSettings = *(GW::UI::ArrayByte **)temp;
		}

		{
			uintptr_t temp = GW::Scanner::Find("\x85\xC0\x74\x58\x5F\x5E\xE9", "xxxxxxx", +16);
			printf("[SCAN] UI::gw_ui_drawn = %p\n", (void *)temp);
			gw_ui_drawn = *(DWORD **)temp;
		}

		{
			uintptr_t temp = GW::Scanner::Find("\x85\xC0\x0F\x85\x00\x00\x00\x00\x85\xC9\x75\x00", "xxxx????xxx?", +14);
			printf("[SCAN] UI::gw_shift_screen = %p\n", (void *)temp);
			gw_shift_screen = *(DWORD **)temp;
		}
	}
}


void GW::UI::SendUIMessage(unsigned message, unsigned int wParam, int lParam)
{
	if (!g_uiSendMessage) {
		::Initialize();
	}
	g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}

void GW::UI::SendUIMessage(unsigned message, void * wParam, void * lParam)
{
	if (!g_uiSendMessage) {
		::Initialize();
	}
	g_uiSendMessage(message, (void*)wParam, (void*)lParam);
}

void GW::UI::DrawOnCompass(unsigned sessionid, unsigned ptcount, CompassPoint pts[8])
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

void GW::UI::LoadSettings(size_t size, uint8_t *data) {
	if (!GwLoadSettings) {
		::Initialize();
	}
	GwLoadSettings(size, data);
}

GW::UI::ArrayByte GW::UI::GetSettings() {
	if (!GwSettings) {
		::Initialize();
	}
	return *GwSettings;
}

bool GW::UI::GetIsUIDrawn() {
	if (!gw_ui_drawn) {
		::Initialize();
	}
	return (*gw_ui_drawn == 0);
}

bool GW::UI::GetIsShiftScrennShot() {
	if (!gw_shift_screen) {
		::Initialize();
	}
	return (*gw_shift_screen != 0);
}