#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

#include <GWCA/GWCA.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/Constants/Constants.h>

#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/RenderMgr.h>

// We can forward declare, because we won't use it
struct IDirect3DDevice9;

static volatile bool running;

static void CmdTeleport(const wchar_t *msg, int argc, wchar_t **argv)
{
    if (argc != 2) {
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Teleport: /tp <outpost id>");
        return;
    }

    const int MAX_MAP_ID = static_cast<int>(GW::Constants::MapID::Ashford_Catacombs_1070_AE);

    int outpost_id = wcstol(argv[1], nullptr, 0);
    if (outpost_id < 0 || MAX_MAP_ID < outpost_id) {
        wchar_t buffer[512];
        wsprintfW(buffer, L"Teleport: The map id must be between 0 and %d", MAX_MAP_ID);

        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, buffer);
        return;
    }

    GW::Map::Travel(static_cast<GW::Constants::MapID>(outpost_id));
}

static void GameLoop(IDirect3DDevice9* device)
{
    // This is call from within the game thread and all operation should be done here.
    // You can't freeze this thread, so no blocking operation or at your own risk.
    static bool initialized = false;
    if (!initialized) {
        GW::Chat::CreateCommand(L"tp", CmdTeleport);
        initialized = true;

        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Teleport: Initialized");
    }

    if (GetAsyncKeyState(VK_END) & 1) {
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Teleport: Bye!");
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
        CloseHandle(handle);
    }

    return TRUE;
}