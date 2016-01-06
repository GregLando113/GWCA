#include "MemoryPatcher.h"

GWCA::MemoryPatcher::MemoryPatcher(LPVOID addr, BYTE *patch, UINT size) {
	this->addr = addr;
	this->size = size;
	this->enable = false;

	this->patch = new BYTE[size];
	memcpy(this->patch, patch, size);
	
	this->backup = new BYTE[size];

	DWORD oldProt;
	VirtualProtect(addr, size, PAGE_EXECUTE_READ, &oldProt);
	memcpy(this->backup, addr, size);
	VirtualProtect(addr, size, oldProt, &oldProt);
}

GWCA::MemoryPatcher::~MemoryPatcher() {
	TooglePatch(false);
	delete[] patch;
	delete[] backup;
}

bool GWCA::MemoryPatcher::TooglePatch(bool enable) {
	if (this->enable == enable)
		return enable;

	DWORD oldProt;
	VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oldProt);
	if (enable) {
		memcpy(addr, patch, size);
	} else {
		memcpy(addr, backup, size);
	}
	VirtualProtect(addr, size, oldProt, &oldProt);

	this->enable = enable;
	return enable;
}
