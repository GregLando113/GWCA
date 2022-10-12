#include "stdafx.h"

#include <GWCA/Constants/Skills.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>

#include <GWCA/GameEntities/Skill.h>

#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/EffectMgr.h>

namespace {
    using namespace GW;

    uint32_t alcohol_level = 0;
    // post processing effects hook
    typedef void(__cdecl *PostProcessEffect_pt)(uint32_t intensity, uint32_t tint);
    PostProcessEffect_pt RetPostProcessEffect = 0;
    PostProcessEffect_pt PostProcessEffect_Func = 0;

    typedef void(__cdecl* DropBuff_pt)(uint32_t buff_id);
    DropBuff_pt DropBuff_Func = 0;

    void __cdecl OnPostProcessEffect(uint32_t intensity, uint32_t tint) {
        HookBase::EnterHook();
        alcohol_level = intensity;
        RetPostProcessEffect(intensity, tint);
        HookBase::LeaveHook();
    }



    void Init() {
        PostProcessEffect_Func = (PostProcessEffect_pt)Scanner::Find("\xD9\x5D\x0C\xD9\x45\x0C\x8D\x45\xF8", "xxxxxxxxx", -0x1C);

        DWORD address = Scanner::Find("\xf6\x40\x04\x01\x74\x10", "xxxxxx", 0x9);
        DropBuff_Func = (DropBuff_pt)Scanner::FunctionFromNearCall(address);

        GWCA_INFO("[SCAN] PostProcessEffect Function = %p", PostProcessEffect_Func);
        GWCA_INFO("[SCAN] DropBuff Function = %p", DropBuff_Func);
#ifdef _DEBUG
        GWCA_ASSERT(PostProcessEffect_Func);
        GWCA_ASSERT(DropBuff_Func);
#endif

        HookBase::CreateHook(PostProcessEffect_Func, OnPostProcessEffect, (void**)&RetPostProcessEffect);
    }

    void DisableHooks() {
        if (PostProcessEffect_Func)
            HookBase::DisableHooks(PostProcessEffect_Func);
    }
    void EnableHooks() {
        if (PostProcessEffect_Func)
            HookBase::EnableHooks(PostProcessEffect_Func);
    }

    void Exit() {
        HookBase::RemoveHook(PostProcessEffect_Func);
    }

}

namespace GW {
    Module EffectModule = {
        "EffectModule",     // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,             // exit_module
        ::EnableHooks,               // enable_hooks
        ::DisableHooks,               // disable_hooks
    };
    namespace Effects {

        uint32_t GetAlcoholLevel() {
            return alcohol_level;
        }

        void GetDrunkAf(uint32_t intensity, uint32_t tint) {
            RetPostProcessEffect(intensity, tint);
        }



        Effect* GetPlayerEffectBySkillId(Constants::SkillID skill_id) {
            auto* effects = GetPlayerEffects();
            if (!effects) return nullptr;
            for (auto& effect : *effects) {
                if (effect.skill_id == skill_id)
                    return &effect;
            }
            return nullptr;
        }

        Buff* GetPlayerBuffBySkillId(Constants::SkillID skill_id) {
            auto* buffs = GetPlayerBuffs();
            if (!buffs) return nullptr;
            for (auto& buff : *buffs) {
                if (buff.skill_id == skill_id)
                    return &buff;
            }
            return nullptr;
        }
        AgentEffectsArray* GetPartyEffectsArray() {
            auto* w = GetWorldContext();
            return w && w->party_effects.valid() ? &w->party_effects : nullptr;
        }
        AgentEffects* GetAgentEffectsArray(uint32_t agent_id) {
            AgentEffectsArray* agent_effects = GetPartyEffectsArray();
            if (!agent_effects) return nullptr;
            for (auto& agent_effect : *agent_effects) {
                if (agent_effect.agent_id == agent_id)
                    return &agent_effect;
            }
            return nullptr;
        }
        AgentEffects* GetPlayerEffectsArray() {
            return GetAgentEffectsArray(Agents::GetPlayerId());
        }
        EffectArray* GetAgentEffects(uint32_t agent_id) {
            auto* e = GetAgentEffectsArray(agent_id);
            return e && e->effects.valid() ? &e->effects : nullptr;
        }
        BuffArray* GetAgentBuffs(uint32_t agent_id) {
            auto* e = GetAgentEffectsArray(agent_id);
            return e && e->buffs.valid() ? &e->buffs : nullptr;
        }
        EffectArray* GetPlayerEffects() {
            return GetAgentEffects(Agents::GetPlayerId());
        }

        BuffArray* GetPlayerBuffs() {
            return GetAgentBuffs(Agents::GetPlayerId());
        }

        bool DropBuff(uint32_t buff_id) {
            if (!DropBuff_Func)
                return false;
            DropBuff_Func(buff_id);
            return true;
        }
    }
} // namespace GW
