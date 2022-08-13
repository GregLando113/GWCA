#include "stdafx.h"

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/MemoryPatcher.h>

namespace GW {
    MemoryPatcher::~MemoryPatcher() {
        Reset();
    }

    void MemoryPatcher::Reset() {
        if (GetIsEnable())
            TogglePatch(false);

        if (m_patch) {
            delete[] m_patch;
            m_patch = nullptr;
        }
        if (m_backup) {
            delete[] m_backup;
            m_backup = nullptr;
        }

        m_addr = nullptr;
        m_size = 0;
        m_enable = false;
    }

    void MemoryPatcher::SetPatch(uintptr_t addr, const char *patch, size_t size) {
        GWCA_ASSERT(m_addr == nullptr);

        m_addr = reinterpret_cast<void *>(addr);
        m_size = size;
        m_enable = false;

        m_patch = new uint8_t[size];
        m_backup = new uint8_t[size];
        memcpy(m_patch, patch, size);

        DWORD old_prot;
        VirtualProtect(m_addr, size, PAGE_EXECUTE_READ, &old_prot);
        memcpy(m_backup, m_addr, size);
        VirtualProtect(m_addr, size, old_prot, &old_prot);
    }

    bool MemoryPatcher::TogglePatch(bool flag) {
        GWCA_ASSERT(m_addr != nullptr);

        if (m_enable == flag)
            return flag;

        DWORD old_prot;
        VirtualProtect(m_addr, m_size, PAGE_EXECUTE_READWRITE, &old_prot);
        if (flag) {
            memcpy(m_addr, m_patch, m_size);
        } else {
            memcpy(m_addr, m_backup, m_size);
        }
        VirtualProtect(m_addr, m_size, old_prot, &old_prot);

        m_enable = flag;
        return flag;
    }
}
