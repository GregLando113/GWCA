#include "stdafx.h"

#include <GWCA/GameContainers/Array.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/RenderMgr.h>

namespace {
    using namespace GW;
    typedef Render::Mat4x3f*(__cdecl* GwGetTransform_pt)(Render::Transform transform);
    GwGetTransform_pt GwGetTransform_func;

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

    typedef bool(__cdecl *GwEndScene_pt)(gwdx *ctx, void *unk);
    GwEndScene_pt RetGwEndScene;
    GwEndScene_pt GwEndScene_Func;

    GwEndScene_pt RetScreenCapture;
    GwEndScene_pt ScreenCapture_Func;

    typedef bool(__cdecl *GwReset_pt)(gwdx *ctx);
    GwReset_pt RetGwReset;
    GwReset_pt GwReset_Func;

    std::function<void (IDirect3DDevice9 *)> render_callback;
    std::function<void (IDirect3DDevice9 *)> reset_callback;

    bool __cdecl OnGwEndScene(gwdx *ctx, void *unk) {
        HookBase::EnterHook();
        gwdx_ptr = ctx;
        if (render_callback)
            render_callback(ctx->device);
        bool retval = RetGwEndScene(ctx, unk);
        HookBase::LeaveHook();
        return retval;
    }

    bool __cdecl OnGwReset(gwdx *ctx) {
        HookBase::EnterHook();
        gwdx_ptr = ctx;
        if (reset_callback)
            reset_callback(ctx->device);
        bool retval = RetGwReset(ctx);
        HookBase::LeaveHook();
        return retval;
    }

    bool __cdecl OnScreenCapture(gwdx *ctx, void *unk) {
        HookBase::EnterHook();
        // @Enhancement: This should probably be an option.
        if (!GW::UI::GetIsShiftScreenShot() && render_callback) {
            render_callback(ctx->device);
        }
        bool retval = RetScreenCapture(ctx, unk);
        HookBase::LeaveHook();
        return retval;
    }

    void Init() {

        uintptr_t address = Scanner::Find(
            "\x8B\x75\x08\x83\xFE\x05\x7C\x14\x68\xDB\x02\x00\x00", "xxxxxxxxxxxxx", -0x4);
        GWCA_INFO("[SCAN] GwGetTransform = %p\n", address);
        if (Verify(address)) {
            GwGetTransform_func = (GwGetTransform_pt)address;
        }

        GwEndScene_Func = (GwEndScene_pt)Scanner::Find(
            "\x89\x45\xFC\x57\x8B\x7D\x08\x8B\x8F", "xxxxxxxxx", -0xD);
        GWCA_INFO("[SCAN] GwEndScene = %p\n", GwEndScene_Func);

        ScreenCapture_Func = (GwEndScene_pt)Scanner::Find(
            "\x83\xC4\x10\x8B\x86\x00\x00\x00\x00\x83", "xxxxx??xxx", -0x8F);
        GWCA_INFO("[SCAN] GwScreenCapture = %p\n", ScreenCapture_Func);

        GwReset_Func = (GwReset_pt)Scanner::Find(
            "\x3B\x4D\xB4\x6A\x00\x1B\xDB\xF7\xDB", "xxxxxxxxx", -0x8C);
        GWCA_INFO("[SCAN] GwReset = %p\n", GwReset_Func);

        if (Verify(GwEndScene_Func))
            HookBase::CreateHook(GwEndScene_Func, OnGwEndScene, (void **)&RetGwEndScene);
        if (Verify(ScreenCapture_Func))
            HookBase::CreateHook(ScreenCapture_Func, OnScreenCapture, (void **)&RetScreenCapture);
        if (Verify(GwReset_Func))
            HookBase::CreateHook(GwReset_Func, OnGwReset, (void **)&RetGwReset);
    }

    void Exit() {
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
        ::Exit,             // exit_module
        NULL,               // enable_hooks
        NULL,               // disable_hooks
    };

    int Render::GetIsFullscreen() {
        // this is hacky and might be unreliable
        if (gwdx_ptr == nullptr) return -1;
        if (gwdx_ptr->viewport_height == gwdx_ptr->window_height
            && gwdx_ptr->viewport_width == gwdx_ptr->window_width) return 1;
        return 0;
    }

    uint32_t Render::GetViewportWidth() {
        // @Cleanup: What to really do here?
        GWCA_ASSERT(gwdx_ptr != nullptr);
        return gwdx_ptr->viewport_width;
    }

    uint32_t Render::GetViewportHeight() {
        // @Cleanup: What to really do here?
        GWCA_ASSERT(gwdx_ptr != nullptr);
        return gwdx_ptr->viewport_height;
    }

    Render::Mat4x3f* Render::GetTransform(Transform transform) {
        GWCA_ASSERT(GwGetTransform_func != nullptr);
        return GwGetTransform_func(transform);
    }

    void Render::SetRenderCallback(std::function<void(IDirect3DDevice9*)> callback) {
        render_callback = callback;
    }

    void Render::SetResetCallback(std::function<void(IDirect3DDevice9* device)> callback) {
        reset_callback = callback;
    }
} // namespace GW
