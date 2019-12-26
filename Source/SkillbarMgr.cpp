#include "stdafx.h"

#include <GWCA/Packets/CtoSHeaders.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameContainers/GamePos.h>

#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PartyMgr.h>
#include <GWCA/Managers/PlayerMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/SkillbarMgr.h>

static const char _Base64ToValue[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // [0,   16)
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // [16,  32)
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // [32,  48)
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // [48,  64)
    -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, // [64,  80)
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // [80,  96)
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // [96,  112)
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // [112, 128)
};

static void _WriteBits(int val, char *buff) {
    buff[0] = ((val >> 0) & 1);
    buff[1] = ((val >> 1) & 1);
    buff[2] = ((val >> 2) & 1);
    buff[3] = ((val >> 3) & 1);
    buff[4] = ((val >> 4) & 1);
    buff[5] = ((val >> 5) & 1);
}

static int _ReadBits(char **str, int n) {
    int val = 0;
    char *s = *str;
    for (int i = 0; i < n; i++)
        val |= (*s++ << i);
    *str = s;
    return val;
}

typedef void(__cdecl *UseSkill_pt)(uint32_t, uint32_t, uint32_t, uint32_t);
static UseSkill_pt UseSkill_Func;

static uintptr_t skill_array_addr;

static void Init() {
    {
        uintptr_t address = GW::Scanner::Find(
            "\x8D\x04\xB6\xC1\xE0\x05\x05", "xxxxxxx", +7);
        printf("[SCAN] SkillArray = %p\n", (void *)address);
        if (Verify(address))
            skill_array_addr = *(uintptr_t *)address;
    }

    UseSkill_Func = (UseSkill_pt)GW::Scanner::Find(
        "\x85\xF6\x74\x5B\x83\xFE\x11\x74", "xxxxxxxx", -0x126);
}

namespace GW {

    Module SkillbarModule = {
        "SkillbarModule",   // name
        NULL,               // param
        ::Init,             // init_module
        NULL,               // exit_module
        NULL,               // enable_hooks
        NULL,               // disable_hooks
    };

    Skill& SkillbarMgr::GetSkillConstantData(uint32_t skill_id) {
        Skill *skill_constants = (Skill *)skill_array_addr;
        return skill_constants[skill_id];
    }

    void SkillbarMgr::ChangeSecondary(uint32_t profession, int hero_index) {
        AgentID agent_id = Agents::GetHeroAgentID(hero_index);
        CtoS::SendPacket(0xC, CtoGS_MSGChangeSecondary, agent_id, profession);
    }

    void SkillbarMgr::LoadSkillbar(uint32_t *skills, size_t n_skills, int hero_index) {
        uint32_t skill_ids[8] = {0};
        memcpy(skill_ids, skills, n_skills * sizeof(uint32_t));
        AgentID agent_id = Agents::GetHeroAgentID(hero_index);
        CtoS::SendPacket(0x2C, CtoGS_MSGLoadSkillbar, agent_id, 0x8,
            skill_ids[0], skill_ids[1], skill_ids[2], skill_ids[3], skill_ids[4],
            skill_ids[5], skill_ids[6], skill_ids[7]);
    }

    void SkillbarMgr::LoadSkillbar(Constants::SkillID *skills, size_t n_skills, int hero_index) {
        uint32_t skill_ids[8];
        assert(n_skills <= _countof(skill_ids));
        for (size_t i = 0; i < n_skills; i++)
            skill_ids[i] = static_cast<uint32_t>(skills[i]);
        return LoadSkillbar(skill_ids, n_skills, hero_index);
    }

    bool SkillbarMgr::DecodeSkillTemplate(SkillTemplate *result, const char *temp)
    {
        const int SKILL_MAX = 3410; // @Cleanup: This should go somewhere else (it could be readed from the client)
        const int ATTRIBUTE_MAX = 44; // @Cleanup: This should go somewhere else (it could be readed from the client)

        int skill_count = 0;
        int attrib_count = 0;

        size_t len = strlen(temp);
        // char *bitStr = new char[len * 6]; // @Enhancement: this doesn't need to be a heap alloc.

        const int bufSize = 1024;
        assert((len * 6) < bufSize);
        char bitStr[bufSize]; // @Cleanup: Confirm that the buffer is alway big enough.

        for (size_t i = 0; i < len; i++) {
            int numeric_value = _Base64ToValue[temp[i]];
            if (numeric_value == -1) {
                fprintf_s(stderr, "Unvalid base64 character '%c' in string '%s'\n", temp[i], temp);
                return false;
            }
            _WriteBits(numeric_value, bitStr + (6 * i));
        }

        char *it = bitStr;
        char *end = bitStr + 6 * len;

        // HEADER
        int header = _ReadBits(&it, 4);
        if (header != 0 && header != 14) {
            fprintf_s(stderr, "Template header '%d' not valid.", header);
            return false;
        }
        if (header == 14) _ReadBits(&it, 4);
        int bits_per_prof = 2 * _ReadBits(&it, 2) + 4;
        int prof1 = _ReadBits(&it, bits_per_prof);
        int prof2 = _ReadBits(&it, bits_per_prof);
        if (prof1 <= 0 || prof2 < 0 || prof1 > 10 || prof2 > 10) return false;

        // ATTRIBUTES
        attrib_count = _ReadBits(&it, 4);
        if (attrib_count >= _countof(result->attributes)) {
            fprintf(stderr, "Found too many attributes %d in the template '%s'\n", attrib_count, temp);
            return false;
        }

        int bits_per_attr = _ReadBits(&it, 4) + 4;
        for (int i = 0; i < attrib_count; i++) {
            int attrib_id = _ReadBits(&it, bits_per_attr);
            int attrib_val = _ReadBits(&it, 4);
            if (attrib_id > ATTRIBUTE_MAX) {
                fprintf_s(stderr, "Attribute id %d is out of range. (max = %d)\n", attrib_id, ATTRIBUTE_MAX);
                return false;
            }
            result->attributes[i].attribute = static_cast<Constants::Attribute>(attrib_id);
            result->attributes[i].points = attrib_val;
        }
        for (int i = attrib_count; i < _countof(result->attributes); i++) {
            result->attributes[i].attribute = Constants::Attribute::None;
            result->attributes[i].points = 0;
        }

        // SKILLS
        int bits_per_skill = _ReadBits(&it, 4) + 8;
        for (skill_count = 0; skill_count < _countof(result->skills); skill_count++) {
            if (it + bits_per_skill > end) break; // Gw parse a template that doesn't specifie all empty skills.
            int skill_id = _ReadBits(&it, bits_per_skill);
            if (skill_id > SKILL_MAX) {
                fprintf_s(stderr, "Skill id %d is out of range. (max = %d)\n", skill_id, SKILL_MAX);
                return false;
            }
            result->skills[skill_count] = static_cast<Constants::SkillID>(skill_id);
        }
        for (int i = skill_count; i < _countof(result->skills); i++) {
            result->skills[i] = Constants::SkillID::No_Skill;
        }

        result->primary = static_cast<Constants::Profession>(prof1);
        result->secondary = static_cast<Constants::Profession>(prof2);
        return true;
    }

    bool SkillbarMgr::LoadSkillTemplate(const char *temp) {
        using Constants::Profession;

        if (Map::GetInstanceType() != Constants::InstanceType::Outpost)
            return false;

        SkillTemplate skill_template;
        if (!DecodeSkillTemplate(&skill_template, temp)) {
            return false;
        }

        Agent *me = Agents::GetPlayer();
        if (!me) return false;

        if (me->primary != (BYTE)skill_template.primary)
            return false;
        // @Enhancement: Check if we already bought this secondary profession.
        if (me->secondary != (BYTE)skill_template.secondary)
            PlayerMgr::ChangeSecondProfession(skill_template.secondary);
        // @Robustness: That cast is not very good :(
        LoadSkillbar(skill_template.skills, _countof(skill_template.skills));
        SetAttributes(skill_template.attributes, _countof(skill_template.attributes));
        return true;
    }

    bool SkillbarMgr::LoadSkillTemplate(const char *temp, int hero_index) {
        using Constants::Profession;

        if (Map::GetInstanceType() != Constants::InstanceType::Outpost)
            return false;

        if (hero_index == 0)
            return LoadSkillTemplate(temp);

        SkillTemplate skill_template;
        if (!DecodeSkillTemplate(&skill_template, temp)) {
            return false;
        }

        if (!PartyMgr::GetIsPartyLoaded())
            return false;

        PartyInfo *info = PartyMgr::GetPartyInfo();
        if (!info) return false;

        PlayerArray players = Agents::GetPlayerArray();
        if (!players.valid()) return false;

        Agent *me = Agents::GetPlayer();
        if (!me) return false;
        HeroPartyMemberArray &heroes = info->heroes;

        if (hero_index <= 0 || (int)heroes.size() < hero_index)
            return false;

        HeroPartyMember &hero = heroes[hero_index-1];
        if (hero.owner_player_id != me->login_number)
            return false;

        // @Enhancement: There should be a systematic way to get the profession of an hero.
        Constants::Profession expected_primary = Constants::HeroProfs[hero.hero_id];

        // Hacky, because we can't check for mercenary heroes and Razah
        if (expected_primary != skill_template.primary && expected_primary != Profession::None) {
            return false;
        }

        // @Enhancement: We may want to check if the hero already have the secondary prof needed.
        PlayerMgr::ChangeSecondProfession(skill_template.secondary, hero_index);
        // @Robustness: That cast is not very good :(
        LoadSkillbar(skill_template.skills, _countof(skill_template.skills), hero_index);
        SetAttributes(skill_template.attributes, _countof(skill_template.attributes), hero_index);
        return true;
    }

    void SkillbarMgr::SetAttributes(uint32_t attribute_count,
        uint32_t *attribute_ids, uint32_t *attribute_values, int hero_index) {

        struct tSetAttributes {
            uint32_t header = CtoGS_MSGSetAttributes;
            AgentID  agent_id;
            uint32_t attribute_count1;
            uint32_t attribute_ids[16];
            uint32_t attribute_count2;
            uint32_t attribute_values[16];
        };

        tSetAttributes set_attributes_buffer;

        AgentID agent_id = Agents::GetHeroAgentID(hero_index);

        set_attributes_buffer.agent_id = agent_id;
        set_attributes_buffer.attribute_count1 = attribute_count;
        set_attributes_buffer.attribute_count2 = attribute_count;

        for (uint32_t i = 0; i < attribute_count; ++i) {
            set_attributes_buffer.attribute_ids[i] = attribute_ids[i];
            set_attributes_buffer.attribute_values[i] = attribute_values[i];
        }

        CtoS::SendPacket<tSetAttributes>(&set_attributes_buffer);
    }

    void SkillbarMgr::SetAttributes(Attribute *attributes, size_t n_attributes, int hero_index) {
        uint32_t count;
        uint32_t attribute_ids[16];
        uint32_t attribute_values[16];

        for (count = 0; count < _countof(attribute_ids); count++) {
            if (attributes[count].attribute == Constants::Attribute::None)
                break;
            attribute_ids[count] = static_cast<uint32_t>(attributes[count].attribute);
            attribute_values[count] = static_cast<uint32_t>(attributes[count].points);
        }

        return SetAttributes(count, attribute_ids, attribute_values, hero_index);
    }

    void SkillbarMgr::UseSkill(uint32_t slot, uint32_t target, uint32_t call_target) {
        if (Verify(UseSkill_Func)) {
            UseSkill_Func(Agents::GetPlayerId(), slot, target, call_target);
        }
    }

    void SkillbarMgr::UseSkillByID(uint32_t skill_id, uint32_t target, uint32_t call_target) {
        CtoS::SendPacket(0x14, CtoGS_MSGUseSkill, skill_id, 0, target, call_target);
    }

    int SkillbarMgr::GetSkillSlot(Constants::SkillID skill_id) {
        uint32_t id = static_cast<uint32_t>(skill_id);
        Skillbar *bar = SkillbarMgr::GetPlayerSkillbar();
        if (!bar || !bar->IsValid()) return -1;
        for (int i = 0; i < 8; ++i) {
            if (bar->skills[i].skill_id == id) {
                return i;
            }
        }
        return -1;
    }

    SkillbarArray SkillbarMgr::GetSkillbarArray() {
        return GameContext::instance()->world->skillbar;
    }

    Skillbar *SkillbarMgr::GetPlayerSkillbar() {
        SkillbarArray sb = SkillbarMgr::GetSkillbarArray();
        if (sb.valid()) {
            return &sb[0];
        } else {
            return NULL;
        }
    }
} // namespace GW