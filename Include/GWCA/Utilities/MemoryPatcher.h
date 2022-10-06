#pragma once

namespace GW {

    class MemoryPatcher {
    public:
        MemoryPatcher() = default;
        MemoryPatcher(const MemoryPatcher&) = delete;
        ~MemoryPatcher();

        void Reset();
        void SetPatch(uintptr_t addr, const char* patch, size_t size);

        // Use to redirect a CALL or JMP instruction to call a different function instead.
        bool SetRedirect(uintptr_t call_instruction_address, void* redirect_func);

        bool TogglePatch(bool flag);
        bool TogglePatch() { TogglePatch(!m_enable); };

        bool GetIsEnable() { return m_enable; };
    private:
        void       *m_addr = nullptr;
        uint8_t    *m_patch = nullptr;
        uint8_t    *m_backup = nullptr;
        size_t      m_size = 0;
        bool        m_enable = false;
    };
}
