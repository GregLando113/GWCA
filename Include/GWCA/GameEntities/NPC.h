#pragma once

#include <GWCA/GameContainers/Array.h>

namespace GW {
    namespace Constants {
        enum class Profession;
    }
    struct NPC { // total: 0x30/48
        /* +h0000 */ uint32_t model_file_id;
        /* +h0004 */ uint32_t h0004;
        /* +h0008 */ uint32_t scale; // I think, 2 highest order bytes are percent of size, so 0x64000000 is 100%
        /* +h000C */ uint32_t sex;
        /* +h0010 */ uint32_t npc_flags;
        /* +h0014 */ Constants::Profession primary;
        /* +h0018 */ uint32_t h0018;
        /* +h001C */ uint8_t  default_level;
        // +h001D    uint8_t  padding;
        // +h001E    uint16_t padding;
        /* +h0020 */ wchar_t *name_enc;
        /* +h0024 */ uint32_t *model_files;
        /* +h0028 */ uint32_t files_count; // length of ModelFile
        /* +h002C */ uint32_t files_capacity; // capacity of ModelFile
    };
    static_assert(sizeof(NPC) == 0x30, "struct NPC has incorect size");

    typedef Array<NPC> NPCArray;
}
