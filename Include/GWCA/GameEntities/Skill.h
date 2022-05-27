#pragma once
#include <GWCA/Constants/Types.h>

#include <GWCA/GameContainers/Array.h>

namespace GW {
    namespace Constants {
        enum class Campaign;
    }
    struct Skill { // total : 0xA0/160
        /* +h0000 */ Constants::SkillID skill_id;
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ Constants::Campaign campaign;
        /* +h000C */ uint32_t type;
        /* +h0010 */ uint32_t special;
        /* +h0014 */ uint32_t combo_req;
        /* +h0018 */ uint32_t effect1;
        /* +h001C */ uint32_t condition;
        /* +h0020 */ uint32_t effect2;
        /* +h0024 */ uint32_t weapon_req;
        /* +h0028 */ uint8_t profession;
        /* +h0029 */ uint8_t attribute;
        /* +h002A */ uint8_t h002A[2];
        /* +h002C */ Constants::SkillID skill_id_pvp;
        /* +h0030 */ uint8_t combo;
        /* +h0031 */ uint8_t target;
        /* +h0032 */ uint8_t h0032;
        /* +h0033 */ uint8_t skill_equip_type;
        /* +h0034 */ uint8_t overcast; // only if special flag has 0x000001 set
        /* +h0035 */ uint8_t energy_cost;
        /* +h0036 */ uint8_t health_cost;
        /* +h0037 */ uint8_t h0037;
        /* +h0038 */ uint32_t adrenaline;
        /* +h003C */ float activation;
        /* +h0040 */ float aftercast;
        /* +h0044 */ uint32_t duration0;
        /* +h0048 */ uint32_t duration15;
        /* +h004C */ uint32_t recharge;
        /* +h0050 */ uint16_t h0050[4];
        /* +h0058 */ uint32_t skill_arguments; // 1 - duration set, 2 - scale set, 4 - bonus scale set (3 would mean duration and scale is set/used by the skill)
        /* +h005C */ uint32_t scale0;
        /* +h0060 */ uint32_t scale15;
        /* +h0064 */ uint32_t bonusScale0;
        /* +h0068 */ uint32_t bonusScale15;
        /* +h006C */ float aoe_range;
        /* +h0070 */ float const_effect;
        /* +h0074 */ uint32_t caster_overhead_animation_id; //2077 == max == no animation
        /* +h0078 */ uint32_t caster_body_animation_id;
        /* +h007C */ uint32_t target_body_animation_id;
        /* +h0080 */ uint32_t target_overhead_animation_id;
        /* +h0084 */ uint32_t projectile_animation_1_id;
        /* +h0088 */ uint32_t projectile_animation_2_id;
		/* +h008C */ uint32_t icon_file_id;
		/* +h0090 */ uint8_t h0090[4];
        /* +h0094 */ uint32_t name; // String id
        /* +h0098 */ uint32_t concise; // String id
        /* +h009C */ uint32_t description; // String id

        uint8_t GetEnergyCost() const {
            switch (energy_cost) {
            case 11: return 15;
            case 12: return 25;
            default: return energy_cost;
            }
        }
        inline bool IsPvP() { return (special & 0x400000) != 0; }
    };
    static_assert(sizeof(Skill) == 0xa0, "struct Skill has incorect size");

    struct SkillbarSkill { // total: 0x14/20
        /* +h0000 */ uint32_t adrenaline_a;
        /* +h0004 */ uint32_t adrenaline_b;
        /* +h0008 */ uint32_t recharge;
        /* +h000C */ Constants::SkillID skill_id; // see GWConst::SkillIds
        /* +h0010 */ uint32_t event;

        uint32_t GetRecharge() const; // returns recharge time remaining in milliseconds, or 0 if recharged
    };
    static_assert(sizeof(SkillbarSkill) == 20, "struct SkillbarSkill has incorect size");

    struct Skillbar { // total: 0xBC/188
        /* +h0000 */ AgentID agent_id; // id of the agent whose skillbar this is
        /* +h0004 */ SkillbarSkill skills[8];
        /* +h00A4 */ uint32_t disabled;
        /* +h00A8 */ uint32_t h00A8[2];
        /* +h00B0 */ uint32_t casting;
        /* +h00B4 */ uint32_t h00B4[2];

        bool IsValid() const { return agent_id > AgentID::None; }

        SkillbarSkill *GetSkillById(Constants::SkillID skill_id);
    };
    static_assert(sizeof(Skillbar) == 188, "struct Skillbar has incorect size");

    typedef Array<Skillbar> SkillbarArray;

    struct Effect { // total: 0x18/24
        /* +h0000 */ Constants::SkillID skill_id; // skill id of the effect
        /* +h0004 */ uint32_t effect_type; // 0 = condition/shout, 8 = stance, 11 = maintained enchantment, 14 = enchantment/nature ritual
        /* +h0008 */ Constants::EffectID effect_id; // unique identifier of effect
        /* +h000C */ AgentID agent_id; // non-zero means maintained enchantment - caster id
        /* +h0010 */ float duration; // non-zero if effect has a duration
        /* +h0014 */ DWORD timestamp; // GW-timestamp of when effect was applied - only with duration

        DWORD GetTimeElapsed() const;
        DWORD GetTimeRemaining() const;
    };
    static_assert(sizeof(Effect) == 24, "struct Effect has incorect size");

    typedef Array<Effect> EffectArray;

    struct Buff { // total: 0x10/16
        /* +h0000 */ Constants::SkillID skill_id; // skill id of the buff
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ uint32_t buff_id; // id of buff in the buff array
        /* +h000C */ AgentID target_agent_id; // agent id of the target (0 if no target)
    };
    static_assert(sizeof(Buff) == 16, "struct Buff has incorect size");

    typedef Array<Buff> BuffArray;

    struct AgentEffects { // total: 0x24/36
        /* +h0000 */ AgentID agent_id;
        /* +h0004 */ BuffArray buffs;
        /* +h0014 */ EffectArray effects;
    };
    static_assert(sizeof(AgentEffects) == 36, "struct AgentEffects has incorect size");

    typedef Array<AgentEffects> AgentEffectsArray;
}
