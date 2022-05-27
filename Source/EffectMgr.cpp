#include "stdafx.h"

#include <GWCA/Packets/Opcodes.h>
#include <GWCA/Constants/Skills.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/GamePos.h>

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
    typedef void(__cdecl *PostProcessEffect_pt)(uint32_t intensity, uint32_t tint);
    PostProcessEffect_pt RetPostProcessEffect;
    PostProcessEffect_pt PostProcessEffect_Func;

    void __cdecl OnPostProcessEffect(uint32_t intensity, uint32_t tint) {
        HookBase::EnterHook();
        alcohol_level = intensity;
        RetPostProcessEffect(intensity, tint);
        HookBase::LeaveHook();
    }

    void Init() {
        PostProcessEffect_Func = (PostProcessEffect_pt)Scanner::Find(
            "\xD9\x5D\x0C\xD9\x45\x0C\x8D\x45\xF8", "xxxxxxxxx", -0x1C);
        GWCA_INFO("[SCAN] PostProcessEffect = %p\n", PostProcessEffect_Func);

        if (Verify(PostProcessEffect_Func))
            HookBase::CreateHook(PostProcessEffect_Func, OnPostProcessEffect, (void **)&RetPostProcessEffect);
    }

    void Exit() {
        if (PostProcessEffect_Func)
            HookBase::RemoveHook(PostProcessEffect_Func);
    }

}

namespace GW {
    Module EffectModule = {
        "EffectModule",     // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,             // exit_module
        NULL,               // enable_hooks
        NULL,               // disable_hooks
    };

    uint32_t Effects::GetAlcoholLevel() {
        return alcohol_level;
    }

    void Effects::GetDrunkAf(uint32_t intensity, uint32_t tint) {
        RetPostProcessEffect(intensity, tint);
    }

    Effect *Effects::GetPlayerEffectById(Constants::SkillID skill_id) {
        auto* effects = GetPlayerEffectArray();
        for (uint32_t i = 0; effects && i < effects->size(); i++) {
            auto& effect = effects->at(i);
            if (effect.skill_id == skill_id)
                return &effect;
        }
        return nullptr;
    }

    Buff *Effects::GetPlayerBuffBySkillId(Constants::SkillID skill_id) {
        auto* buffs = GetPlayerBuffArray();
        for (uint32_t i = 0; buffs && i < buffs->size(); i++) {
            auto& buff = buffs->at(i);
            if (buff.skill_id == skill_id)
                return &buff;
        }
        return nullptr;
    }

    AgentEffects* Effects::GetAgentEffectArray(AgentID agent_id) {
        if (agent_id == AgentID::None) return nullptr;
        auto* agent_effects = GetPartyEffectArray();
        for (size_t i = 0; agent_effects && i < agent_effects->size(); i++) {
            auto& effect = agent_effects->at(i);
            if (effect.agent_id == agent_id)
                return &effect;
        }
        return nullptr;
    }

    EffectArray* Effects::GetPlayerEffectArray() {
        auto* agent_effects = GetAgentEffectArray(Agents::GetPlayerId());
        return agent_effects ? &agent_effects->effects : nullptr;
    }

    AgentEffectsArray* Effects::GetPartyEffectArray() {
        auto* w = WorldContext::instance();
        return w && w->party_effects.valid() ? &w->party_effects : nullptr;
    }

    BuffArray* Effects::GetPlayerBuffArray() {
        auto* agent_effects = GetAgentEffectArray(Agents::GetPlayerId());
        return agent_effects ? &agent_effects->buffs : nullptr;
    }

    void Effects::DropBuff(uint32_t buff_id) {
        CtoS::SendPacket(0x8, GAME_CMSG_DROP_BUFF, buff_id);
    }
} // namespace GW
