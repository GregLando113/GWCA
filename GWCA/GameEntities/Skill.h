#ifndef _ENTITIE_SKILL_INC
#define _ENTITIE_SKILL_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>
#include <GWCA\Utilities\Maybe.h>
#include <GWCA\Constants\Constants.h>

namespace GW {
    struct Skill { // total : 0xA0/160
        /* +h0000 */ DWORD SkillId;
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ DWORD Campaign;
        /* +h000C */ DWORD Type;
        /* +h0010 */ DWORD Special;
        /* +h0014 */ DWORD ComboReq;
        /* +h0018 */ DWORD Effect1;
        /* +h001C */ DWORD Condition;
        /* +h0020 */ DWORD Effect2;
        /* +h0024 */ DWORD WeaponReq;
        /* +h0028 */ BYTE Profession;
        /* +h0029 */ BYTE Attribute;
        /* +h002A */ BYTE h002A[2];
        /* +h002C */ DWORD SkillId_PvP;
        /* +h0030 */ BYTE Combo;
        /* +h0031 */ BYTE Target;
        /* +h0032 */ BYTE h0032;
        /* +h0033 */ BYTE SkillEquipType;
        /* +h0034 */ BYTE h0034;
        /* +h0035 */ BYTE EnergyCost;
        /* +h0036 */ BYTE HealthCost;
        /* +h0037 */ BYTE h0037;
        /* +h0038 */ DWORD Adrenaline;
        /* +h003C */ float Activation;
        /* +h0040 */ float Aftercast;
        /* +h0044 */ DWORD Duration0;
        /* +h0048 */ DWORD Duration15;
        /* +h004C */ DWORD Recharge;
        /* +h0050 */ BYTE h0050[12];
        /* +h005C */ DWORD Scale0;
        /* +h0060 */ DWORD Scale15;
        /* +h0064 */ DWORD BonusScale0;
        /* +h0068 */ DWORD BonusScale15;
        /* +h006C */ float AoERange;
        /* +h0070 */ float ConstEffect;
        /* +h0074 */ BYTE h0074[32];
        /* +h0094 */ DWORD Name; // String id
        /* +h0098 */ DWORD h0098;
        /* +h009C */ DWORD Description; // String id

        BYTE GetEnergyCost() {
            switch (EnergyCost) {
            case 11: return 15;
            case 12: return 25;
            default: return EnergyCost;
            }
        }
    };

    struct SkillbarSkill { // total: 0x14/20
        /* +h0000 */ DWORD AdrenalineA;
        /* +h0004 */ DWORD AdrenalineB;
        /* +h0008 */ DWORD Recharge;
        /* +h000C */ DWORD SkillId; // see GWConst::SkillIds
        /* +h0010 */ DWORD Event;

        long GetRecharge() const; // returns recharge time remaining in milliseconds, or 0 if recharged
    };

    struct Skillbar { // total: 0xBC/188
        /* +h0000 */ DWORD AgentId; // id of the agent whose skillbar this is
        /* +h0004 */ SkillbarSkill Skills[8];
        /* +h00A4 */ DWORD Disabled;
        /* +h00A8 */ DWORD h00A8[2];
        /* +h00B0 */ DWORD Casting;
        /* +h00B4 */ DWORD h00B4[2];

        bool IsValid() const { return AgentId > 0; }
        // Same as SkillbarArray::GetSkillbarArray()[0] with checks. 
        static Skillbar GetPlayerSkillbar();
        Maybe<SkillbarSkill> GetSkillById(Constants::SkillID skillId);
    };

    struct SkillbarArray : Array<Skillbar> {
        // Get array of skillbars, [0] = player [1-7] = heroes.
        static SkillbarArray GetSkillbarArray();
    };

    struct Effect { // total: 0x18/24
        /* +h0000 */ DWORD SkillId; // skill id of the effect
        /* +h0004 */ long EffectType; // 0 = condition/shout, 8 = stance, 11 = maintained enchantment, 14 = enchantment/nature ritual
        /* +h0008 */ DWORD EffectId; // unique identifier of effect
        /* +h000C */ DWORD AgentId; // non-zero means maintained enchantment - caster id
        /* +h0010 */ float Duration; // non-zero if effect has a duration
        /* +h0014 */ DWORD TimeStamp; // GW-timestamp of when effect was applied - only with duration

        long GetTimeElapsed() const;
        long GetTimeRemaining() const;
        static Effect Nil() { return Effect(); }
        Effect() : SkillId(0), EffectType(0), EffectId(0), AgentId(0), Duration(0.0f), TimeStamp(0) {}
    };

    struct Buff { // total: 0x10/16
        /* +h0000 */ DWORD SkillId; // skill id of the buff
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ DWORD BuffId; // id of buff in the buff array
        /* +h000C */ DWORD TargetAgentId; // agent id of the target (0 if no target)

        static Buff Nil() { return Buff(); }
        Buff() : SkillId(0), h0004(0), BuffId(0), TargetAgentId(0) {}
    };

    using EffectArray = Array<Effect>;
    using BuffArray = Array<Buff>;


    struct AgentEffects {
        DWORD AgentId;
        BuffArray Buffs;
        EffectArray Effects;
    };

    using AgentEffectsArray = Array<AgentEffects>;
}

#endif // _ENTITIE_SKILL_INC
