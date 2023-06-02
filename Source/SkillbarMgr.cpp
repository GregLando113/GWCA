#include "stdafx.h"

#include <GWCA/Constants/Constants.h>
#include <GWCA/Constants/Skills.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/GameEntities/Party.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>
#include <GWCA/GameEntities/Attribute.h>
#include <GWCA/GameEntities/Hero.h>

#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PartyMgr.h>
#include <GWCA/Managers/SkillbarMgr.h>
#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace {
    using namespace GW;
    using namespace SkillbarMgr;

    Skill* skill_array_addr = 0;
    AttributeInfo* attribute_array_addr = nullptr;
    uint32_t attribute_array_count = 0x33;

    const char _Base64ToValue[128] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // [0,   16)
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // [16,  32)
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // [32,  48)
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // [48,  64)
        -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, // [64,  80)
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // [80,  96)
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // [96,  112)
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // [112, 128)
    };
    const unsigned char _Base64Table[65] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int _WriteBits(int val, char* buff, int count = 6) {
        for (int i = 0; i < count; i++) {
            buff[i] = ((val >> i) & 1);
        }
        return count;
    }

    int _ReadBits(char** str, int n) {
        int val = 0;
        char* s = *str;
        for (int i = 0; i < n; i++)
            val |= (*s++ << i);
        *str = s;
        return val;
    }

    Constants::Profession GetAttributeProfession(Constants::Attribute attribute, bool* is_primary_attribute = nullptr) {

        auto info = GetAttributeConstantData(attribute);
        if (!info)
            return Constants::Profession::None;
        if (is_primary_attribute) {
            switch (attribute) {
            case Constants::Attribute::FastCasting:
            case Constants::Attribute::SoulReaping:
            case Constants::Attribute::EnergyStorage:
            case Constants::Attribute::DivineFavor:
            case Constants::Attribute::Strength:
            case Constants::Attribute::Expertise:
            case Constants::Attribute::CriticalStrikes:
            case Constants::Attribute::SpawningPower:
            case Constants::Attribute::Leadership:
            case Constants::Attribute::Mysticism:
                *is_primary_attribute = true;
                break;
            default:
                *is_primary_attribute = false;
                break;
            }
        }

        return (Constants::Profession)info->profession_id;
    }

    typedef void(__cdecl* UseSkill_pt)(uint32_t, uint32_t, uint32_t, uint32_t);
    UseSkill_pt UseSkill_Func = 0;
    UseSkill_pt RetUseSkill = 0;
    //typedef HookCallback<uint32_t, uint32_t, uint32_t, uint32_t> UseSkillCallback;

    HookEntry OnLoadSkillbar_HookEntry;
    typedef void(__cdecl* LoadSkills_pt)(uint32_t agent_id, uint32_t skill_ids_count, uint32_t* skill_ids);
    LoadSkills_pt LoadSkills_Func = 0;
    LoadSkills_pt RetLoadSkills = 0;
    struct OnLoadSkillbar_UIMessage_Packet {
        uint32_t agent_id;
        uint32_t skill_ids[8] = { 0 };
    };
    void OnLoadSkillbar(uint32_t agent_id, uint32_t skill_ids_count, uint32_t* skill_ids) {
        GW::Hook::EnterHook();
        OnLoadSkillbar_UIMessage_Packet pack;
        pack.agent_id = agent_id;
        memcpy(pack.skill_ids, skill_ids, skill_ids_count * sizeof(*skill_ids));
        // Pass this through UI, we'll pick it up in OnLoadSkillbar_UIMessage. Ensure tmp_skill_ids is an array of at least 8 skill ids.
        UI::SendUIMessage(UI::UIMessage::kSendLoadSkillbar, &pack);
        GW::Hook::LeaveHook();
    };
    void OnLoadSkillbar_UIMessage(GW::HookStatus* status, UI::UIMessage message_id, void* wparam, void*) {
        GWCA_ASSERT(message_id == UI::UIMessage::kSendLoadSkillbar && wparam);
        if (!status->blocked && RetLoadSkills) {
            OnLoadSkillbar_UIMessage_Packet* pack = (OnLoadSkillbar_UIMessage_Packet*)wparam;
            RetLoadSkills(pack->agent_id, _countof(pack->skill_ids), pack->skill_ids);
        }
    }
    typedef void(__cdecl* LoadAttributes_pt)(uint32_t agent_id, uint32_t attribute_count, uint32_t* attribute_ids, uint32_t* attribute_values);
    LoadAttributes_pt LoadAttributes_Func = 0;

    typedef void(__cdecl* ChangeSecondary_pt)(uint32_t agent_id, uint32_t profession);
    ChangeSecondary_pt ChangeSecondary_Func = 0;

    std::unordered_map<HookEntry*, UseSkillCallback> OnUseSkill_Callbacks;
    void __cdecl OnUseSkill(uint32_t agent_id, uint32_t slot, uint32_t target, uint32_t call_target)
    {
        HookBase::EnterHook();
        HookStatus status;
        for (auto& it : OnUseSkill_Callbacks) {
            it.second(&status, agent_id, slot, target, call_target);
            ++status.altitude;
        }
        if (!status.blocked)
            RetUseSkill(agent_id, slot, target, call_target);
        HookBase::LeaveHook();
    }

    void Init() {

        DWORD address = 0;
        address = GW::Scanner::Find("\x8D\x04\xB6\xC1\xE0\x05\x05", "xxxxxxx", +7);
        if(Scanner::IsValidPtr(*(uintptr_t*)address,Scanner::RDATA))
            skill_array_addr = *(Skill**)address;

        address = GW::Scanner::Find("\xba\x33\x00\x00\x00\x89\x08\x8d\x40\x04", "x?xxxxxxxx", -4);
        if (Scanner::IsValidPtr(*(uintptr_t*)address, Scanner::RDATA)) {
            attribute_array_addr = *(AttributeInfo**)address;
        }

        UseSkill_Func = (UseSkill_pt)GW::Scanner::Find( "\x85\xF6\x74\x5B\x83\xFE\x11\x74", "xxxxxxxx", -0x126);

        address = Scanner::FindAssertion("p:\\code\\gw\\ui\\game\\templates\\templateshelpers.cpp", "targetPrimaryProf == templateData.profPrimary");
        ChangeSecondary_Func = (ChangeSecondary_pt)Scanner::FunctionFromNearCall(address + 0x20);
        LoadAttributes_Func = (LoadAttributes_pt)Scanner::FunctionFromNearCall(address + 0x34);
        LoadSkills_Func = (LoadSkills_pt)Scanner::FunctionFromNearCall(address + 0x40);

        if (LoadSkills_Func) {
            HookBase::CreateHook(LoadSkills_Func, OnLoadSkillbar, (void**)&RetLoadSkills);
            UI::RegisterUIMessageCallback(&OnLoadSkillbar_HookEntry, UI::UIMessage::kSendLoadSkillbar, OnLoadSkillbar_UIMessage, 0x1);
        }

        HookBase::CreateHook(UseSkill_Func, OnUseSkill, (void**)&RetUseSkill);

        GWCA_INFO("[SCAN] SkillArray = %p", skill_array_addr);
        GWCA_INFO("[SCAN] AttributeArray = %p", attribute_array_addr);
        GWCA_INFO("[SCAN] UseSkill_Func = %p", UseSkill_Func);
        GWCA_INFO("[SCAN] ChangeSecondary_Func = %p", ChangeSecondary_Func);
        GWCA_INFO("[SCAN] LoadAttributes_Func = %p", LoadAttributes_Func);
        GWCA_INFO("[SCAN] LoadSkills_Func = %p", LoadSkills_Func);
#ifdef _DEBUG
        GWCA_ASSERT(skill_array_addr);
        GWCA_ASSERT(attribute_array_addr);
        GWCA_ASSERT(UseSkill_Func);
        GWCA_ASSERT(ChangeSecondary_Func);
        GWCA_ASSERT(LoadAttributes_Func);
        GWCA_ASSERT(LoadSkills_Func);
#endif

    }

    void EnableHooks() {
        if(UseSkill_Func)
            HookBase::EnableHooks(UseSkill_Func);
        if(LoadSkills_Func)
            HookBase::EnableHooks(LoadSkills_Func);
    }

    void DisableHooks() {
        if (UseSkill_Func)
            HookBase::DisableHooks(UseSkill_Func);
        if (LoadSkills_Func)
            HookBase::DisableHooks(LoadSkills_Func);
    }

    void Exit() {
        HookBase::RemoveHook(UseSkill_Func);
        HookBase::RemoveHook(LoadSkills_Func);
    }

    // Gets current/available professions for a party member.
    const ProfessionState* GetAgentProfessionState(uint32_t agent_id) {
        const auto w = GetWorldContext();
        if (!w) return nullptr;
        for (const auto& it : w->party_profession_states) {
            if (it.agent_id == agent_id)
                return &it;
        }
        return nullptr;
    }

    Constants::Profession GetSkillProfession(Constants::SkillID skill_id) {
        auto data = GetSkillConstantData(skill_id);
        return data ? static_cast<GW::Constants::Profession>(data->profession) : GW::Constants::Profession::None;
    }
    bool IsPrimaryAttributeRequired(const SkillTemplate& skill_template, const GW::Constants::Profession profession) {
        if (profession == GW::Constants::Profession::None)
            return false;
        // If any of the attributes in this skill bar rely on the primary profession drop out
        bool is_primary_attribute = false;
        for (size_t i = 0; i < _countof(skill_template.attributes); i++) {
            if (GetAttributeProfession(skill_template.attributes[i].attribute, &is_primary_attribute) == profession
                && is_primary_attribute)
                return true;
        }
        return false;
    }
    bool IsProfessionRequired(const SkillTemplate& skill_template, const GW::Constants::Profession profession) {
        if (profession == GW::Constants::Profession::None)
            return false;
        // If any of the skills in this skill bar rely on the primary profession drop out
        for (size_t i = 0; i < _countof(skill_template.skills); i++) {
            if (GetSkillProfession(skill_template.skills[i]) == profession)
                return true;
        }
        // If any of the attributes in this skill bar rely on the primary profession drop out
        for (size_t i = 0; i < _countof(skill_template.attributes); i++) {
            if (GetAttributeProfession(skill_template.attributes[i].attribute) == profession)
                return true;
        }
        return false;
    }

}
namespace GW {

    Module SkillbarModule = {
        "SkillbarModule",   // name
        NULL,               // param
        ::Init,             // init_module
        ::Exit,               // exit_module
        ::EnableHooks,               // enable_hooks
        ::DisableHooks,               // disable_hooks
    };
    namespace SkillbarMgr {



        Skill* GetSkillConstantData(Constants::SkillID skill_id) {
            Skill* arr = (Skill*)skill_array_addr;
            return skill_array_addr ? &arr[(uint32_t)skill_id] : nullptr;
        }
        AttributeInfo* GetAttributeConstantData(Constants::Attribute attribute_id) {
            if (attribute_array_count <= static_cast<uint32_t>(attribute_id))
                return nullptr;
            return &attribute_array_addr[static_cast<uint32_t>(attribute_id)];
        }

        bool ChangeSecondProfession(Constants::Profession profession, uint32_t hero_index) {
            if (!ChangeSecondary_Func)
                return false;
            AgentID agent_id = Agents::GetHeroAgentID(hero_index);
            ChangeSecondary_Func(agent_id, (uint32_t)profession);
            return true;
        }

        bool LoadSkillbar(Constants::SkillID* skills, size_t n_skills, uint32_t hero_index) {
            if (!LoadSkills_Func)
                return false;
            const size_t bytes = n_skills * sizeof(uint32_t);
            uint32_t* skill_ids = (uint32_t*)malloc(bytes);
            if (skill_ids == 0)
                return false;
            memset(skill_ids, 0, bytes);
            memcpy(skill_ids, skills, bytes);
            AgentID agent_id = Agents::GetHeroAgentID(hero_index);
            // NB: GW Client sends UI Message 0x1000005e (update skillbar skills message) without waiting for a server response!
            // This is a bug in the client, but because this affects rendering we have to enqueue the call
            GameThread::Enqueue([agent_id, n_skills, skill_ids]() {
                LoadSkills_Func(agent_id, n_skills, skill_ids);
                free(skill_ids);
                });
            return true;
        }
        bool EncodeSkillTemplate(const SkillTemplate& in, char* build_code_result, size_t build_code_result_len)
        {
            const int bufSize = 1024;

            char bitStr[bufSize]; // @Cleanup: Confirm that the buffer is alway big enough.
            size_t offset = 0;

            offset += _WriteBits(14, &bitStr[offset], 4); // 4 Bits - Template Type - 14 (0xE) for Skill Template
            offset += _WriteBits(0, &bitStr[offset], 4); // 4 Bits - Version Number - 0

            // Professions
            int bits_per_prof = 4; // Professions go up to 10 - more than 4 bits would be 16 - how could this ever be more than 4 bits?
            offset += _WriteBits((int)((bits_per_prof - 4) * 0.5), &bitStr[offset], 2); // 2 Bits - A code controlling the number of encoded bits per profession id
            offset += _WriteBits((int)in.primary, &bitStr[offset], bits_per_prof);
            offset += _WriteBits((int)in.secondary, &bitStr[offset], bits_per_prof);

            // Attributes
            int bits_per_attr = 4;
            int attrib_count = 0;
            for (const Attribute& attribute : in.attributes) {
                if (attribute.attribute == Constants::Attribute::None || attribute.points == 0) {
                    continue;
                }
                int tmp = (int)log2((int)attribute.attribute) + 1;
                if (tmp > bits_per_attr) {
                    bits_per_attr = tmp;
                }
                attrib_count++;
            }
            offset += _WriteBits(attrib_count, &bitStr[offset], 4);
            offset += _WriteBits(bits_per_attr - 4, &bitStr[offset], 4);
            for (const Attribute& attribute : in.attributes) {
                if (attribute.attribute != GW::Constants::Attribute::None
                    && attribute.points != 0) {
                    offset += _WriteBits((int)attribute.attribute, &bitStr[offset], bits_per_attr);
                    offset += _WriteBits((int)attribute.points, &bitStr[offset], 4);
                }
            }

            // Skills
            int bits_per_skill = 8;
            for (const GW::Constants::SkillID skill : in.skills) {
                int tmp = (int)log2((int)skill) + 1;
                if (tmp > bits_per_skill) {
                    bits_per_skill = tmp;
                }
            }
            offset += _WriteBits(bits_per_skill - 8, &bitStr[offset], 4);
            for (const GW::Constants::SkillID skill : in.skills) {
                offset += _WriteBits((int)skill, &bitStr[offset], bits_per_skill);
            }

            char* it = bitStr;
            size_t r = offset % 6;
            for (size_t i = 0; i < r; i++) {
                bitStr[offset++] = 0;
            }
            size_t needed_length = offset / 6;
            if (needed_length > build_code_result_len - 1) {
                GWCA_ERR("Result length %d less than required build code length %d\n", needed_length, build_code_result_len);
                return false;
            }
            for (size_t i = 0; i < needed_length; i++) {
                int value = _ReadBits(&it, 6);
                build_code_result[i] = (char)_Base64Table[value];
            }
            build_code_result[needed_length] = 0;
            return true;
        }
        bool DecodeSkillTemplate(SkillTemplate* result, const char* temp)
        {
            const int SKILL_MAX = 3431; // @Cleanup: This should go somewhere else (it could be readed from the client)

            int skill_count = 0;
            int attrib_count = 0;

            size_t len = strlen(temp);
            // char *bitStr = new char[len * 6]; // @Enhancement: this doesn't need to be a heap alloc.

            const int bufSize = 1024;
            GWCA_ASSERT((len * 6) < bufSize);
            char bitStr[bufSize]; // @Cleanup: Confirm that the buffer is alway big enough.

            size_t bitStrLen = 0;
            for (size_t i = 0; i < len; i++) {
                int numeric_value = _Base64ToValue[temp[i]];
                if (numeric_value == -1) {
                    GWCA_ERR("Unvalid base64 character '%c' in string '%s'\n", temp[i], temp);
                    return false;
                }
                bitStrLen += _WriteBits(numeric_value, &bitStr[bitStrLen], 6);
            }

            char* it = bitStr;
            char* end = bitStr + 6 * len;

            // HEADER
            int header = _ReadBits(&it, 4);
            if (header != 0 && header != 14) {
                GWCA_ERR("Template header '%d' not valid.", header);
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
            bool is_primary_attribute = false;
            for (int i = 0; i < attrib_count; i++) {
                uint32_t attrib_id = (uint32_t)_ReadBits(&it, bits_per_attr);
                int attrib_val = _ReadBits(&it, 4);
                if (attrib_id >= attribute_array_count) {
                    GWCA_ERR("Attribute id %d is out of range. (count = %d)\n", attrib_id, attribute_array_count);
                    return false;
                }
                if (attrib_val > 12) {
                    GWCA_ERR("Attribute id %d has a value of %d. (max = 12)\n", attrib_id, attrib_val);
                    return false;
                }
                if (!attrib_val) {
                    continue;
                }
                int prof = (int)GetAttributeProfession((Constants::Attribute)attrib_id, &is_primary_attribute);
                if (prof != prof1 && prof != prof2) {
                    GWCA_ERR("Attribute id %d does not match build profession(s)\n", attrib_id);
                    return false;
                }
                if (is_primary_attribute && prof != prof1) {
                    GWCA_ERR("Primary attribute id %d does not match primary profession\n", attrib_id);
                    return false;
                }
                result->attributes[i].attribute = static_cast<Constants::Attribute>(attrib_id);
                result->attributes[i].points = static_cast<uint32_t>(attrib_val);
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
                    GWCA_ERR("Skill id %d is out of range. (max = %d)\n", skill_id, SKILL_MAX);
                    return false;
                }
                Skill* s = GetSkillConstantData((GW::Constants::SkillID)skill_id);
                if (!s) {
                    GWCA_ERR("Failed to get info for skill ID %d\n", skill_id);
                    return false;
                }
                if (s->profession != 0 && s->profession != (uint8_t)prof1 && s->profession != (uint8_t)prof2) {
                    GWCA_ERR("Skill id %d doesn't match build profession(s)\n", skill_id);
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

        bool LoadSkillTemplate(const char* temp) {
            using Constants::Profession;

            if (Map::GetInstanceType() != Constants::InstanceType::Outpost)
                return false;

            SkillTemplate skill_template;
            if (!DecodeSkillTemplate(&skill_template, temp)) {
                return false;
            }

            AgentLiving* me = Agents::GetPlayerAsAgentLiving();
            if (!me) return false;

            const auto profession_state = GetAgentProfessionState(me->agent_id);
            if (!profession_state) {
                return false;
            }
            if (skill_template.primary != Constants::Profession::None && profession_state->primary != skill_template.primary) {
                if (IsPrimaryAttributeRequired(skill_template, skill_template.primary)) {
                    // Build contains points in this profession's primary attribute; we can't avoid it.
                    return false;
                }
                if (IsProfessionRequired(skill_template, skill_template.primary)) {
                    // Primary is required e.g. skills or attributes, but not the primary attribute - it could be loaded as a secondary
                    if (IsProfessionRequired(skill_template, skill_template.secondary)) {
                        // Secondary profession is also required, can't use it.
                        return false; 
                    }
                    // Swap primary with secondary (we'll check whether its unlocked later)
                    skill_template.secondary = skill_template.primary;
                }
            }
            if (skill_template.secondary != Constants::Profession::None && IsProfessionRequired(skill_template, skill_template.secondary)) {
                if (!profession_state->IsProfessionUnlocked(skill_template.secondary)) {
                    return false;
                }
                if (profession_state->secondary != skill_template.secondary) {
                    ChangeSecondProfession(skill_template.secondary);
                }
            }

            LoadSkillbar(skill_template.skills, _countof(skill_template.skills));
            SetAttributes(skill_template.attributes, _countof(skill_template.attributes));
            return true;
        }

        bool LoadSkillTemplate(const char* temp, uint32_t hero_index) {
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

            PartyInfo* info = PartyMgr::GetPartyInfo();
            if (!info) return false;

            PlayerArray* players = Agents::GetPlayerArray();
            if (!players) return false;

            AgentLiving* me = Agents::GetPlayerAsAgentLiving();
            if (!me) return false;
            HeroPartyMemberArray& heroes = info->heroes;

            if (hero_index <= 0 || (int)heroes.size() < hero_index)
                return false;

            HeroPartyMember& hero = heroes[hero_index - 1];
            if (hero.owner_player_id != me->login_number)
                return false;

            const auto profession_state = GetAgentProfessionState(hero.agent_id);
            if (!profession_state) {
                return false; // Hero not unlocked??
            }
            if (skill_template.primary != Constants::Profession::None && profession_state->primary != skill_template.primary) {
                if (IsPrimaryAttributeRequired(skill_template, skill_template.primary)) {
                    // Build contains points in this profession's primary attribute; we can't avoid it.
                    return false;
                }
                if (IsProfessionRequired(skill_template, skill_template.primary)) {
                    // Primary is required e.g. skills or attributes, but not the primary attribute - it could be loaded as a secondary
                    if (IsProfessionRequired(skill_template, skill_template.secondary)) {
                        // Secondary profession is also required, can't use it.
                        return false; 
                    }
                    // Swap primary with secondary (we'll check whether its unlocked later)
                    skill_template.secondary = skill_template.primary;
                }
            }
            if (skill_template.secondary != Constants::Profession::None && profession_state->secondary != skill_template.secondary) {
                // NB: Heroes have all secondary professions unlocked?
                ChangeSecondProfession(skill_template.secondary, hero_index);
            }

            // @Robustness: That cast is not very good :(
            LoadSkillbar(skill_template.skills, _countof(skill_template.skills), hero_index);
            SetAttributes(skill_template.attributes, _countof(skill_template.attributes), hero_index);
            return true;
        }

        bool SetAttributes(uint32_t attribute_count,
            uint32_t* attribute_ids, uint32_t* attribute_values, uint32_t hero_index) {
            if (!LoadAttributes_Func)
                return false;
            AgentID agent_id = Agents::GetHeroAgentID(hero_index);
            LoadAttributes_Func(agent_id, attribute_count, attribute_ids, attribute_values);
            return true;
        }

        bool SetAttributes(Attribute* attributes, size_t n_attributes, uint32_t hero_index) {
            uint32_t count;
            uint32_t attribute_ids[16];
            uint32_t attribute_values[16];

            for (count = 0; count < n_attributes; count++) {
                if (attributes[count].attribute == Constants::Attribute::None)
                    break;
                attribute_ids[count] = static_cast<uint32_t>(attributes[count].attribute);
                attribute_values[count] = static_cast<uint32_t>(attributes[count].points);
            }

            return SetAttributes(count, attribute_ids, attribute_values, hero_index);
        }

        bool UseSkill(uint32_t slot, uint32_t target, uint32_t call_target) {
            if (!UseSkill_Func)
                return false;
            UseSkill_Func(Agents::GetPlayerId(), slot, target, call_target);
            return true;
        }

        bool UseSkillByID(uint32_t skill_id, uint32_t target, uint32_t call_target) {
            int slot = GetSkillSlot((Constants::SkillID)skill_id);
            if (slot == -1)
                return false;
            return UseSkill((uint32_t)slot, target, call_target);
        }

        int GetSkillSlot(Constants::SkillID skill_id) {
            Skillbar* bar = GetPlayerSkillbar();
            if (!bar || !bar->IsValid()) return -1;
            for (int i = 0; i < 8; ++i) {
                if (bar->skills[i].skill_id == skill_id) {
                    return i;
                }
            }
            return -1;
        }

        SkillbarArray* GetSkillbarArray() {
            auto* w = GetWorldContext();
            return w && w->skillbar.valid() ? &w->skillbar : nullptr;
        }

        Skillbar* GetPlayerSkillbar() {
            SkillbarArray* sba = GetSkillbarArray();
            uint32_t player_id = sba ? GW::Agents::GetPlayerId() : 0;
            if (!player_id) return nullptr;
            for (auto& sb : *sba) {
                if (sb.agent_id == player_id)
                    return &sb;
            }
            return nullptr;
        }
        Skill* GetHoveredSkill() {
            UI::TooltipInfo* tooltip = UI::GetCurrentTooltip();
            if (!(tooltip && tooltip->type() == UI::TooltipType::Skill))
                return nullptr;
            return GetSkillConstantData(*(GW::Constants::SkillID*)tooltip->payload);
        }
        bool GetIsSkillUnlocked(GW::Constants::SkillID skill_id) {
            GW::GameContext* g = GW::GetGameContext();
            GW::WorldContext* w = g->world;

            auto& array = w->unlocked_character_skills;

            uint32_t index = static_cast<uint32_t>(skill_id);
            uint32_t real_index = index / 32;
            if (real_index >= array.size())
                return false;
            uint32_t shift = index % 32;
            uint32_t flag = 1U << shift;
            return (array[real_index] & flag) != 0;
        }
        void RegisterUseSkillCallback(
            HookEntry* entry,
            const UseSkillCallback& callback)
        {
            OnUseSkill_Callbacks.insert({ entry, callback });
        }

        void RemoveUseSkillCallback(
            HookEntry* entry)
        {
            auto it = OnUseSkill_Callbacks.find(entry);
            if (it != OnUseSkill_Callbacks.end())
                OnUseSkill_Callbacks.erase(it);
        }
    }
} // namespace GW
