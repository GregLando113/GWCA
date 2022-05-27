#pragma once
#include <GWCA/Constants/Types.h>


#include <GWCA/Utilities/Export.h>

namespace GW {

    struct Player;
    typedef Array<Player> PlayerArray;

    namespace Constants {
        enum class TitleID;
    }

    struct Module;
    extern Module PlayerModule;

    namespace PlayerMgr {

        GWCA_API void SetActiveTitle(Constants::TitleID title_id);

        GWCA_API void RemoveActiveTitle();

        GWCA_API PlayerArray* GetPlayerArray();

        // Get the player number of the currently logged in character
        GWCA_API PlayerID GetPlayerID();

        GWCA_API Player *GetPlayerByID(PlayerID player_id);

        GWCA_API wchar_t *GetPlayerName(PlayerID player_id);

        GWCA_API bool SetPlayerName(PlayerID player_id, const wchar_t *replace_name);

        GWCA_API bool ChangeSecondProfession(Constants::Profession prof, uint32_t hero_index = 0);

        GWCA_API Player *GetPlayerByName(const wchar_t *name);
    };
}
