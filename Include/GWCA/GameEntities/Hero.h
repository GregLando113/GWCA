#pragma once
#include <GWCA/Constants/Types.h>

#include <GWCA/GameContainers/Array.h>

namespace GW {
    namespace Constants {
        enum class HeroID;
        enum class Profession;
    }
    struct HeroFlag { // total: 0x20/36
        /* +h0000 */ Constants::HeroID hero_id;
        /* +h0004 */ AgentID  agent_id;
        /* +h0008 */ uint32_t level;
        /* +h000C */ uint32_t hero_behavior;
        /* +h0010 */ Vec2f flag;
        /* +h0018 */ uint32_t h0018;
        /* +h001C */ AgentID locked_target_id;
        /* +h0020 */ uint32_t h0020; // type is unknown too, added for padding
    };
    static_assert(sizeof(HeroFlag) == 0x24, "struct HeroFlag has incorect size");

    struct HeroInfo { // total: 0x78/120
        /* +h0000 */ Constants::HeroID hero_id;
        /* +h0004 */ AgentID agent_id;
        /* +h0008 */ uint32_t level;
        /* +h000C */ Constants::Profession primary; // Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h0010 */ Constants::Profession secondary; // Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
        /* +h0014 */ uint32_t hero_file_id;
        /* +h0018 */ uint32_t model_file_id;
        /* +h001C */ uint8_t  h001C[52];
        /* +h0050 */ wchar_t  name[20];
    };
    static_assert(sizeof(HeroInfo) == 120, "struct HeroInfo has incorect size");
}
