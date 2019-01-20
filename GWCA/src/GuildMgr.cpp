#include <stdint.h>
#include <Windows.h>

#include <GWCA/CtoSHeaders.h>
#include <GWCA/Utilities/Export.h>

#include <GWCA/GameEntities/Guild.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/GuildContext.h>

#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/GuildMgr.h>

GW::GuildContext* GW::GuildMgr::GetGuildContext() {
    return GameContext::instance()->guild;
}

wchar_t* GW::GuildMgr::GetPlayerGuildAnnouncer() {
    return GameContext::instance()->guild->announcement_author;
}

wchar_t* GW::GuildMgr::GetPlayerGuildAnnouncement() {
    return GameContext::instance()->guild->announcement;
}

DWORD GW::GuildMgr::GetPlayerGuildIndex() {
    return GameContext::instance()->guild->player_guild_index;
}

GW::GuildArray GW::GuildMgr::GetGuildArray() {
    return GameContext::instance()->guild->guilds;
}

void GW::GuildMgr::TravelGH() {
    GW::GHKey playerguild = GameContext::instance()->guild->player_gh_key;

    CtoS::SendPacket(0x18, CtoGS_MSGTravelGH, playerguild.k[0], playerguild.k[1], playerguild.k[2], playerguild.k[3]);
}

void GW::GuildMgr::TravelGH(GW::GHKey key) {
    CtoS::SendPacket(0x18, CtoGS_MSGTravelGH, key.k[0], key.k[1], key.k[2], key.k[3]);
}

void GW::GuildMgr::LeaveGH() {
    CtoS::SendPacket(0x8, CtoGS_MSGLeaveGH, 0x1);
}
