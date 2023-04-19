#include "stdafx.h"

#include <MinHook.h>
#include <GWCA/Utilities/Hooker.h>

static std::atomic<int> init_count;
static std::atomic<int> in_hook_count;

void GW::HookBase::Initialize()
{
    ++init_count;
    MH_Initialize();
}

void GW::HookBase::Deinitialize()
{
    if (--init_count == 0)
        MH_Uninitialize();
}

void GW::HookBase::EnterHook()
{
    ++in_hook_count;
}

void GW::HookBase::LeaveHook()
{
    --in_hook_count;
}

int GW::HookBase::GetInHookCount()
{
    return in_hook_count;
}

void GW::HookBase::EnableHooks(void *target)
{
    if (target) {
        MH_EnableHook(target);
    } else {
        MH_EnableHook(MH_ALL_HOOKS);
    }
}

void GW::HookBase::DisableHooks(void *target)
{
    if (target) {
        MH_DisableHook(target);
    } else {
        MH_DisableHook(MH_ALL_HOOKS);
    }
}

int GW::HookBase::CreateHook(void *target, void *detour, void **trampoline)
{
    return target ? MH_CreateHook(target, detour, trampoline) : -1;
}

void GW::HookBase::RemoveHook(void *target)
{
    if(target)
        MH_RemoveHook(target);
}
