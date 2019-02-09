#pragma once

namespace GW {

    // Shoutouts to DarthTon @ unknowncheats.me for this class.
    namespace GameThread {
        GWCA_API void Initialize();
        GWCA_API void ClearCalls();
        GWCA_API void RestoreHooks();

        GWCA_API void Enqueue(std::function<void ()> f);

        GWCA_API uint32_t AddPermanentCall(std::function<void ()> f);

        GWCA_API void RemovePermanentCall(uint32_t identifier);
        
        // static void renderHook();
        GWCA_API void ToggleRenderHook();
    };
}
