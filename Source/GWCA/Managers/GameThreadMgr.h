#pragma once

namespace GW {

    struct Module;
    extern Module GameThreadModule;

    namespace GameThread {
        GWCA_API void ClearCalls();

        GWCA_API void Enqueue(std::function<void ()> f);

        GWCA_API uint32_t AddPermanentCall(std::function<void ()> f);

        GWCA_API void RemovePermanentCall(uint32_t identifier);
        
        // static void renderHook();
        // GWCA_API void ToggleRenderHook();
    };
}
