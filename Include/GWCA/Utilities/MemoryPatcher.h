#pragma once

namespace GW {

    class MemoryPatcher {
    public:
        MemoryPatcher(uintptr_t addr, void *patch, size_t size);
        ~MemoryPatcher();

        bool TooglePatch(bool flag);
        bool TooglePatch() { TooglePatch(!flag); };

        bool GetPatchState() { return flag; };
    private:
        void    *addr;
        uint8_t *patch;
        uint8_t *backup;
        size_t   size;

        bool flag;
    };
}
