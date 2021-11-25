#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace {
    using namespace GW;

    CRITICAL_SECTION mutex;

    uint32_t last_identifier = 0;
    bool render_state = false;

    typedef void(__cdecl *Render_t)(void*);
    uintptr_t *g__thingy;
    Render_t g__thingyret;

    bool in_gamethread = false;

    std::vector<std::function<void(void)>> singleshot_callbacks;
    std::unordered_map<HookEntry *, GameThread::GameThreadCallback> GameThread_callbacks;

    void CallFunctions()
    {
        EnterCriticalSection(&mutex);
        in_gamethread = true;
        if (!singleshot_callbacks.empty()) {
            for (const auto& Call : singleshot_callbacks) {
                Call();
            }

            singleshot_callbacks.clear();
        }

        HookStatus status;
        for (auto& it : GameThread_callbacks) {
            it.second(&status);
            ++status.altitude;
        }
        in_gamethread = false;
        LeaveCriticalSection(&mutex);
    }

    void __cdecl gameLoopHook(void* unk)
    {
        CallFunctions();
        g__thingyret(unk);
    }

    void Init()
    {
        InitializeCriticalSection(&mutex);

        uintptr_t address = Scanner::Find(
            "\x2B\xCE\x8B\x15\x00\x00\x00\x00\xF7\xD9\x1B\xC9", "xxxx????xxxx", +4);
        GWCA_INFO("[SCAN] BasePointerLocation = %p\n", (void *)address);

        if (Verify(address)) {
            address = *(uintptr_t *)(address);
            address = *(uintptr_t *)(address + 0);
            address = *(uintptr_t *)(address + 0x3C);

            g__thingy = (uintptr_t *)(address + 4);
            g__thingyret = (Render_t)*g__thingy;
        }
    }

    void Exit()
    {
        GameThread::ClearCalls();
        DeleteCriticalSection(&mutex);
    }

    void EnableHooks()
    {
        *g__thingy = (uintptr_t)gameLoopHook;
    }

    void DisableHooks()
    {
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

    void GameThread::ClearCalls()
    {
        EnterCriticalSection(&mutex);
        singleshot_callbacks.clear();
        GameThread_callbacks.clear();
        LeaveCriticalSection(&mutex);
    }

    void GameThread::Enqueue(std::function<void()> f)
    {
        EnterCriticalSection(&mutex);
        if (in_gamethread) {
            f();
        }
        else {
            singleshot_callbacks.emplace_back(f);
        }
        LeaveCriticalSection(&mutex);
    }

    void GameThread::RegisterGameThreadCallback(
        HookEntry *entry,
        GameThreadCallback callback)
    {
        GameThread_callbacks.insert({entry, callback});
    }

    void GameThread::RemoveGameThreadCallback(
        HookEntry *entry)
    {
        auto it = GameThread_callbacks.find(entry);
        if (it != GameThread_callbacks.end())
            GameThread_callbacks.erase(it);
    }
} // namespace GW
