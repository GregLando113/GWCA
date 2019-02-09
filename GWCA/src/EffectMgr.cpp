#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Skills.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>

#include <GWCA/GameContainers/List.h>
#include <GWCA/GameContainers/Vector.h>

#include <GWCA/Packets/StoC.h>

#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/StoCMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/EffectMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace {
    DWORD alcohol_level = 0;
    // post processing effects hook
    typedef void(__fastcall *PPEFunc_t)(DWORD Intensity, DWORD Tint);
    GW::THook<PPEFunc_t> ppe_hook;
    void __fastcall ppe_detour(DWORD Intensity, DWORD Tint) {
        alcohol_level = Intensity;
        ppe_hook.Original()(Intensity, Tint);
    }

    struct MSG_EFFECT_APPLIED {
        DWORD header = 54;
        DWORD agent_id;
        DWORD skill_id; // skill id that will define the image shown.
        DWORD effect_type;
        DWORD effect_id; // sync with server, but it's not an index of a array so can be anything (I think ?).
        float duration; // in seconds.
    };

    struct MSG_EFFECT_REAPPLIED {
        DWORD header = 55;
        DWORD agent_id;
        DWORD effect_type;
        DWORD effect_id; // sync with server, but it's not an index of a array so can be anything (I think ?).
        float duration; // in seconds.
    };

    struct MSG_EFFECT_REMOVED {
        DWORD header = 56;
        DWORD agent_id;
        DWORD effect_id;
    };
}

void GW::Effects::SetupPostProcessingEffectHook() {
    if (ppe_hook.Empty()) {
        PPEFunc_t ppe_original = (PPEFunc_t)Scanner::Find("\x55\x8B\xEC\x83\xEC\x10\x89\x4D\xF8\xC7\x45\xFC", "xxxxxxxxxxxx", 0);
        printf("[SCAN] PostProcessEffectFunction = %p\n", ppe_original);
        ppe_hook.Detour(ppe_original, ppe_detour, 6);
    }   
}

void GW::Effects::RestoreHooks() {
    HookBase::DisableHooks(&ppe_hook);
}

DWORD GW::Effects::GetAlcoholLevel() {
    return alcohol_level;
}

void GW::Effects::GetDrunkAf(DWORD Intensity, DWORD Tint) {
    ppe_hook.Original()(Intensity, Tint);
}

GW::Effect GW::Effects::GetPlayerEffectById(GW::Constants::SkillID SkillID) {
    DWORD id = static_cast<DWORD>(SkillID);
    GW::AgentEffectsArray AgEffects = GetPartyEffectArray();

    if (AgEffects.valid()) {
        GW::EffectArray Effects = AgEffects[0].effects;
        if (Effects.valid()) {
            for (DWORD i = 0; i < Effects.size(); i++) {
                if (Effects[i].skill_id == id) return Effects[i];
            }
        }
    }

    return GW::Effect::Nil();
}

GW::Buff GW::Effects::GetPlayerBuffBySkillId(GW::Constants::SkillID SkillID) {
    DWORD id = static_cast<DWORD>(SkillID);
    GW::AgentEffectsArray AgEffects = GetPartyEffectArray();

    if (AgEffects.valid()) {
        GW::BuffArray Buffs = AgEffects[0].buffs;
        if (Buffs.valid()) {
            for (DWORD i = 0; i < Buffs.size(); i++) {
                if (Buffs[i].skill_id == id) return Buffs[i];
            }
        }
    }

    return GW::Buff::Nil();
}

GW::EffectArray GW::Effects::GetPlayerEffectArray() {
    GW::AgentEffectsArray ageffects = GetPartyEffectArray();
    if (ageffects.valid()) {
        return ageffects[0].effects;
    } else {
        return GW::EffectArray();
    }
}

GW::AgentEffectsArray GW::Effects::GetPartyEffectArray() {
    return GameContext::instance()->world->party_effects;
}

GW::BuffArray GW::Effects::GetPlayerBuffArray() {
    GW::AgentEffectsArray ageffects = GetPartyEffectArray();
    if (ageffects.valid()) {
        return ageffects[0].buffs;
    } else {
        return GW::BuffArray();
    }
}

void GW::Effects::DropBuff(DWORD buffId) {
    CtoS::SendPacket(0x8, CtoGS_MSGDropBuff, buffId);
}

#if 0 // deprecated
MSG_EFFECT_APPLIED applied_effect;
void GW::Effects::CreateEffect(DWORD effect_id, DWORD skill_id, float duration) {
    applied_effect.header = 58;
    applied_effect.agent_id = GW::Agents::GetPlayerId();
    applied_effect.skill_id = skill_id;
    applied_effect.effect_type = 0;
    applied_effect.effect_id = effect_id;
    applied_effect.duration = duration;

    GW::GameThread::Enqueue([](){
        /*
        typedef bool (__fastcall *EffectApplied_t)(MSG_EFFECT_APPLIED *effect);
        EffectApplied_t EffectApplied = (EffectApplied_t)0x00888020; // Emulate SMSG_054
        EffectApplied(&applied_effect);
        */
        GW::StoC::EmulatePacket((Packet::StoC::PacketBase*)&applied_effect);
    });
}

MSG_EFFECT_REAPPLIED reapplied_effect;
void GW::Effects::ReapplieEffect(DWORD effect_id, float duration) {
    reapplied_effect.header = 59;
    reapplied_effect.agent_id = GW::Agents::GetPlayerId();
    reapplied_effect.effect_type = 0;
    reapplied_effect.effect_id = effect_id;
    reapplied_effect.duration = duration;

    GW::GameThread::Enqueue([]() {
        /*
        typedef bool(__fastcall *EffectReapplied_t)(MSG_EFFECT_REAPPLIED *effect);
        EffectReapplied_t EffectReapplied = (EffectReapplied_t)0x00888040; // Emulate SMSG_055
        EffectReapplied(&reapplied_effect);
        */
        GW::StoC::EmulatePacket((Packet::StoC::PacketBase*)&reapplied_effect);
    });
}

MSG_EFFECT_REMOVED removed_effect;
void GW::Effects::RemoveEffect(DWORD effect_id) {
    removed_effect.header = 60;
    removed_effect.agent_id = GW::Agents::GetPlayerId();
    removed_effect.effect_id = effect_id;

    GW::GameThread::Enqueue([]() {
        /*
        typedef bool(__fastcall *EffectRemoved_t)(MSG_EFFECT_REMOVED *effect);
        EffectRemoved_t EffectRemoved = (EffectRemoved_t)0x00888060; // Emulate SMSG_056
        EffectRemoved(&removed_effect);
        */
        GW::StoC::EmulatePacket((Packet::StoC::PacketBase*)&removed_effect);
    });
}
#endif