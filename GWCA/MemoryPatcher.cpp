#include "MemoryPatcher.h"

GWAPI::MemoryPatcher::MemoryPatcher(LPVOID addr, BYTE *patch, UINT size)
{
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

GWAPI::MemoryPatcher::~MemoryPatcher()
{
	TooglePatch(false);
	delete[] this->patch;
	delete[] this->backup;
}

bool GWAPI::MemoryPatcher::TooglePatch(bool enable)
{
	if (this->enable == enable)
		return enable;

	DWORD oldProt;
	VirtualProtect(this->addr, this->size, PAGE_EXECUTE_READWRITE, &oldProt);
	if (enable) {
		memcpy(this->addr, this->patch, this->size);
	} else {
		memcpy(this->addr, this->backup, this->size);
	}
	VirtualProtect(this->addr, this->size, oldProt, &oldProt);

	this->enable = enable;
	return enable;
}
