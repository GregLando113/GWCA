#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Maybe.h>

namespace GW {
    struct Skill { // total : 0xA0/160
        /* +h0000 */ uint32_t skill_id;
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ uint32_t campaign;
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
        /* +h002C */ uint32_t skill_id_pvp;
        /* +h0030 */ uint8_t combo;
        /* +h0031 */ uint8_t target;
        /* +h0032 */ uint8_t h0032;
        /* +h0033 */ uint8_t skill_equip_type;
        /* +h0034 */ uint8_t h0034;
        /* +h0035 */ uint8_t energy_cost;
        /* +h0036 */ uint8_t health_cost;
        /* +h0037 */ uint8_t h0037;
        /* +h0038 */ uint32_t adrenaline;
        /* +h003C */ float activation;
        /* +h0040 */ float aftercast;
        /* +h0044 */ uint32_t duration0;
        /* +h0048 */ uint32_t duration15;
        /* +h004C */ uint32_t recharge;
        /* +h0050 */ uint8_t h0050[12];
        /* +h005C */ uint32_t scale0;
        /* +h0060 */ uint32_t scale15;
        /* +h0064 */ uint32_t bonusScale0;
        /* +h0068 */ uint32_t bonusScale15;
        /* +h006C */ float aoe_range;
        /* +h0070 */ float const_effect;
        /* +h0074 */ uint8_t h0074[32];
        /* +h0094 */ uint32_t name; // String id
        /* +h0098 */ uint32_t h0098;
        /* +h009C */ uint32_t description; // String id

        uint8_t GetEnergyCost() {
            switch (energy_cost) {
            case 11: return 15;
            case 12: return 25;
            default: return energy_cost;
            }
        }
    };
    static_assert(sizeof(Skill) == 160, "struct Skill has incorect size");

    struct SkillbarSkill { // total: 0x14/20
        /* +h0000 */ uint32_t adrenaline_a;
        /* +h0004 */ uint32_t adrenaline_b;
        /* +h0008 */ uint32_t recharge;
        /* +h000C */ uint32_t skill_id; // see GWConst::SkillIds
        /* +h0010 */ uint32_t event;

        uint32_t GetRecharge() const; // returns recharge time remaining in milliseconds, or 0 if recharged
    };
    static_assert(sizeof(SkillbarSkill) == 20, "struct SkillbarSkill has incorect size");

    struct Skillbar { // total: 0xBC/188
        /* +h0000 */ uint32_t agent_id; // id of the agent whose skillbar this is
        /* +h0004 */ SkillbarSkill skills[8];
        /* +h00A4 */ uint32_t disabled;
        /* +h00A8 */ uint32_t h00A8[2];
        /* +h00B0 */ uint32_t casting;
        /* +h00B4 */ uint32_t h00B4[2];

        bool IsValid() const { return agent_id > 0; }
        // Same as SkillbarArray::GetSkillbarArray()[0] with checks. 
        static Skillbar GetPlayerSkillbar();
        Maybe<SkillbarSkill> GetSkillById(Constants::SkillID skill_id);
    };
    static_assert(sizeof(Skillbar) == 188, "struct Skillbar has incorect size");

    struct SkillbarArray : Array<Skillbar> {
        // Get array of skillbars, [0] = player [1-7] = heroes.
        static SkillbarArray GetSkillbarArray();
    };

    struct Effect { // total: 0x18/24
        /* +h0000 */ uint32_t skill_id; // skill id of the effect
        /* +h0004 */ uint32_t effect_type; // 0 = condition/shout, 8 = stance, 11 = maintained enchantment, 14 = enchantment/nature ritual
        /* +h0008 */ uint32_t effect_id; // unique identifier of effect
        /* +h000C */ uint32_t agent_id; // non-zero means maintained enchantment - caster id
        /* +h0010 */ float duration; // non-zero if effect has a duration
        /* +h0014 */ uint32_t timestamp; // GW-timestamp of when effect was applied - only with duration

        uint32_t GetTimeElapsed() const;
        uint32_t GetTimeRemaining() const;
        static Effect Nil() { return Effect(); }
        Effect() : skill_id(0), effect_type(0), effect_id(0), agent_id(0), duration(0.0f), timestamp(0) {}
    };
    static_assert(sizeof(Effect) == 24, "struct Effect has incorect size");

    struct Buff { // total: 0x10/16
        /* +h0000 */ uint32_t skill_id; // skill id of the buff
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ uint32_t buff_id; // id of buff in the buff array
        /* +h000C */ uint32_t target_agent_id; // agent id of the target (0 if no target)

        static Buff Nil() { return Buff(); }
        Buff() : skill_id(0), h0004(0), buff_id(0), target_agent_id(0) {}
    };
    static_assert(sizeof(Buff) == 16, "struct Buff has incorect size");

    using EffectArray = Array<Effect>;
    using BuffArray = Array<Buff>;

    struct AgentEffects { // total: 0x24/36
        /* +h0000 */ uint32_t agent_id;
        /* +h0004 */ BuffArray buffs;
        /* +h0014 */ EffectArray effects;
    };
    static_assert(sizeof(AgentEffects) == 36, "struct AgentEffects has incorect size");

    using AgentEffectsArray = Array<AgentEffects>;
}
