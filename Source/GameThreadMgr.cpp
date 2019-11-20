#include "stdafx.h"

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace {
    using namespace GW;

    CRITICAL_SECTION criticalsection;

    uint32_t last_identifier = 0;
    bool render_state = false;
	bool in_game_thread = false;

    typedef void(__fastcall *Render_t)(void*);
    uintptr_t *g__thingy;
    Render_t g__thingyret;

    std::vector<std::function<void(void)> > calls;
    std::map<uint32_t, std::function<void(void)>> calls_permanent;

    void __stdcall CallFunctions() {
        if (TryEnterCriticalSection(&criticalsection)) {
			in_game_thread = true;
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
			in_game_thread = false;
            LeaveCriticalSection(&criticalsection);
        }
    }

    void __fastcall gameLoopHook(void* unk) {
        CallFunctions();
        g__thingyret(unk);
    }

    void Init() {
        InitializeCriticalSection(&criticalsection);

        uintptr_t Pointer = MemoryMgr::BasePointerLocation;
        Pointer = *(uintptr_t *)(Pointer);
        Pointer = *(uintptr_t *)(Pointer + 0);
        Pointer = *(uintptr_t *)(Pointer +  + 0x3C);

        g__thingy = (uintptr_t *)(Pointer + 4);
        g__thingyret = (Render_t)*g__thingy;
    }

    void Exit() {
        GameThread::ClearCalls();
        DeleteCriticalSection(&criticalsection);
    }

    void EnableHooks() {
        *g__thingy = (uintptr_t)gameLoopHook;
    }

    void DisableHooks() {
        *g__thingy = (uintptr_t)g__thingyret;
    }
}

namespace GW {
    Module GameThreadModule = {
        "GameThreadModule", // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,             // exit_module
        ::EnableHooks,      // enable_hooks
        ::DisableHooks,     // disable_hooks
    };

    void GameThread::ClearCalls() {
        EnterCriticalSection(&criticalsection);
        calls.clear();
        calls_permanent.clear();
        LeaveCriticalSection(&criticalsection);
    }

    static void __declspec(naked) renderHook() {
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
	void GameThread::Enqueue(std::function<void()> f) {
		if (in_game_thread) {
			f();
		}
		else {
			EnterCriticalSection(&criticalsection);
			calls.emplace_back(f);
			LeaveCriticalSection(&criticalsection);
		}
    }

    uint32_t GameThread::AddPermanentCall(std::function<void()> f) {
        EnterCriticalSection(&criticalsection);
        last_identifier++;
        calls_permanent[last_identifier] = f;
        LeaveCriticalSection(&criticalsection);

        return last_identifier;
    }

    void GameThread::RemovePermanentCall(uint32_t identifier) {
        EnterCriticalSection(&criticalsection);
        calls_permanent.erase(identifier);
        LeaveCriticalSection(&criticalsection);
    }
} // namespace GW