#include <Windows.h>

#include "GWCA.h"

#include <GWCA\GWCA.h>
#include <GWCA\Managers\AgentMgr.h>
#include <GWCA\Managers\CameraMgr.h>
#include <GWCA\Managers\ChatMgr.h>
#include <GWCA\Managers\CToSMgr.h>
#include <GWCA\Managers\EffectMgr.h>
#include <GWCA\Managers\FriendListMgr.h>
#include <GWCA\Managers\GameThreadMgr.h>
#include <GWCA\Managers\GuildMgr.h>
#include <GWCA\Managers\ItemMgr.h>
#include <GWCA\Managers\MapMgr.h>
#include <GWCA\Managers\MemoryMgr.h>
#include <GWCA\Managers\MerchantMgr.h>
#include <GWCA\Managers\PartyMgr.h>
#include <GWCA\Managers\PlayerMgr.h>
#include <GWCA\Managers\SkillbarMgr.h>
#include <GWCA\Managers\StoCMgr.h>
#include <GWCA\Managers\TradeMgr.h>

#pragma comment(lib, "GWCAd.lib")

using namespace GW;

GWAPI GW::Agent* GWCA::GetPlayer() { return Agents::GetPlayer(); }
GWAPI GW::Agent* GWCA::GetTarget() { return Agents::GetTarget(); }

GWAPI GW::Agent* GWCA::GetAgentByID(DWORD id) { return Agents::GetAgentByID(id); }

// Get Current AgentID's of player or target.
GWAPI DWORD GWCA::GetPlayerId() { return Agents::GetPlayerId(); }
GWAPI DWORD GWCA::GetTargetId() { return Agents::GetTargetId(); }

GWAPI void GWCA::SendChat(char channel, const wchar *msg) {
	GW::Chat::SendChat(msg, channel);
}

GWAPI void GWCA::SendChat(char channel, const char *msg) {
	GW::Chat::SendChat(msg, channel);
}

GWAPI void GWCA::PrintChat(Channel channel, const wchar *msg) {
	GW::Chat::WriteChat((GW::Chat::Channel)channel, msg);
}

GWAPI void GWCA::CreateCommand(std::wstring cmd, CmdCB callback) {
    // GW::Chat::RegisterCommand(cmd, callback);
}

GWAPI void GWCA::DeleteCommand(std::wstring cmd) {
	// GW::Chat::DeleteCommand(cmd);
}

static void (*_OnEvent)(GWCA::Event e, ...);

static bool _OnLocalMessage(int chan, wchar *msg) {
    _OnEvent(GWCA::Event::OnChatMessage, chan, L"", msg);
	return false;
}

static void _OnWhisperMessage(const wchar sender[20], const wchar msg[140]) {
    _OnEvent(GWCA::Event::OnChatMessage, GWCA::CHANNEL_WHISPER, sender, msg);
}

static bool _OnPacketRecv(GW::Packet::StoC::PacketBase *p) {
    _OnEvent(GWCA::Event::OnServerMessage, p->header, p);
	return false;
}

DWORD WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        if(!GW::Initialize())
            return FALSE;

        GW::Chat::SetLocalMessageCallback(_OnLocalMessage);
        GW::Chat::SetWhisperCallback(_OnWhisperMessage);
        for (int i = 0; i < 481; i++)
            GW::StoC::AddCallback(i, _OnPacketRecv);
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        GW::Terminate();
    }
    return TRUE;
}