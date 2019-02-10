#include "stdafx.h"

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Constants/Constants.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Macros.h>

#include <GWCA/GameContainers/Vector.h>

#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Skill.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

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

typedef void(__fastcall *UseSkill_pt)(uint32_t, uint32_t, uint32_t, uint32_t);
static UseSkill_pt UseSkill_Func;

static uintptr_t skill_array_addr;

static void Init() {
    {
        uintptr_t address = GW::Scanner::Find(
            "\x8D\x04\xB6\x5E\xC1\xE0\x05\x05", "xxxxxxxx", 8);
        printf("[SCAN] SkillArray = %p\n", (void *)address);
        if (Verify(address))
            skill_array_addr = *(uintptr_t *)address;
    }

    UseSkill_Func = (UseSkill_pt)GW::Scanner::Find(
        "\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\xD9\x57\x8B\xF2\x89\x5D\xF0", "xxxxxxxxxxxxxxxx", 0);
}

namespace GW {

    Module SkillbarModule = {
        "SkillbarModule",   // name
        NULL,               // param
        ::Init,             // init_module
        NULL,               // exit_module
        NULL,               // exit_module
        NULL,               // remove_hooks
    };

    Skill SkillbarMgr::GetSkillConstantData(uint32_t skill_id) {
        Skill *skill_constants = (Skill *)skill_array_addr;
        return skill_constants[skill_id];
    }

    void SkillbarMgr::ChangeSecondary(uint32_t profession, int hero_index) {
        AgentID agent_id = Agents::GetHeroAgentID(hero_index);
        CtoS::SendPacket(0xC, CtoGS_MSGChangeSecondary, agent_id, profession);
    }

    void SkillbarMgr::LoadSkillbar(uint32_t *skill_ids, int hero_index) {
        AgentID agent_id = Agents::GetHeroAgentID(hero_index);
        CtoS::SendPacket(0x2C, CtoGS_MSGLoadSkillbar, agent_id, 0x8,
            skill_ids[0], skill_ids[1], skill_ids[2], skill_ids[3], skill_ids[4],
            skill_ids[5], skill_ids[6], skill_ids[7]);
    }

    static bool decode_skill_template(const char *temp, int *attrib_ids, int *attrib_vals,
        int *attrib_count, int *skill_ids, int *skill_count,
        Constants::Profession *primary, Constants::Profession *secondary)
    {
        const int SKILL_MAX = 3410; // @Cleanup: This should go somewhere else (it could be readed from the client)
        const int ATTRIBUTE_MAX = 44; // @Cleanup: This should go somewhere else (it could be readed from the client)

        int _skill_count = 0;
        int _attrib_count = 0;

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
        _attrib_count = _ReadBits(&it, 4);
        int bits_per_attr = _ReadBits(&it, 4) + 4;
        for (int i = 0; i < _attrib_count; i++) {
            attrib_ids[i] = _ReadBits(&it, bits_per_attr);
            attrib_vals[i] = _ReadBits(&it, 4);
            if (attrib_ids[i] > ATTRIBUTE_MAX) {
                fprintf_s(stderr, "Attribute id %d is out of range. (max = %d)\n", attrib_ids[i], 44);
                return false;
            }
        }

        // SKILLS
        int bits_per_skill = _ReadBits(&it, 4) + 8;
        for (_skill_count = 0; _skill_count < 8; _skill_count++) {
            skill_ids[_skill_count] = _ReadBits(&it, bits_per_skill);
            if (skill_ids[_skill_count] > SKILL_MAX) {
                fprintf_s(stderr, "Skill id %d is out of range. (max = %d)\n", skill_ids[_skill_count], SKILL_MAX);
                return false;
            }
            if (it + bits_per_skill > end) break; // Gw parse a template that doesn't specifie all empty skills.
        }

        *attrib_count = _attrib_count;
        *skill_count = _skill_count;
        *primary = static_cast<Constants::Profession>(prof1);
        *secondary = static_cast<Constants::Profession>(prof2);

        return true;
    }

    bool SkillbarMgr::LoadSkillTemplate(const char *temp) {
        using Constants::Profession;

        int skill_count = 0;
        int attrib_count = 0;
        int attrib_ids[10] = {0};
        int attrib_vals[10] = {0};
        int skill_ids[8] = {0};

        Profession primary = Profession::None;
        Profession secondary = Profession::None;

        if (!decode_skill_template(temp, attrib_ids, attrib_vals, &attrib_count, skill_ids, &skill_count, &primary, &secondary))
            return false;

        Agent *me = Agents::GetPlayer();
        if (!me) return false;

        // @Enhancement: Check if we already bought this secondary profession.
        if (me->secondary != (BYTE)secondary)
            PlayerMgr::ChangeSecondProfession(secondary);
        LoadSkillbar((uint32_t *)skill_ids);
        SetAttributes(attrib_count, (uint32_t *)attrib_ids, (uint32_t *)attrib_vals);
        return true;
    }

    bool SkillbarMgr::LoadSkillTemplate(const char *temp, int hero_index) {
        using Constants::Profession;

        if (hero_index == 0)
            return LoadSkillTemplate(temp);

        int skill_count = 0;
        int attrib_count = 0;
        int attrib_ids[10] = {0};
        int attrib_vals[10] = {0};
        int skill_ids[8] = {0};

        Profession primary = Profession::None;
        Profession secondary = Profession::None;

        if (!decode_skill_template(temp, attrib_ids, attrib_vals, &attrib_count, skill_ids, &skill_count, &primary, &secondary))
            return false;

        if (!PartyMgr::GetIsPartyLoaded())
            return false;

        PartyInfo *info = PartyMgr::GetPartyInfo();
        if (!info) return false;

        PlayerArray &players = Agents::GetPlayerArray();
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
        if (expected_primary != primary && expected_primary != Profession::None) {
            return false;
        }

        // @Enhancement: We may want to check if the hero already have the secondary prof needed.
        PlayerMgr::ChangeSecondProfession(secondary, hero_index);
        LoadSkillbar((uint32_t *)skill_ids, hero_index);
        SetAttributes(attrib_count, (uint32_t *)attrib_ids, (uint32_t *)attrib_vals, hero_index);
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
        Skillbar *bar = Skillbar::GetPlayerSkillbar();
        if (!bar || !bar->IsValid()) return -1;
        for (int i = 0; i < 8; ++i) {
            if (bar->skills[i].skill_id == id) {
                return i;
            }
        }
        return -1;
    }
} // namespace GW