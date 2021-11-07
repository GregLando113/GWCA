#pragma once

namespace GW {

    struct LoginCharacter {
        uint32_t unk0; // Some kind of function call
        wchar_t character_name[20];
    };
    struct PreGameContext {

        static PreGameContext* instance();

        /* +h0000 */ uint8_t h0000[0x13C];
        /* +h0140 */ uint32_t chosen_character_index;
        /* +h0144 */ uint32_t h0144;
        /* +h0148 */ GW::Array<LoginCharacter> chars;
    };
}
