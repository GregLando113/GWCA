#include "GWCA.h"

GWAPI::GWAPIMgr* GWAPI::GWCA::api_ = nullptr;

bool GWAPI::GWCA::Initialize() {
	if (MemoryMgr::Scan()) {
		api_ = new GWAPIMgr();
		return true;
	} else {
		return false;
	}
}
void GWAPI::GWCA::Destruct() {
	if (api_) {
		delete api_;
		api_ = nullptr;
	}
}
