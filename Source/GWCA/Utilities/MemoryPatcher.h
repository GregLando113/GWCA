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
        void  *addr;
        void  *patch;
        void  *backup;
        size_t size;

        bool flag;
    };
}
