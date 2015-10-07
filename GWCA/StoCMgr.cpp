#include "StoCMgr.h"

inline GWAPI::StoCMgr::StoCMgr(GWAPIMgr * obj) : parent_(obj) {
	game_server_handler_ = *MemoryMgr::ReadPtrChain<StoCHandlerArray*>(MemoryMgr::GetGSObject(), 2, 0x8, 0x2C);
	original_functions_ = new StoCHandler[game_server_handler_.size()];

	for (DWORD i = 0; i < game_server_handler_.size(); ++i) {
		original_functions_[i] = game_server_handler_[i];
	}

}

inline GWAPI::StoCMgr::~StoCMgr() {

	for (DWORD i = 0; i < game_server_handler_.size(); ++i) {
		game_server_handler_[i] = original_functions_[i];
	}

	delete[] original_functions_;
}
