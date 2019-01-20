#pragma once

namespace GW {

    // Shoutouts to DarthTon @ unknowncheats.me for this class.
    namespace GameThread {
        GWCA_API void Initialize();
        GWCA_API void ClearCalls();
        GWCA_API void RestoreHooks();

        GWCA_API void Enqueue(std::function<void()> f);

        GWCA_API DWORD AddPermanentCall(std::function<void()> f);

        GWCA_API void RemovePermanentCall(DWORD identifier);
        
        // static void renderHook();
        GWCA_API void ToggleRenderHook();
    };
}
