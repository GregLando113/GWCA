#pragma once

/**********************************************************************************\

_____ _    _ _____   ___
|  __ \ |  | /  __ \ / _ \   _     _
| |  \/ |  | | /  \// /_\ \_| |_ _| |_
| | __| |/\| | |    |  _  |_   _|_   _|
| |_\ \  /\  / \__/\| | | | |_|   |_|
\____/\/  \/ \____/\_| |_/


Created by KAOS (a.k.a. 4D 1) and gigi for use in GWToolbox++

Credits to:

Sune C (a.k.a. Harboe) and ACB -
Most low level ground work of API via the GWCA and GWLP:R
projects as well as getting the gamerevision community rolling.
Much help from following these two gentlemen's breadcrumbs.

Miracle444 -
GWA2, as well as helping me really understand how the game handles
its memory information.

d8rken -
Zraw menu/api which acted as a gateway to get a grasp of cpp as well
as directx.

gigi -
For getting me to not make everything singletons :P

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, and/or merge copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

\**********************************************************************************/

/*
Class used for GWCA initialization, access and destruction

// start by calling
GWCA::Initialize()

// then access through GWCA object
GWCA api;
api().Agent().GetPlayer()

// or directly through static call
GWCA::Api().Agent().GetPlayer()

// Finally destroy GWCA
GWCA::Destruct()

*/

#include <Windows.h>
#include <vector>

// included memorymgr, gamethread and ctos since 
// they are needed by most other managers
#include "Managers\MemoryMgr.h"
#include "Managers\GameThreadMgr.h"
#include "Managers\CtoSMgr.h"

// include agentmgr and mapmgr by default since they are used by most applications
#include "Managers\AgentMgr.h"
#include "Managers\MapMgr.h"

namespace GWCA {

	class GameThreadMgr;
	class CtoSMgr;
	class StoCMgr;
	class AgentMgr;
	class PartyMgr;
	class ItemMgr;
	class SkillbarMgr;
	class EffectMgr;
	class ChatMgr;
	class MerchantMgr;
	class GuildMgr;
	class MapMgr;
	class FriendListMgr;
	class CameraMgr;

	// GWCA Module Accessors.
	GameThreadMgr& Gamethread();
	CtoSMgr& CtoS();
	StoCMgr& StoC();
	AgentMgr& Agents();
	PartyMgr& Party();
	ItemMgr& Items();
	SkillbarMgr& Skillbar();
	EffectMgr& Effects();
	ChatMgr& Chat();
	MerchantMgr& Merchant();
	GuildMgr& Guild();
	MapMgr& Map();
	FriendListMgr& FriendList();
	CameraMgr& Camera();

	class Api {
		friend class GWCABaseManager;

	public:
		static bool Initialize();
		static void Destruct();

	private:
		static std::vector<GWCABaseManager*> managers;
	};
}
