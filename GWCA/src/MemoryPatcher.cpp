#include <stdint.h>
#include <Windows.h>

#include <GWCA/Utilities/MemoryPatcher.h>

GW::MemoryPatcher::MemoryPatcher(uintptr_t addr, void *patch, size_t size) {
    this->addr = (void *)addr;
    this->size = size;
    this->flag = false;

    this->patch = new char[size];
    memcpy(this->patch, patch, size);

    this->backup = new char[size];

    DWORD oldProt;
    VirtualProtect(this->addr, size, PAGE_EXECUTE_READ, &oldProt);
    memcpy(this->backup, this->addr, size);
    VirtualProtect(this->addr, size, oldProt, &oldProt);
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
