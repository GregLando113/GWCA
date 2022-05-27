#pragma once
#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Quest.h>
#include <GWCA/GameEntities/Title.h>
#include <GWCA/GameEntities/Player.h>
#include <GWCA/GameEntities/Hero.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>
#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Map.h>
#include <GWCA/GameEntities/Attribute.h>

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/GamePos.h>


namespace GW {

    typedef Array<NPC> NPCArray;
    typedef Array<Quest> QuestLog;
    typedef Array<Title> TitleArray;
    typedef Array<ItemID> MerchItemArray;
    typedef Array<Player> PlayerArray;
    typedef Array<HeroFlag> HeroFlagArray;
    typedef Array<HeroInfo> HeroInfoArray;
    typedef Array<MapAgent> MapAgentArray;
    typedef Array<Skillbar> SkillbarArray;
    typedef Array<AgentInfo> AgentInfoArray;
    typedef Array<AgentEffects> AgentEffectsArray;
    typedef Array<MissionMapIcon> MissionMapIconArray;
    typedef Array<PartyAttribute> PartyAttributeArray;

    struct ControlledMinions {
        AgentID agent_id;
        uint32_t minion_count;
    };
    struct DupeSkill {
        Constants::SkillID skill_id;
        uint32_t count;
    };
    struct ProfessionState {
        AgentID agent_id;
        Constants::Profession current_primary;
        Constants::Profession current_secondary;
        uint32_t unlocked_professions; // bitwise
        uint32_t unk;

        inline bool IsProfessionUnlocked(Constants::Profession profession) {
            return (unlocked_professions & (1 << (int)profession)) != 0;
        }
    };
    static_assert(sizeof(ProfessionState) == 0x14);

    struct WorldContext {

        static WorldContext* instance();

        struct sub1 {
            wchar_t* name;
            //...
        } *sub1; // 0x0

        /* +h0004 */ Array<wchar_t> message_buff;
        /* +h0014 */ uint32_t h0014[4];
        /* +h0024 */ MerchItemArray merch_items;
        /* +h0034 */ MerchItemArray merch_items2;
        /* +h0044 */ uint32_t accumMapInitUnk0;
        /* +h0048 */ uint32_t accumMapInitUnk1;
        /* +h004C */ uint32_t accumMapInitOffset;
        /* +h0050 */ uint32_t accumMapInitLength;
        /* +h0054 */ uint32_t h0054;
        /* +h0058 */ uint32_t accumMapInitUnk2;
        /* +h005C */ uint32_t h005C[8];
        /* +h007C */ MapAgentArray map_agents;
        /* +h008C */ uint32_t h008C[4];
        /* +h009C */ Vec3f all_flag;
        /* +h00A8 */ uint32_t h00A8;
        /* +h00AC */ PartyAttributeArray attributes;
        /* +h00BC */ uint32_t h00BC[255];
        /* +h04B8 */ Array<void *> h04B8;
        /* +h04C8 */ Array<void *> h04C8;
        /* +h04D8 */ uint32_t h04D8;
        /* +h04DC */ Array<void *> h04DC;
        /* +h04EC */ uint32_t h04EC[7];
        /* +h0508 */ AgentEffectsArray party_effects;
        /* +h0518 */ Array<void *> h0518;
        /* +h0528 */ uint32_t active_quest_id;
        /* +h052C */ QuestLog quest_log;
        /* +h053C */ uint32_t h053C[10];
        /* +h0564 */ Array<MissionObjective> mission_objectives;
        /* +h0574 */ Array<AgentID> henchmen_agent_ids;
        /* +h0584 */ HeroFlagArray hero_flags;
        /* +h0594 */ HeroInfoArray hero_info;
        /* +h05A4 */ Array<void *> cartographed_areas; // Struct size = 0x20
        /* +h05B4 */ uint32_t h05B4[2];
        /* +h05BC */ Array<ControlledMinions> controlled_minion_count;
        /* +h05CC */ Array<uint32_t> missions_bonus;
        /* +h05DC */ Array<uint32_t> missions_completed;
        /* +h05EC */ Array<uint32_t> missions_bonus_hm;
        /* +h05FC */ Array<uint32_t> missions_completed_hm;
        /* +h060C */ Array<uint32_t> unlocked_map;
        /* +h061C */ uint32_t h061C[4];
        /* +h062C */ Array<void *> h062C;
        /* +h063C */ uint32_t h063C[16];
        /* +h067C */ PlayerID player_number;
        /* +h0680 */ void* playerControlledChar; // Struct size = 0x134 ?
        /* +h0684 */ uint32_t is_hard_mode_unlocked;
        /* +h0688 */ uint32_t h0688[2];
        /* +h0690 */ uint32_t salvage_session_id;
        /* +h0694 */ uint32_t h0694[5];
        /* +h06A8 */ uint32_t playerTeamToken;
        /* +h06AC */ uint32_t h06AC[4];
        /* +h06BC */ Array<ProfessionState> party_profession_states; // Current state of primary/secondary/unlocked for current player and party heroes, used in skill window. aka attribStates
        /* +h06CC */ Array<void *> h06CC;
        /* +h06DC */ uint32_t h06DC;
        /* +h06E0 */ Array<void *> h06E0;
        /* +h06F0 */ SkillbarArray skillbar;
        /* +h0700 */ Array<void *> h0700; // related to +h0710, but always 0
        /* +h0710 */ Array<uint32_t> unlocked_character_skills; // bit field
        /* +h0720 */ Array<DupeSkill> duplicated_character_skills; // When res signet is bought more than once, its mapped into this array. Used in skill window.
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
        /* +h0790 */ uint32_t morale;
        /* +h0794 */ uint32_t morale_dupe;
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
        /* +h07C8 */ uint32_t equipment_status;
        /* +h07CC */ AgentInfoArray agent_infos;
        /* +h07DC */ Array<void *> h07DC;
        /* +h07EC */ MissionMapIconArray mission_map_icons;
        /* +h07FC */ NPCArray npcs;
        /* +h080C */ PlayerArray players;
        /* +h081C */ TitleArray titles;
        /* +h082C */ Array<TitleTier> title_tiers;
        /* +h083C */ Array<uint32_t> vanquished_areas;
        /* +h084C */ uint32_t foes_killed;
        /* +h0850 */ uint32_t foes_to_kill;
        //... couple more arrays after this


    };
    static_assert(sizeof(WorldContext) == 0x854); // Not the final size of WorldContext, but used to make sure offsets are correct.
}
