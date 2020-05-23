#pragma once

#include <GWCA/Utilities/Export.h>

// forward declaration, we don't need to include the full directx header here
struct IDirect3DDevice9;

namespace GW {

    struct Module;
    extern Module RenderModule;

    namespace Render {
        // Set up a callback for drawing on screen. 
        // Will be called after GW render. 
        // 
        // Important: if you use this, you should call  GW::Terminate() 
        // or at least GW::Render::RestoreHooks() from within the callback
        GWCA_API void SetRenderCallback(std::function<void(IDirect3DDevice9 *)> callback);

        // Set up a callback for directx device reset
        GWCA_API void SetResetCallback(std::function<void(IDirect3DDevice9 *)> callback);

        // Check if gw is in fullscreen
        // Note: requires one or both callbacks to be set and called before
        // Note: does not update while minimized
        // Note: returns -1 if it doesn't know yet
        GWCA_API int GetIsFullscreen();

        GWCA_API uint32_t GetViewportWidth();
        GWCA_API uint32_t GetViewportHeight();
    }
}
