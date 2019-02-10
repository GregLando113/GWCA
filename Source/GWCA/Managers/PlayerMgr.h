#pragma once

namespace GW {

    struct Player;

    namespace Constants {
        enum TitleID : uint32_t;
        enum class Profession;
    }

    struct Module;
    extern Module PlayerModule;

    namespace PlayerMgr {

        GWCA_API void SetActiveTitle(Constants::TitleID title_id);

        GWCA_API void RemoveActiveTitle();

        GWCA_API PlayerArray& GetPlayerArray();

        GWCA_API Player *GetPlayerByID(uint32_t player_id);

        GWCA_API wchar_t *GetPlayerName(uint32_t player_id);

        GWCA_API void SetPlayerName(uint32_t player_id, const wchar_t *replace_name);

        GWCA_API void ChangeSecondProfession(Constants::Profession prof, uint32_t hero_index = 0);

        GWCA_API Player *GetPlayerByName(const wchar_t *name);
    };
}
