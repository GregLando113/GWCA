#include <GWCA\Managers\PlayerMgr.h>

#include <GWCA\Context\GameContext.h>

#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\CtoSMgr.h>

void GW::PlayerMgr::SetActiveTitle(GW::Constants::TitleID id) {
	CtoS::SendPacket(0x8, 0x57, (DWORD)id);
}

void GW::PlayerMgr::RemoveActiveTitle() {
	CtoS::SendPacket(0x4, 0x58);
}

GW::PlayerArray& GW::PlayerMgr::GetPlayerArray() {
	return GameContext::instance()->world->players;
}

GW::Player& GW::PlayerMgr::GetPlayerByID(PlayerID id) {
	return GetPlayerArray()[id];
}

wchar_t* GW::PlayerMgr::GetPlayerName(PlayerID id) {
	return GetPlayerArray()[id].Name;
}

void GW::PlayerMgr::SetPlayerName(PlayerID id, const wchar *replacename) {
	wcscpy_s(GetPlayerArray()[id].NameEnc + 2, 20, replacename);
}

void GW::PlayerMgr::ChangeSecondProfession(GW::Constants::Profession prof, DWORD heroindex) {
	CtoS::SendPacket(12, 0x41, GW::Agents::GetHeroAgentID(heroindex), prof);
}
