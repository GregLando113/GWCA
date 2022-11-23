#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <map>
#include <string>

#include <GWCA/GWCA.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/Constants/Constants.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/RenderMgr.h>

// We can forward declare, because we won't use it
struct IDirect3DDevice9;

static volatile bool running;

std::map<GW::AgentID, std::pair<float, float> > hiddenAgents;
GW::Constants::Allegiance hideAllegiance = (GW::Constants::Allegiance) 0;

inline void HideAgent(GW::Agent* agt)
{
    if (!agt) return;
    if (agt->width2 > 0 && agt->height2 > 0)
    {
        hiddenAgents[agt->agent_id] = std::pair<float, float>(agt->width2, agt->height2);
        agt->width2 = agt->height2 = 0;
    }
}

void HideAgents()
{
    auto agents = GW::Agents::GetAgentArray();
    if (!agents) return;
    for (auto agt : *agents)
    {
        if (!agt) continue;
        // hide any previously hidden agents that may have been out of range
        if (agt->width2 > 0 || agt->height2 > 0)
        {
            auto it = hiddenAgents.find(agt->agent_id);
            if (it != hiddenAgents.end())
            {
                HideAgent(agt);
                continue;
            }
        }
        // hide any agents that match allegiance criteria
        auto agtLiving = agt->GetAsAgentLiving();
        if (!agtLiving) continue;
        if (agtLiving->allegiance != hideAllegiance) continue;
        HideAgent(agt);
    }
}

inline void UnhideAgent(GW::Agent* agt)
{
    if (!agt) return;
    if (agt->width2 > 0 && agt->height2 > 0) return;
    auto it = hiddenAgents.find(agt->agent_id);
    if (it == hiddenAgents.end()) return;
    auto& pair = it->second;
    agt->width2 = pair.first;
    agt->height2 = pair.second;
    hiddenAgents.erase(it);
}

void UnhideAgents()
{
    auto agents = GW::Agents::GetAgentArray();
    if (!agents) return;
    for (auto agt : *agents)
    {
        if (!agt) continue;
        UnhideAgent(agt);
    }
}

static void CmdHide(const wchar_t* msg, int argc, wchar_t** argv)
{
    if (argc < 2)
    {
        GW::Agent* target = GW::Agents::GetTarget();
        if (!target)
        {
            GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Please select a target first");
            return;
        }

        HideAgent(target);
        return;
    }

    std::wstring ws(argv[1]);
    if (ws == L"enemies")
    {
        hideAllegiance = (GW::Constants::Allegiance::Enemy);
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"hiding enemies...");
    }
    else if (ws == L"none")
    {
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Unhiding all agents...");
        hideAllegiance = (GW::Constants::Allegiance) 0;
        UnhideAgents();
    }
    else
    {
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"Usage: \"/hide\" or \"/hide enemies\" or \"/hide none\"");
    }
}

static void GameLoop(IDirect3DDevice9* device)
{
    // This is call from within the game thread and all operation should be done here.
    // You can't freeze this thread, so no blocking operation or at your own risk.
    static bool initialized = false;
    if (!initialized) 
    {
        GW::Chat::CreateCommand(L"hide", CmdHide);
        initialized = true;
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"HideAgent: Initialized. Press the END key to quit.");
    }
    if (GetAsyncKeyState(VK_END) & 1) 
    {
        GW::Chat::WriteChat(GW::Chat::CHANNEL_MODERATOR, L"HideAgent: Bye!");
        GW::DisableHooks();
        running = false;
    }

    if (GW::Map::GetInstanceType() == GW::Constants::InstanceType::Loading)
    {
        hiddenAgents.clear();
    }
    else
    {
        if (running)
            HideAgents();
        else
            UnhideAgents();
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
