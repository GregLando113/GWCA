#pragma once

namespace GW {

    class MemoryPatcher {
    public:
        MemoryPatcher() = default;
        MemoryPatcher(const MemoryPatcher&) = delete;
        ~MemoryPatcher();

        void Reset();
        void SetPatch(uintptr_t addr, void* patch, size_t size);

        bool TooglePatch(bool flag);
        bool TooglePatch() { TooglePatch(!m_enable); };

        bool GetIsEnable() { return m_enable; };
    private:
        void       *m_addr = nullptr;
        uint8_t    *m_patch = nullptr;
        uint8_t    *m_backup = nullptr;
        size_t      m_size = 0;
        bool        m_enable = false;
    };
}
