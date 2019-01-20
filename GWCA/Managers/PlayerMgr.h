#pragma once

namespace GW {
    namespace PlayerMgr {

        GWCA_API void SetActiveTitle(GW::Constants::TitleID id);

        GWCA_API void RemoveActiveTitle();

        GWCA_API GW::PlayerArray& GetPlayerArray();

        GWCA_API GW::Player& GetPlayerByID(PlayerID id);

        GWCA_API wchar_t* GetPlayerName(PlayerID id);

        GWCA_API void SetPlayerName(PlayerID id, const wchar_t* replacename);

        GWCA_API void ChangeSecondProfession(GW::Constants::Profession prof, DWORD heroindex = 0);

        GWCA_API GW::Player *GetPlayerByName(const wchar_t *name);
    };
}
