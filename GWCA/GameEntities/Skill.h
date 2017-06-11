#pragma once
#include <Windows.h>

#include <GWCA\Constants\Constants.h>
#include <GWCA\GameContainers\Array.h>
#include <GWCA\Utilities\Maybe.h>

namespace GW {
	struct Skill { // total : A0 BYTEs
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

	struct SkillbarSkill {
		long AdrenalineA;				// 0000
		long AdrenalineB;				// 0004
		DWORD Recharge;					// 0008
		DWORD SkillId;					// 000C						see GWConst::SkillIds
		DWORD Event;					// 0010	s

		long GetRecharge() const;		// returns recharge time remaining in milliseconds, or 0 if recharged
	};
	struct Skillbar {						// total : BC BYTEs
		Skillbar() : AgentId(0) {}
		DWORD AgentId;						// 0000						id of the agent whose skillbar this is
		SkillbarSkill Skills[8];			// 0004
		DWORD Disabled;
		BYTE unknown1[8];					// 00A8	|--8 BYTEs--|
		DWORD Casting;						// 00B0
		BYTE unknown2[8];					// 00B4	|--8 BYTEs--|
		bool IsValid() const { return AgentId > 0; }

		// Same as SkillbarArray::GetSkillbarArray()[0] with checks. 
		static Skillbar GetPlayerSkillbar();
        Maybe<SkillbarSkill> GetSkillById(Constants::SkillID skillId);
	};

	struct SkillbarArray : Array<Skillbar> {
		// Get array of skillbars, [0] = player [1-7] = heroes.
		static SkillbarArray GetSkillbarArray();
	};

	struct Effect {							// total : 18 BYTEs
		DWORD SkillId;						// 0000						skill id of the effect
		long EffectType;					// 0004						type classifier 0 = condition/shout, 8 = stance, 11 = maintained enchantment, 14 = enchantment/nature ritual
		DWORD EffectId;						// 0008						unique identifier of effect
		DWORD AgentId;						// 000C						non-zero means maintained enchantment - caster id
		float Duration;						// 0010						non-zero if effect has a duration
		DWORD TimeStamp;					// 0014						GW-timestamp of when effect was applied - only with duration

		long GetTimeElapsed() const;
		long GetTimeRemaining() const;
		static Effect Nil() { return Effect(); }
		Effect() : SkillId(0), EffectType(0), EffectId(0), AgentId(0), Duration(0.0f), TimeStamp(0) {}
	};

	struct Buff {							// total : 10 bytes
		DWORD SkillId;						// 0000						skill id of the buff
		DWORD Unknown1;						// 0004
		DWORD BuffId;						// 0008						id of buff in the buff array
		DWORD TargetAgentId;				// 000C						agent id of the target (0 if no target)
		static Buff Nil() { return Buff(); }
		Buff() : SkillId(0), Unknown1(0), BuffId(0), TargetAgentId(0) {}
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
