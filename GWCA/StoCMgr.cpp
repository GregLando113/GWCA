#include "StoCMgr.h"

#include "GWAPIMgr.h"

GWAPI::StoCMgr::StoCHandlerArray GWAPI::StoCMgr::game_server_handler_;
GWAPI::StoCMgr::StoCHandler* GWAPI::StoCMgr::original_functions_ = NULL;
std::map<DWORD, std::vector<GWAPI::StoCMgr::Handler>> GWAPI::StoCMgr::event_calls_;

bool GWAPI::StoCMgr::StoCHandlerFunc(StoC::PacketBase* pak, DWORD unk)
{
	for (auto call : event_calls_[pak->header])
	{
		call(pak);
	}
	return original_functions_[pak->header].handlerfunc(pak, unk);
}

GWAPI::StoCMgr::StoCMgr(GWAPIMgr& api) : GWCAManager(api) {
	StoCHandlerArray* ptr = MemoryMgr::ReadPtrChain<StoCHandlerArray*>(*(DWORD*)MemoryMgr::GSObjectPtr, 2, 0x8, 0x2C);
	
	game_server_handler_ = *ptr;

	original_functions_ = new StoCHandler[game_server_handler_.size()];

	for (DWORD i = 0; i < game_server_handler_.size(); ++i) {
		original_functions_[i] = game_server_handler_[i];
	}

}

GWAPI::StoCMgr::~StoCMgr() {

	for (DWORD i = 0; i < game_server_handler_.size(); ++i) {
		game_server_handler_[i] = original_functions_[i];
	}

	delete[] original_functions_;
}
