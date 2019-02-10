#include "stdafx.h"

#include <MinHook.h>
#include <GWCA/Utilities/Hooker.h>

void GW::HookBase::Initialize()
{
    MH_Initialize();
}

void GW::HookBase::Deinitialize()
{
    MH_Uninitialize();
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

void GW::HookBase::CreateHook(void *target, void *detour, void **trampoline)
{
    MH_CreateHook(target, detour, trampoline);
}

void GW::HookBase::RemoveHook(void *target)
{
    MH_RemoveHook(target);
}
