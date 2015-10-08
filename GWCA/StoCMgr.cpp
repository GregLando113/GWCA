#include "StoCMgr.h"

GWAPI::StoCMgr::StoCHandler* GWAPI::StoCMgr::game_server_handler_ = NULL;
DWORD GWAPI::StoCMgr::game_server_handler_count_ = 0;
GWAPI::StoCMgr::StoCHandler* GWAPI::StoCMgr::original_functions_ = NULL;
std::vector<GWAPI::StoCMgr::Handler>* GWAPI::StoCMgr::event_calls_ = NULL;

void GWAPI::StoCMgr::AddGameServerEvent(DWORD packetheader, Handler func)
{
	event_calls_[packetheader].push_back(func);
	game_server_handler_[packetheader].handlerfunc = StoCHandlerFunc;
}

bool GWAPI::StoCMgr::StoCHandlerFunc(StoCPacketBase* pak, DWORD unk)
{
	for (auto call : event_calls_[pak->header])
	{
		call(pak);
	}
	return original_functions_[pak->header].handlerfunc(pak, unk);
}

GWAPI::StoCMgr::StoCMgr(GWAPIMgr * obj) : parent_(obj) {
	StoCHandler** ptr = MemoryMgr::ReadPtrChain<StoCHandler**>(*(DWORD*)MemoryMgr::GSObjectPtr, 2, 0x8, 0x2C);

	
	game_server_handler_ = *ptr;
	game_server_handler_count_ = *(DWORD*)(((BYTE*)ptr) + 0x8);

	original_functions_ = new StoCHandler[game_server_handler_count_];
	event_calls_ = new std::vector<Handler>[game_server_handler_count_];

	for (DWORD i = 0; i < game_server_handler_count_; ++i) {
		original_functions_[i] = game_server_handler_[i];
	}

}

GWAPI::StoCMgr::~StoCMgr() {

	for (DWORD i = 0; i < game_server_handler_count_; ++i) {
		game_server_handler_[i] = original_functions_[i];
	}

	delete[] original_functions_;
	delete[] event_calls_;
}
