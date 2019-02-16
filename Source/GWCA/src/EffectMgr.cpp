#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Skills.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/Vector.h>

#include <GWCA/Packets/StoC.h>

#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/StoCMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/EffectMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace {
    using namespace GW;

    uint32_t alcohol_level = 0;
    // post processing effects hook
    typedef void(__fastcall *PostProcessEffect_pt)(uint32_t intensity, uint32_t tint);
    PostProcessEffect_pt RetPostProcessEffect;
    PostProcessEffect_pt PostProcessEffect_Func;

    void __fastcall OnPostProcessEffect(uint32_t intensity, uint32_t tint) {
        HookBase::EnterHook();
        alcohol_level = intensity;
        RetPostProcessEffect(intensity, tint);
        HookBase::LeaveHook();
    }

    void Init() {
        PostProcessEffect_pt PostProcessEffect_Func = (PostProcessEffect_pt)Scanner::Find(
            "\x55\x8B\xEC\x83\xEC\x10\x89\x4D\xF8\xC7\x45\xFC", "xxxxxxxxxxxx", 0);
        printf("[SCAN] PostProcessEffect = %p\n", PostProcessEffect_Func);
    }

    void CreateHooks() {
        if (Verify(PostProcessEffect_Func))
            HookBase::CreateHook(PostProcessEffect_Func, OnPostProcessEffect, (void **)&RetPostProcessEffect);
    }

    void RemoveHooks() {
        if (PostProcessEffect_Func)
            HookBase::RemoveHook(PostProcessEffect_Func);
    }

}

namespace GW {
    Module EffectModule = {
        "EffectModule",     // name
        NULL,               // param
        ::Init,             // init_module
        NULL,               // exit_module
        ::CreateHooks,      // exit_module
        ::RemoveHooks,      // remove_hooks
    };

    uint32_t Effects::GetAlcoholLevel() {
        return alcohol_level;
    }

    void Effects::GetDrunkAf(uint32_t intensity, uint32_t tint) {
        RetPostProcessEffect(intensity, tint);
    }

    Effect *Effects::GetPlayerEffectById(Constants::SkillID skill_id) {
        uint32_t id = static_cast<uint32_t>(skill_id);
        AgentEffectsArray AgEffects = GetPartyEffectArray();

        if (AgEffects.valid()) {
            EffectArray effects = AgEffects[0].effects;
            if (effects.valid()) {
                for (uint32_t i = 0; i < effects.size(); i++) {
                    if (effects[i].skill_id == id)
                        return &effects[i];
                }
            }
        }

        return NULL;
    }

    Buff *Effects::GetPlayerBuffBySkillId(Constants::SkillID skill_id) {
        uint32_t id = static_cast<uint32_t>(skill_id);
        AgentEffectsArray agent_effects = GetPartyEffectArray();

        if (agent_effects.valid()) {
            BuffArray buffs = agent_effects[0].buffs;
            if (buffs.valid()) {
                for (uint32_t i = 0; i < buffs.size(); i++) {
                    if (buffs[i].skill_id == id)
                        return &buffs[i];
                }
            }
        }

        return NULL;
    }

    EffectArray Effects::GetPlayerEffectArray() {
        AgentEffectsArray agent_effects = GetPartyEffectArray();
        if (agent_effects.valid()) {
            return agent_effects[0].effects;
        } else {
            return EffectArray();
        }
    }

    AgentEffectsArray Effects::GetPartyEffectArray() {
        return GameContext::instance()->world->party_effects;
    }

    BuffArray Effects::GetPlayerBuffArray() {
        AgentEffectsArray ageffects = GetPartyEffectArray();
        if (ageffects.valid()) {
            return ageffects[0].buffs;
        } else {
            return BuffArray();
        }
    }

    void Effects::DropBuff(uint32_t buff_id) {
        CtoS::SendPacket(0x8, CtoGS_MSGDropBuff, buff_id);
    }
} // namespace GW
