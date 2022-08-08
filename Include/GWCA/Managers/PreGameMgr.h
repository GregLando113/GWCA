#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Export.h>

namespace GW
{
    struct CharacterSelectionContext
    {
        /* +h0000 */ uintptr_t v_table_addr;
        /* +h0004 */ uint32_t frame_index;
        /* +h0008 */ uint32_t number_of_characters;
        /* +h000C */ uint32_t unk1_hexFFFFFFFF;
        /* +h0010 */ uint32_t unk2_hex00000000;
        /* +h0014 */ uint32_t unk3_hex00000000;
        /* +h0018 */ uint32_t unk4_hex00000001;
        /* +h001C */ wchar_t selected_character_name[20];
    };

    struct Frame
    {
        /* +h0000 */ uint32_t h_0000[45];
        /* +h00B4 */ uint32_t id;
        // ...
    };

    struct Character // total: 0x84/132
    {
        /* +h0000 */ uint32_t h_0000;
        /* +h0004 */ uint32_t h_0004;
        /* +h0008 */ uint32_t h_0008;
        /* +h000C */ uint32_t h_000C;
        /* +h0010 */ uint32_t h_0010;
        /* +h0014 */ uint32_t h_0014;
        /* +h0018 */ wchar_t name[20];
        // ...
    };

    typedef Array<Frame*> FrameArray;
    typedef Array<Character> CharacterArray;

    struct Module;
    extern Module PreGameModule;

    namespace PreGameMgr
    {
        GWCA_API bool IsLoggedOut();
        GWCA_API bool IsSelectingCharacter();
        GWCA_API bool IsPlayingCharacter();

        GWCA_API void LoginAccount(wchar_t* email, wchar_t* password);
        GWCA_API void LogoutAccount();

        GWCA_API CharacterArray* GetCharacterArray();
        GWCA_API void SelectCharacter(wchar_t* character_name);
        GWCA_API void PlayCharacter();
        GWCA_API void ChangeCharacter();
    };
}
