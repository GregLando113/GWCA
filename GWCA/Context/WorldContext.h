#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>
#include <GWCA\GameEntities\Agent.h>
#include <GWCA\GameEntities\Item.h>
#include <GWCA\GameEntities\Attribute.h>
#include <GWCA\GameEntities\Hero.h>
#include <GWCA\GameEntities\NPC.h>
#include <GWCA\GameEntities\Player.h>
#include <GWCA\GameEntities\Skill.h>
#include <GWCA\GameEntities\Title.h>
#include <GWCA\GameEntities\Quest.h>
#include <GWCA\GameEntities\Map.h>

namespace GW {

	class WorldContext {
	public:
		WorldContext() {}
		WorldContext(const WorldContext&) {}

		struct sub1 {
			wchar_t* name;
			//...
		} *sub1; // 0x0
		Array<wchar_t> message_buff; // 0x4 Last chat message recieved via gameserver
		BYTE pad1[0x10]; // 0x14
		MerchItemArray merchitems; // 0x24
		BYTE pad1_1[0x48]; // 0x34
		MapAgentArray mapagents; // 0x7C
		BYTE pad2[0x10]; // 0x8C
		GamePos all_flag; // 0x9C
		BYTE pad3[0x4]; // 0xA8
		PartyAttributeArray attributes; // 0xAC
		BYTE pad4[0x3FC]; // 0xBC
		Array<void*> unk1_array; // 0x4B8
		Array<void*> unk2_array; // 0x4C8
		BYTE pad5[0x4]; // 0x4D8
		Array<void*> unk3_array; // 0x4DC
		BYTE pad6[0x1C]; // 0x4EC
		AgentEffectsArray partyeffects; // 0x508
		Array<void*> unk4_array; // 0x518
		DWORD activequestid; // 0x528
		QuestLog questlog; // 0x52C
		BYTE pad7[0x38]; // 0x53C
		Array<DWORD> unk5_array; // 0x574 - Seems like agent/item ids (only appears in outpost?)
		HeroFlagArray hero_flags; // 0x584
		Array<void*> unk6_array; // 0x594
		Array<void*> unk7_array; // 0x5A4 - Struct size = 0x20
		DWORD unk2[2]; // 0x5B4
		Array<void*> unk8_array; // 0x5BC
		Array<void*> unk9_array; // 0x5CC
		Array<void*> unk10_array; // 0x5DC
		Array<void*> unk11_array; // 0x5EC
		Array<void*> unk12_array; // 0x5FC
		Array<void*> unk13_array; // 0x60C
		DWORD unk3[4]; // 0x61C
		Array<void*> unk14_array; // 0x62C
		BYTE pad8[0x44]; // 0x63C
		Array<void*> unk15_array; // 0x680 - Struct size = 0x138 i think, has a vtable
		DWORD salvagesessionid; // 0x690
		DWORD unk4[0xA]; // 0x694
		Array<void*> unk16_array; // 0x6BC
		Array<void*> unk17_array; // 0x6CC
		DWORD unk5; // 0x6DC
		Array<void*> unk18_array; // 0x6E0
		SkillbarArray skillbar; // 0x6F0
		Array<void*> unk19_array; // 0x700
		Array<void*> unk20_array; // 0x710
		Array<void*> unk21_array; // 0x720
		Array<void*> unk22_array; // 0x730
		DWORD experience; // 0x740
		DWORD experience_dupe; // 0x744
		DWORD currentkurzick; // 0x748
		DWORD currentkurzick_dupe; // 0x74C
		DWORD totalearnedkurzick; // 0x750
		DWORD totalearnedkurzick_dupe; // 0x754
		DWORD currentluxon; // 0x758
		DWORD currentluxon_dupe; // 0x75C
		DWORD totalearnedluxon; // 0x760
		DWORD totalearnedluxon_dupe; // 0x764
		DWORD currentimperial; // 0x768
		DWORD currentimperial_dupe; // 0x76C
		DWORD totalearnedimperial; // 0x770
		DWORD totalearnedimperial_dupe; // 0x774
		DWORD unkfaction4; // 0x778
		DWORD unkfaction4_dupe; // 0x77C
		DWORD unkfaction5; // 0x780
		DWORD unkfaction5_dupe; // 0x784
		DWORD level; // 0x788
		DWORD level_dupe; // 0x78C
		DWORD unkfaction6; // 0x790
		DWORD unkfaction6_dupe; // 0x794
		DWORD currentbalth; // 0x798
		DWORD currentbalth_dupe; // 0x79C
		DWORD totalearnedbalth; // 0x7A0
		DWORD totalearnedbalth_dupe; // 0x7A4
		DWORD currentskillpoints; // 0x7A8
		DWORD currentskillpoints_dupe; // 0x7AC
		DWORD totalearnedskillpoints; // 0x7B0
		DWORD totalearnedskillpoints_dupe; // 0x7B4
		DWORD maxkurzick; // 0x7B8
		DWORD maxluxon; // 0x7BC
		DWORD maxbalth; // 0x7C0
		DWORD maximperial; // 0x7C4
		DWORD unk6; // 0x7C8
		AgentInfoArray agentInfos; // 0x7CC
		Array<void*> unk24_array; // 0x7DC
		MissionMapIconArray missionmapicons; // 0x7EC
		NPCArray npcs; // 0x7FC
		PlayerArray players; // 0x80C
		TitleArray titles; // 0x81C
		BYTE pad9[0x20]; // 0x82C
		DWORD foes_killed; // 0x84C
		DWORD foes_to_kill; // 0x850
		//... couple more arrays after this
	};
}
