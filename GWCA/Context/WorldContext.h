#pragma once

#include <GWCA/GameContainers/Array.h>

#include <GWCA/GameEntities/Map.h>
#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Hero.h>
#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Quest.h>
#include <GWCA/GameEntities/Skill.h>
#include <GWCA/GameEntities/Title.h>
#include <GWCA/GameEntities/Player.h>
#include <GWCA/GameEntities/Attribute.h>

namespace GW {
    struct WorldContext {
        struct sub1 {
            wchar_t* name;
            //...
        } *sub1; // 0x0

        /* +h0004 */ Array<wchar_t> message_buff;
        /* +h0014 */ uint8_t h0014[16];
        /* +h0024 */ MerchItemArray merch_items;
        /* +h0034 */ MerchItemArray merch_items2;
        /* +h0044 */ uint8_t h0044[56];
        /* +h007C */ MapAgentArray map_agents;
        /* +h008C */ uint8_t h008C[16];
        /* +h009C */ Vec3f all_flag;
        /* +h00A8 */ uint8_t h00A8[4];
        /* +h00AC */ PartyAttributeArray attributes;
        /* +h00BC */ uint8_t h00BC[1020];
        /* +h04B8 */ Array<void *> h04B8;
        /* +h04C8 */ Array<void *> h04C8;
        /* +h04D8 */ uint8_t h04D8[4];
        /* +h04DC */ Array<void *> h04DC;
        /* +h04EC */ uint8_t h04EC[28];
        /* +h0508 */ AgentEffectsArray party_effects;
        /* +h0518 */ Array<void *> h0518;
        /* +h0528 */ uint32_t active_quest_id;
        /* +h052C */ QuestLog quest_log;
        /* +h053C */ uint8_t h053C[40];
        /* +h0564 */ Array<MissionObjective> mission_objectives;
        /* +h0574 */ Array<uint32_t> h0574; // Seems like agent/item ids (only appears in outpost?)
        /* +h0584 */ HeroFlagArray hero_flags;
        /* +h0594 */ HeroInfoArray hero_info;
        /* +h05A4 */ Array<void *> h05A4; // Struct size = 0x20
        /* +h05B4 */ uint32_t h05B4[2];
        /* +h05BC */ Array<void *> h05BC;
        /* +h05CC */ Array<void *> h05CC;
        /* +h05DC */ Array<void *> h05DC;
        /* +h05EC */ Array<void *> h05EC;
        /* +h05FC */ Array<void *> h05FC;
        /* +h060C */ Array<uint32_t> unlocked_map;
        /* +h061C */ uint32_t h061C[4];
        /* +h062C */ Array<void *> h062C;
        /* +h063C */ uint8_t h063C[68];
        /* +h0680 */ Array<void *> h0680; // Struct size = 0x138 i think, has a vtable
        /* +h0690 */ uint32_t salvage_session_id;
        /* +h0694 */ uint32_t h0694[10];
        /* +h06BC */ Array<void *> h06BC;
        /* +h06CC */ Array<void *> h06CC;
        /* +h06DC */ uint32_t h06DC;
        /* +h06E0 */ Array<void *> h06E0;
        /* +h06F0 */ SkillbarArray skillbar;
        /* +h0700 */ Array<void *> h0700; // related to +h0710, but always 0
        /* +h0710 */ Array<uint32_t> unlocked_character_skills; // bit field
        /* +h0720 */ Array<void *> h0720;
        /* +h0730 */ Array<void *> h0730;
        /* +h0740 */ uint32_t experience;
        /* +h0744 */ uint32_t experience_dupe;
        /* +h0748 */ uint32_t current_kurzick;
        /* +h074C */ uint32_t current_kurzick_dupe;
        /* +h0750 */ uint32_t total_earned_kurzick;
        /* +h0754 */ uint32_t total_earned_kurzick_dupe;
        /* +h0758 */ uint32_t current_luxon;
        /* +h075C */ uint32_t current_luxon_dupe;
        /* +h0760 */ uint32_t total_earned_luxon;
        /* +h0764 */ uint32_t total_earned_luxon_dupe;
        /* +h0768 */ uint32_t current_imperial;
        /* +h076C */ uint32_t current_imperial_dupe;
        /* +h0770 */ uint32_t total_earned_imperial;
        /* +h0774 */ uint32_t total_earned_imperial_dupe;
        /* +h0778 */ uint32_t unk_faction4;
        /* +h077C */ uint32_t unk_faction4_dupe;
        /* +h0780 */ uint32_t unk_faction5;
        /* +h0784 */ uint32_t unk_faction5_dupe;
        /* +h0788 */ uint32_t level;
        /* +h078C */ uint32_t level_dupe;
        /* +h0790 */ uint32_t unk_faction6;
        /* +h0794 */ uint32_t unk_faction6_dupe;
        /* +h0798 */ uint32_t current_balth;
        /* +h079C */ uint32_t current_balth_dupe;
        /* +h07A0 */ uint32_t total_earned_balth;
        /* +h07A4 */ uint32_t total_earned_balth_dupe;
        /* +h07A8 */ uint32_t current_skill_points;
        /* +h07AC */ uint32_t current_skill_points_dupe;
        /* +h07B0 */ uint32_t total_earned_skill_points;
        /* +h07B4 */ uint32_t total_earned_skill_points_dupe;
        /* +h07B8 */ uint32_t max_kurzick;
        /* +h07BC */ uint32_t max_luxon;
        /* +h07C0 */ uint32_t max_balth;
        /* +h07C4 */ uint32_t max_imperial;
        /* +h07C8 */ uint32_t h07C8;
        /* +h07CC */ AgentInfoArray agent_infos;
        /* +h07DC */ Array<void *> h07DC;
        /* +h07EC */ MissionMapIconArray mission_map_icons;
        /* +h07FC */ NPCArray npcs;
        /* +h080C */ PlayerArray players;
        /* +h081C */ TitleArray titles;
        /* +h082C */ uint8_t h082C[32];
        /* +h084C */ uint32_t foes_killed;
        /* +h0850 */ uint32_t foes_to_kill;
        //... couple more arrays after this
    };
}
