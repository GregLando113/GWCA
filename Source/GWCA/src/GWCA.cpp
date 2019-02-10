#include "stdafx.h"

#include <GWCA/GWCA.h>

#include <GWCA/Constants/Constants.h>

#include <GWCA/GameContainers/Vector.h>
#include <GWCA/Packets/StoC.h>

#include <GWCA/Utilities/Hooker.h>

#include <GWCA/GameEntities/Map.h>
#include <GWCA/GameEntities/NPC.h>
#include <GWCA/GameEntities/Item.h>
#include <GWCA/GameEntities/Agent.h>
#include <GWCA/GameEntities/Guild.h>
#include <GWCA/GameEntities/Skill.h>
#include <GWCA/GameEntities/Camera.h>
#include <GWCA/GameEntities/Player.h>
#include <GWCA/GameEntities/Pathing.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/CtoSMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/StoCMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/GuildMgr.h>
#include <GWCA/Managers/PartyMgr.h>
#include <GWCA/Managers/TradeMgr.h>
#include <GWCA/Managers/CameraMgr.h>
#include <GWCA/Managers/EffectMgr.h>
#include <GWCA/Managers/RenderMgr.h>
#include <GWCA/Managers/PlayerMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/SkillbarMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>

namespace GW {
    std::vector<Module *> modules;

    bool Initialize() {
        modules.push_back(&UIModule);
        modules.push_back(&MapModule);
        modules.push_back(&ChatModule);
        modules.push_back(&CtoSModule);
        modules.push_back(&ItemModule);
        modules.push_back(&StoCModule);
        modules.push_back(&AgentModule);
        modules.push_back(&GuildModule);
        modules.push_back(&PartyModule);
        modules.push_back(&TradeModule);
        modules.push_back(&CameraModule);
        modules.push_back(&EffectModule);
        modules.push_back(&PlayerModule);
        modules.push_back(&RenderModule);
        modules.push_back(&SkillbarModule);
        modules.push_back(&GameThreadModule);

        if (MemoryMgr::Scan()) {

            for (Module *module : modules) {
                printf("Initializing module '%s'\n", module->name);
                if (module->init_module)
                    module->init_module();
            }

            HookBase::Initialize();
            for (Module *module : modules) {
                if (module->create_hooks)
                    module->create_hooks();
            }

            HookBase::EnableHooks();

            return true;
        } else {
            return false;
        }
    }

    void Terminate() {

        HookBase::DisableHooks();
        for (Module *module : modules) {
            if (module->remove_hooks)
                module->remove_hooks();
        }

        HookBase::Deinitialize();

        for (Module *module : modules) {
            if (module->exit_module)
                module->exit_module();
        }
    }
} // namespace GW