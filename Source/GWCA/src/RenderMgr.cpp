#include "stdafx.h"

#include <GWCA/GameContainers/Array.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/RenderMgr.h>

namespace {
    using namespace GW;

    struct gwdx {
        /* +h0000 */ uint8_t    h0000[24];
        /* +h0018 */ uint32_t   h0018; // might not be a func pointer, seems like it tho lol
        /* +h001C */ uint8_t    h001C[44];
        /* +h0048 */ wchar_t    gpu_name[32];
        /* +h0088 */ uint8_t    h0088[8];
        /* +h0090 */ IDirect3DDevice9 *device;
        /* +h0094 */ uint8_t    h0094[12];
        /* +h00A0 */ uint32_t   framecount;
        /* +h00A4 */ uint8_t    h00A4[2936];
        /* +h0C1C */ uint32_t   viewport_width;
        /* +h0C20 */ uint32_t   viewport_height;
        /* +h0C24 */ uint8_t    h0C24[148];
        /* +h0CB8 */ uint32_t   window_width;
        /* +h0CBC */ uint32_t   window_height;
        /* +h0CC0 */ uint8_t    h0CC0[952];
    }; //Size: 0x1074
    gwdx* gwdx_ptr = nullptr;

    typedef bool(__fastcall *GwEndScene_pt)(gwdx *ctx, void *unk);
    GwEndScene_pt RetGwEndScene;
    GwEndScene_pt GwEndScene_Func;

    GwEndScene_pt RetScreenCapture;
    GwEndScene_pt ScreenCapture_Func;

    typedef bool(__fastcall *GwReset_pt)(gwdx *ctx);
    GwReset_pt RetGwReset;
    GwReset_pt GwReset_Func;

    std::function<void (IDirect3DDevice9 *)> render_callback;
    std::function<void (IDirect3DDevice9 *)> reset_callback;

    bool __fastcall OnGwEndScene(gwdx *ctx, void *unk) {
        gwdx_ptr = ctx;
        if (render_callback)
            render_callback(ctx->device);
        return RetGwEndScene(ctx, unk);
    }

    bool __fastcall OnGwReset(gwdx *ctx) {
        gwdx_ptr = ctx;
        if (reset_callback)
            reset_callback(ctx->device);
        return RetGwReset(ctx);
    }

    bool __fastcall OnScreenCapture(gwdx *ctx, void *unk) {
        // @Enhancement: This should probably be an option.
        if (!GW::UI::GetIsShiftScrennShot() && render_callback) {
            render_callback(ctx->device);
        }
        return RetScreenCapture(ctx, unk);
    }

    void Init() {

        GwEndScene_Func = (GwEndScene_pt)Scanner::Find(
            "\x55\x8B\xEC\x83\xEC\x28\x56\x8B\xF1\x57\x89\x55\xF8", "xxxxxxxxxxxxx", 0);
        printf("[SCAN] GwEndScene = %p\n", GwEndScene_Func);

        ScreenCapture_Func = (GwEndScene_pt)Scanner::Find(
            "\xC3\x39\x86\x94\x00\x00\x00\x74\x0C", "xxxxxxxxx", -69);
        printf("[SCAN] GwScreenCapture = %p\n", ScreenCapture_Func);

        GwReset_pt GwReset_Func = (GwReset_pt)Scanner::Find(
            "\x55\x8B\xEC\x81\xEC\x98\x00\x00\x00\x53\x56\x57\x8B\xF1\x33\xD2", "xxxxxxxxxxxxxxxx", 0);
        printf("[SCAN] GwReset = %p\n", GwReset_Func);
    }

    void CreateHooks() {
        if (Verify(GwEndScene_Func))
            HookBase::CreateHook(GwEndScene_Func, OnGwEndScene, (void **)&RetGwEndScene);
        if (Verify(ScreenCapture_Func))
            HookBase::CreateHook(ScreenCapture_Func, OnScreenCapture, (void **)&RetScreenCapture);
        if (Verify(GwReset_Func))
            HookBase::CreateHook(GwReset_Func, OnGwReset, (void **)&RetGwReset);
    }

    void RemoveHooks() {
        if (GwEndScene_Func)
            HookBase::RemoveHook(GwEndScene_Func);
        if (ScreenCapture_Func)
            HookBase::RemoveHook(ScreenCapture_Func);
        if (GwReset_Func)
            HookBase::RemoveHook(GwReset_Func);
    }
}

namespace GW {

    Module RenderModule = {
        "RenderModule",     // name
        NULL,               // param
        ::Init,             // init_module
        NULL,               // exit_module
        ::CreateHooks,      // exit_module
        ::RemoveHooks,      // remove_hooks
    };

    int Render::GetIsFullscreen() {
        // this is hacky and might be unreliable
        if (gwdx_ptr == nullptr) return -1;
        if (gwdx_ptr->viewport_height == gwdx_ptr->window_height
            && gwdx_ptr->viewport_width == gwdx_ptr->window_width) return 1;
        return 0;
    }

    int Render::GetViewportWidth() {
        if (gwdx_ptr == nullptr) return -1;
        return gwdx_ptr->viewport_width;
    }

    int Render::GetViewportHeight() {
        if (gwdx_ptr == nullptr) return -1;
        return gwdx_ptr->viewport_height;
    }

    void Render::SetRenderCallback(std::function<void(IDirect3DDevice9*)> callback) {
        render_callback = callback;
    }

    void Render::SetResetCallback(std::function<void(IDirect3DDevice9* device)> callback) {
        reset_callback = callback;
    }
} // namespace GW
