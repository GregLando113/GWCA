#include <GWCA\Managers\GameThreadMgr.h>

#include <GWCA\GWCA.h>
#include <GWCA\Managers\MemoryMgr.h>

DWORD GW::GameThreadMgr::last_identifier_ = 0;
CRITICAL_SECTION GW::GameThreadMgr::criticalsection_;

typedef void(__fastcall *Render_t)(void*);
uintptr_t* g__thingy;
Render_t g__thingyret;

void __stdcall GW::GameThreadMgr::CallFunctions() {
	if (TryEnterCriticalSection(&criticalsection_)) {
		if (!calls_.empty()) {
			for (const auto& Call : calls_) {
				Call();
			}

			calls_.clear();
		}

		if (!calls_permanent_.empty()) {
			for (const auto& Call : calls_permanent_) {
				Call.second();
			}
		}
		LeaveCriticalSection(&criticalsection_);
	}
}

void __fastcall GW::GameThreadMgr::gameLoopHook(void* unk) {
	GameThreadMgr::Instance().CallFunctions();
	g__thingyret(unk);
}

void __declspec(naked) GW::GameThreadMgr::renderHook() {
	Sleep(1);
	_asm {
		POP ESI
		POP EBX
		FSTP DWORD PTR DS : [0xA3F998]
		MOV ESP, EBP
		POP EBP
		RETN
	}
}

void GW::GameThreadMgr::ToggleRenderHook() {
	static BYTE restorebuf[5];
	DWORD dwProt;

	render_state_ = !render_state_;

	if (render_state_) {
		memcpy(restorebuf, MemoryMgr::RenderLoopLocation, 5);

		VirtualProtect(MemoryMgr::RenderLoopLocation, 5, PAGE_EXECUTE_READWRITE, &dwProt);
		memset(MemoryMgr::RenderLoopLocation, 0xE9, 1);
		*(DWORD*)(MemoryMgr::RenderLoopLocation + 1) = (DWORD)((BYTE*)renderHook - MemoryMgr::RenderLoopLocation) - 5;
		VirtualProtect(MemoryMgr::RenderLoopLocation, 5, dwProt, &dwProt);
	} else {
		VirtualProtect(MemoryMgr::RenderLoopLocation, 5, PAGE_EXECUTE_READWRITE, &dwProt);
		memcpy(MemoryMgr::RenderLoopLocation, restorebuf, 5);
		VirtualProtect(MemoryMgr::RenderLoopLocation, 5, dwProt, &dwProt);
	}
}


GW::GameThreadMgr::GameThreadMgr() : render_state_(false) {
	g__thingy = MemoryMgr::ReadPtrChain<uintptr_t*>((DWORD)MemoryMgr::BasePointerLocation, 3, 0, 0x3C, 0x4);
	g__thingyret = (Render_t)*g__thingy;
	*g__thingy = (uintptr_t)GW::GameThreadMgr::gameLoopHook;


	InitializeCriticalSection(&criticalsection_);
}

void GW::GameThreadMgr::RestoreHooks() {
	if (render_state_) ToggleRenderHook();
	*g__thingy = (uintptr_t)g__thingyret;
}
