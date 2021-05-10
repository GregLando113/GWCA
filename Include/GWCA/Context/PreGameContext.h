#pragma once

namespace GW {

    struct LoginCharacter {
        uint32_t unk0; // Some kind of function call
        wchar_t character_name[20];
    };
    struct PreGameContext {

        static PreGameContext* instance();

        /* +h0000 */ uint8_t h0000[0x148];
        /* +h0148 */ GW::Array<LoginCharacter> chars;
    };
}
