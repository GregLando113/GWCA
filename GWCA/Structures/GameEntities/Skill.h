#pragma once
#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
	namespace GW {
		struct Skill {							// total : A0 BYTEs
			DWORD SkillId;						// 0000
			BYTE Unknown1[4];					// 0004
			long Campaign;						// 0008
			long Type;							// 000C
			DWORD Special;
			long ComboReq;						// 0014
			DWORD Effect1;
			DWORD Condition;
			DWORD Effect2;
			DWORD WeaponReq;
			BYTE Profession;					// 0028
			BYTE Attribute;						// 0029
			BYTE Unknown2[2];					// 002A
			long SkillId_PvP;					// 002C
			BYTE Combo;							// 0030
			BYTE Target;						// 0031
			BYTE unknown3;						// 0032
			BYTE SkillEquipType;				// 0033
			BYTE Unknown4;						// 0034
			BYTE EnergyCost;
			BYTE HealthCost;
			BYTE Unknown7;
			DWORD Adrenaline;					// 0038
			float Activation;					// 003C
			float Aftercast;					// 0040
			long Duration0;						// 0044
			long Duration15;					// 0048
			long Recharge;						// 004C
			BYTE Unknown5[12];					// 0050
			long Scale0;						// 005C
			long Scale15;						// 0060
			long BonusScale0;					// 0064
			long BonusScale15;					// 0068
			float AoERange;						// 006C
			float ConstEffect;					// 0070
			BYTE unknown6[44];					// 0074

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
			//long GetRecharge() const {		// returns recharge time remaining in milliseconds, or 0 if recharged
			//  if (Recharge == 0) return 0;
			//  return Recharge - MemoryMgr::GetSkillTimer();
			//}
		};
		struct Skillbar {						// total : BC BYTEs
			Skillbar() : AgentId(0) {}
			DWORD AgentId;						// 0000						id of the agent whose skillbar this is
			SkillbarSkill Skills[8];			// 0004
			DWORD Disabled;
			BYTE unknown1[8];					// 00A8	|--8 BYTEs--|
			DWORD Casting;						// 00B0
			BYTE unknown2[8];					// 00B4	|--8 BYTEs--|
			static Skillbar Nil() { return Skillbar(); }
			bool IsValid() const { return AgentId > 0; }
		};

		typedef gw_array<Skillbar> SkillbarArray;

		struct Effect {							// total : 18 BYTEs
			DWORD SkillId;						// 0000						skill id of the effect
			long EffectType;					// 0004						type classifier 0 = condition/shout, 8 = stance, 11 = maintained enchantment, 14 = enchantment/nature ritual
			DWORD EffectId;						// 0008						unique identifier of effect
			DWORD AgentId;						// 000C						non-zero means maintained enchantment - caster id
			float Duration;						// 0010						non-zero if effect has a duration
			DWORD TimeStamp;					// 0014						GW-timestamp of when effect was applied - only with duration

			//long GetTimeElapsed() const { return MemoryMgr::GetSkillTimer() - TimeStamp; }
			//long GetTimeRemaining() const { return (long)(Duration * 1000) - GetTimeElapsed(); }
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

		using EffectArray = gw_array<Effect>;
		using BuffArray = gw_array<Buff>;


		struct AgentEffects {
			DWORD AgentId;
			BuffArray Buffs;
			EffectArray Effects;
		};

		using AgentEffectsArray = gw_array<AgentEffects>;
	}
}
