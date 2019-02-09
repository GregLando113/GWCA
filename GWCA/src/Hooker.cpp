#include "stdafx.h"

#include <GWCA/Utilities/Hooker.h>
#include <Dependencies/minhook/include/MinHook.h>

void GW::HookBase::Initialize()
{
    MH_Initialize();
}

void GW::HookBase::Deinitialize()
{
    MH_Uninitialize();
}

void GW::HookBase::EnqueueHook(HookBase* base)
{
    MH_CreateHook(base->_sourceFunc, base->_detourFunc, &base->_retourFunc);
    EnableHooks(base);
}

void  GW::HookBase::RemoveHook(HookBase* base)
{
    MH_RemoveHook(base->_sourceFunc);
    base->_retourFunc = nullptr;
}

void GW::HookBase::EnableHooks(HookBase* base)
{
    if (base == nullptr)
        MH_EnableHook(MH_ALL_HOOKS);
    else
        MH_EnableHook(base->_sourceFunc);
}

void GW::HookBase::DisableHooks(HookBase* base)
{
    if (base == nullptr)
        MH_DisableHook(MH_ALL_HOOKS);
    else
        MH_DisableHook(base->_sourceFunc);
}
