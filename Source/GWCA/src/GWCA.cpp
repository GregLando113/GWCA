#include "stdafx.h"

#include <GWCA/GWCA.h>

#include <GWCA/Constants/Constants.h>

#include <GWCA/GameContainers/Vector.h>
#include <GWCA/Packets/StoC.h>

#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Skill.h>
#include <GWCA/GameEntities/Camera.h>
#include <GWCA/GameEntities/Player.h>

#include <GWCA/Managers/Render.h>
#include <GWCA/Managers/StoCMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/PartyMgr.h>
#include <GWCA/Managers/CameraMgr.h>
#include <GWCA/Managers/EffectMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

bool GW::Initialize() {
    if (GW::MemoryMgr::Scan()) {

        // force the initialization of gamethread
        GW::GameThread::Initialize();
        GW::Agents::Initialize();

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
    GW::Render::RestoreHooks();
    GW::Items::RestoreHooks();

    GW::GameThread::RestoreHooks();
}
