#include "..\..\Utilities\MemoryPatcher.h"

GW::MemoryPatcher::MemoryPatcher(LPVOID addr, BYTE *patch, UINT size) {
	this->addr = addr;
	this->size = size;
	this->flag = false;

	this->patch = new BYTE[size];
	memcpy(this->patch, patch, size);

	this->backup = new BYTE[size];

	DWORD oldProt;
	VirtualProtect(addr, size, PAGE_EXECUTE_READ, &oldProt);
	memcpy(this->backup, addr, size);
	VirtualProtect(addr, size, oldProt, &oldProt);
}

GW::MemoryPatcher::~MemoryPatcher() {
	TooglePatch(false);
	delete[] patch;
	delete[] backup;
}

bool GW::MemoryPatcher::TooglePatch(bool flag) {
	if (this->flag == flag)
		return flag;

	DWORD oldProt;
	VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oldProt);
	if (flag) {
		memcpy(addr, patch, size);
	} else {
		memcpy(addr, backup, size);
	}
	VirtualProtect(addr, size, oldProt, &oldProt);

	this->flag = flag;
	return flag;
}
