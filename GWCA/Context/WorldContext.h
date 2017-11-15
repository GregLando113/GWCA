#ifndef _WORLD_CONTEXT_INC
#define _WORLD_CONTEXT_INC

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
    using wchar = wchar_t;

    struct WorldContext {
        struct sub1 {
            wchar_t* name;
            //...
        } *sub1; // 0x0

        /* +h0004 */ Array<wchar> message_buff;
        /* +h0014 */ BYTE h0014[0x10];
        /* +h0024 */ MerchItemArray merchitems;
        /* +h0034 */ BYTE h0034[0x48];
        /* +h007C */ MapAgentArray mapagents;
        /* +h008C */ BYTE h008C[0x10];
        /* +h009C */ Vec3f all_flag;
        /* +h00A8 */ BYTE h00A8[0x4];
        /* +h00AC */ PartyAttributeArray attributes;
        /* +h00BC */ BYTE h00BC[0x3FC];
        /* +h04B8 */ Array<void*> h04B8;
        /* +h04C8 */ Array<void*> h04C8;
        /* +h04D8 */ BYTE h04D8[0x4];
        /* +h04DC */ Array<void*> h04DC;
        /* +h04EC */ BYTE h04EC[0x1C];
        /* +h0508 */ AgentEffectsArray partyeffects;
        /* +h0518 */ Array<void*> h0518;
        /* +h0528 */ DWORD activequestid;
        /* +h052C */ QuestLog questlog;
        /* +h053C */ BYTE h053C[0x38];
        /* +h0574 */ Array<DWORD> h0574; // Seems like agent/item ids (only appears in outpost?)
        /* +h0584 */ HeroFlagArray hero_flags;
        /* +h0594 */ Array<void*> h0594;
        /* +h05A4 */ Array<void*> h05A4; // Struct size = 0x20
        /* +h05B4 */ DWORD h05B4[2];
        /* +h05BC */ Array<void*> h05BC;
        /* +h05CC */ Array<void*> h05CC;
        /* +h05DC */ Array<void*> h05DC;
        /* +h05EC */ Array<void*> h05EC;
        /* +h05FC */ Array<void*> h05FC;
        /* +h060C */ Array<void*> h060C;
        /* +h061C */ DWORD h061C[4];
        /* +h062C */ Array<void*> h062C;
        /* +h063C */ BYTE h063C[0x44];
        /* +h0680 */ Array<void*> h0680; // Struct size = 0x138 i think, has a vtable
        /* +h0690 */ DWORD salvagesessionid;
        /* +h0694 */ DWORD h0694[0xA];
        /* +h06BC */ Array<void*> h06BC;
        /* +h06CC */ Array<void*> h06CC;
        /* +h06DC */ DWORD h06DC;
        /* +h06E0 */ Array<void*> h06E0;
        /* +h06F0 */ SkillbarArray skillbar;
        /* +h0700 */ Array<void*> h0700;
        /* +h0710 */ Array<void*> h0710;
        /* +h0720 */ Array<void*> h0720;
        /* +h0730 */ Array<void*> h0730;
        /* +h0740 */ DWORD experience;
        /* +h0744 */ DWORD experience_dupe;
        /* +h0748 */ DWORD currentkurzick;
        /* +h074C */ DWORD currentkurzick_dupe;
        /* +h0750 */ DWORD totalearnedkurzick;
        /* +h0754 */ DWORD totalearnedkurzick_dupe;
        /* +h0758 */ DWORD currentluxon;
        /* +h075C */ DWORD currentluxon_dupe;
        /* +h0760 */ DWORD totalearnedluxon;
        /* +h0764 */ DWORD totalearnedluxon_dupe;
        /* +h0768 */ DWORD currentimperial;
        /* +h076C */ DWORD currentimperial_dupe;
        /* +h0770 */ DWORD totalearnedimperial;
        /* +h0774 */ DWORD totalearnedimperial_dupe;
        /* +h0778 */ DWORD unkfaction4;
        /* +h077C */ DWORD unkfaction4_dupe;
        /* +h0780 */ DWORD unkfaction5;
        /* +h0784 */ DWORD unkfaction5_dupe;
        /* +h0788 */ DWORD level;
        /* +h078C */ DWORD level_dupe;
        /* +h0790 */ DWORD unkfaction6;
        /* +h0794 */ DWORD unkfaction6_dupe;
        /* +h0798 */ DWORD currentbalth;
        /* +h079C */ DWORD currentbalth_dupe;
        /* +h07A0 */ DWORD totalearnedbalth;
        /* +h07A4 */ DWORD totalearnedbalth_dupe;
        /* +h07A8 */ DWORD currentskillpoints;
        /* +h07AC */ DWORD currentskillpoints_dupe;
        /* +h07B0 */ DWORD totalearnedskillpoints;
        /* +h07B4 */ DWORD totalearnedskillpoints_dupe;
        /* +h07B8 */ DWORD maxkurzick;
        /* +h07BC */ DWORD maxluxon;
        /* +h07C0 */ DWORD maxbalth;
        /* +h07C4 */ DWORD maximperial;
        /* +h07C8 */ DWORD unk6;
        /* +h07CC */ AgentInfoArray agentInfos;
        /* +h07DC */ Array<void*> h07DC;
        /* +h07EC */ MissionMapIconArray missionmapicons;
        /* +h07FC */ NPCArray npcs;
        /* +h080C */ PlayerArray players;
        /* +h081C */ TitleArray titles;
        /* +h082C */ BYTE pad9[0x20];
        /* +h084C */ DWORD foes_killed;
        /* +h0850 */ DWORD foes_to_kill;
        //... couple more arrays after this
    };
}

#endif // _WORLD_CONTEXT_INC
