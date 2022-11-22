#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

#include <GWCA/GWCA.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/Constants/Constants.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/RenderMgr.h>

// We can forward declare, because we won't use it
struct IDirect3DDevice9;

static volatile bool running;

static void CmdHideAgent(const wchar_t *msg, int argc, wchar_t **argv)
{
    GW::Agent * target = GW::Agents::GetTarget();
    if (!target)
    {
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Please select a target first");
        return;
    }

    target->width2 = target->height2 = 0;

}

static void GameLoop(IDirect3DDevice9* device)
{
    // This is call from within the game thread and all operation should be done here.
    // You can't freeze this thread, so no blocking operation or at your own risk.
    static bool initialized = false;
    if (!initialized) {
        GW::Chat::CreateCommand(L"hide", CmdHideAgent);
        initialized = true;

        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"HideAgent: Initialized.. press the END key to quit.");
    }

    if (GetAsyncKeyState(VK_END) & 1) {
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"HideAgent: Bye!");
        GW::DisableHooks();
        running = false;
    }
}

static DWORD WINAPI ThreadProc(LPVOID lpModule)
{
    // This is a new thread so you should only initialize GWCA and setup the hook on the game thread.
    // When the game thread hook is setup (i.e. SetRenderCallback), you should do the next operations
    // on the game from within the game thread.

    HMODULE hModule = static_cast<HMODULE>(lpModule);

    GW::Initialize();

    GW::Render::SetRenderCallback(GameLoop);

    running = true;
    while (running) {
        Sleep(100);
    }

    // Hooks are disable from Guild Wars thread (safely), so we just make sure we exit the last hooks
    while (GW::HookBase::GetInHookCount())
        Sleep(16);

    // We can't guarantee that the code in Guild Wars thread isn't still in the trampoline, but
    // practically a short sleep is fine.
    Sleep(16);
    GW::Terminate();

    FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);

    if (dwReason == DLL_PROCESS_ATTACH) {
        HANDLE handle = CreateThread(0, 0, ThreadProc, hModule, 0, 0);
        if (handle)
            CloseHandle(handle);
    }

    return TRUE;
}
