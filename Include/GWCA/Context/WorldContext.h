#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/GamePos.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct WorldContext;
    GWCA_API WorldContext* GetWorldContext();

    namespace Constants {
        enum class Profession;
    }

    typedef uint32_t ItemID;
    enum class HeroBehavior : uint32_t;


    struct NPC;
    struct Quest;
    struct Title;
    struct Player;
    struct HeroInfo;
    struct HeroFlag;
    struct MapAgent;
    struct Skillbar;
    struct AgentInfo;
    struct AgentEffects;
    struct PartyAttribute;
    struct MissionMapIcon;
    struct MissionObjective;
    struct TitleTier;

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

    struct PartyAlly {
        uint32_t agent_id;
        uint32_t unk;
        uint32_t composite_id;
    };
    static_assert(sizeof(PartyAlly) == 0xc);

    namespace Constants {
        enum class QuestID : uint32_t;
    }

    struct ControlledMinions {
        uint32_t agent_id;
        uint32_t minion_count;
    };
    struct DupeSkill {
        uint32_t skill_id;
        uint32_t count;
    };
    struct ProfessionState {
        uint32_t agent_id;
        GW::Constants::Profession primary;
        GW::Constants::Profession secondary;
        uint32_t unlocked_professions; // bitwise
        uint32_t unk;

        inline bool IsProfessionUnlocked(GW::Constants::Profession profession) const {
            return (unlocked_professions & (1 << (uint32_t)profession)) != 0;
        }
    };
    static_assert(sizeof(ProfessionState) == 0x14);

    struct AccountInfo {
        wchar_t* account_name;
        uint32_t wins;
        uint32_t losses;
        uint32_t rating;
        uint32_t qualifier_points;
        uint32_t rank;
        uint32_t tournament_reward_points;
    };
    static_assert(sizeof(AccountInfo) == 0x1c);

    struct PartyMemberMoraleInfo {
        uint32_t agent_id;
        uint32_t agent_id_dup;
        uint32_t unk[4];
        uint32_t morale;
        // ... unknown size
    };

    struct PartyMoraleLink {
        uint32_t unk;
        uint32_t unk2;
        PartyMemberMoraleInfo* party_member_info;
    };
    static_assert(sizeof(PartyMoraleLink) == 0xc);

    struct PetInfo {
        uint32_t agent_id;
        uint32_t owner_agent_id;
        wchar_t* pet_name;
        uint32_t model_file_id1;
        uint32_t model_file_id2;
        HeroBehavior behavior;
        uint32_t locked_target_id;

    };
    static_assert(sizeof(PetInfo) == 0x1c);



    struct WorldContext {

        static WorldContext* instance();

        /* +h0000 */ AccountInfo* accountInfo;
        /* +h0004 */ Array<wchar_t> message_buff;
        /* +h0014 */ Array<wchar_t> dialog_buff;
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
        /* +h008C */ Array<PartyAlly> party_allies; // List of allies added to the current party
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
        /* +h0528 */ GW::Constants::QuestID active_quest_id;
        /* +h052C */ QuestLog quest_log;
        /* +h053C */ uint32_t h053C[10];
        /* +h0564 */ Array<MissionObjective> mission_objectives;
        /* +h0574 */ Array<uint32_t> henchmen_agent_ids;
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
        /* +h061C */ uint32_t h061C[2];
        /* +h0624 */ PartyMemberMoraleInfo* player_morale_info;
        /* +h0628 */ uint32_t h028C;
        /* +h062C */ Array<PartyMoraleLink> party_morale_related;
        /* +h063C */ uint32_t h063C[16];
        /* +h067C */ uint32_t player_number;
        /* +h0680 */ void* playerControlledChar; // Struct size = 0x134 ?
        /* +h0684 */ uint32_t is_hard_mode_unlocked;
        /* +h0688 */ uint32_t h0688[2];
        /* +h0690 */ uint32_t salvage_session_id;
        /* +h0694 */ uint32_t h0694[5];
        /* +h06A8 */ uint32_t playerTeamToken;
        /* +h06AC */ Array<PetInfo> pets;
        /* +h06BC */ Array<ProfessionState> party_profession_states; // Current state of primary/secondary/unlocked for current player and party heroes, used in skill window. aka attribStates
        /* +h06CC */ Array<void *> h06CC;
        /* +h06DC */ uint32_t h06DC;
        /* +h06E0 */ Array<void *> h06E0;
        /* +h06F0 */ SkillbarArray skillbar;
        /* +h0700 */ Array<uint32_t> learnable_character_skills; // populated at skill trainer and when using signet of capture
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
