#include <GWCA\Managers\GameThreadMgr.h>

#include <vector>
#include <map>

#include <GWCA\GWCA.h>
#include <GWCA\Managers\MemoryMgr.h>

namespace {
	CRITICAL_SECTION criticalsection;

	DWORD last_identifier = 0;
	bool render_state = false;

	typedef void(__fastcall *Render_t)(void*);
	uintptr_t* g__thingy;
	Render_t g__thingyret;

	std::vector<std::function<void(void)> > calls;
	std::map<DWORD, std::function<void(void)>> calls_permanent;

	void __stdcall CallFunctions() {
		if (TryEnterCriticalSection(&criticalsection)) {
			if (!calls.empty()) {
				for (const auto& Call : calls) {
					Call();
				}

				calls.clear();
			}

			if (!calls_permanent.empty()) {
				for (const auto& Call : calls_permanent) {
					Call.second();
				}
			}
			LeaveCriticalSection(&criticalsection);
		}
	}

	void __fastcall gameLoopHook(void* unk) {
		CallFunctions();
		g__thingyret(unk);
	}
}

void GW::GameThread::Initialize() {
	g__thingy = MemoryMgr::ReadPtrChain<uintptr_t*>((DWORD)MemoryMgr::BasePointerLocation, 3, 0, 0x3C, 0x4);
	g__thingyret = (Render_t)*g__thingy;
	*g__thingy = (uintptr_t)gameLoopHook;

	InitializeCriticalSection(&criticalsection);
}

void GW::GameThread::ClearCalls() {
	EnterCriticalSection(&criticalsection);
	calls.clear();
	calls_permanent.clear();
	LeaveCriticalSection(&criticalsection);
}

void GW::GameThread::RestoreHooks() {
	if (render_state) ToggleRenderHook();
	*g__thingy = (uintptr_t)g__thingyret;
}

void __declspec(naked) GW::GameThread::renderHook() {
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

void GW::GameThread::ToggleRenderHook() {
	static BYTE restorebuf[5];
	DWORD dwProt;

	render_state = !render_state;

	if (render_state) {
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

void GW::GameThread::Enqueue(std::function<void()> f) {
	EnterCriticalSection(&criticalsection);
	calls.emplace_back(f);
	LeaveCriticalSection(&criticalsection);
}

DWORD GW::GameThread::AddPermanentCall(std::function<void()> f) {
	EnterCriticalSection(&criticalsection);
	last_identifier++;
	calls_permanent[last_identifier] = f;
	LeaveCriticalSection(&criticalsection);

	return last_identifier;
}

void GW::GameThread::RemovePermanentCall(DWORD identifier) {
	EnterCriticalSection(&criticalsection);
	calls_permanent.erase(identifier);
	LeaveCriticalSection(&criticalsection);
}
