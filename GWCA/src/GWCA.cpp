#include "..\GWCA.h"

#include "..\Managers\MemoryMgr.h"
#include "..\Managers\GameThreadMgr.h"
#include "..\Managers\AgentMgr.h"
#include "..\Managers\PartyMgr.h"
#include "..\Managers\EffectMgr.h"
#include "..\Managers\ChatMgr.h"
#include "..\Managers\StoCMgr.h"
#include "..\Managers\CameraMgr.h"

bool GW::Initialize() {
	if (MemoryMgr::Scan()) {

		// force the initialization of gamethread
		GameThread::Initialize();
		Agents::Initialize();

		return true;
	} else {
		return false;
	}
}

void GW::Terminate() {
	GW::GameThread::ClearCalls();

	GW::CameraMgr::RestoreHooks();
	GW::Chat::RestoreHooks();
	GW::PartyMgr::RestoreHooks();
	GW::StoC::RestoreHooks();
	GW::Agents::RestoreHooks();
	GW::Effects::RestoreHooks();

	GW::GameThread::RestoreHooks();
}
