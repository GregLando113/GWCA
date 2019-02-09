#include "stdafx.h"

#include <GWCA/GameContainers/Array.h>

#include <GWCA/Utilities/Export.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/Render.h>

namespace {
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

    typedef bool(__fastcall *GwEndScene_t)(gwdx *ctx, void *unk);
    typedef bool(__fastcall *GwReset_t)(gwdx *ctx);

    GwEndScene_t endscene_original = nullptr;
    GwEndScene_t screen_capture_original = nullptr;

    GW::THook<GwEndScene_t> endscene_hook;
    GW::THook<GwEndScene_t> screen_capture_hook;
    GW::THook<GwReset_t> reset_hook;

    std::function<void(IDirect3DDevice9 *)> render_callback;
    std::function<void(IDirect3DDevice9 *)> reset_callback;

    bool __fastcall endscene_detour(gwdx *ctx, void *unk) {
        gwdx_ptr = ctx;
        render_callback(ctx->device);
        if (endscene_hook.Valid()) {
            return endscene_hook.Original()(ctx, unk);
        } else {
            return endscene_original(ctx, unk);
        }
    }

    bool __fastcall reset_detour(gwdx *ctx) {
        gwdx_ptr = ctx;
        reset_callback(ctx->device);
        return reset_hook.Original()(ctx);
    }

    bool __fastcall screen_capture_detour(gwdx *ctx, void *unk) {
        // @Enhancement: This should probably be an option.
        if (!GW::UI::GetIsShiftScrennShot()) {
            render_callback(ctx->device);
        }
        if (screen_capture_hook.Valid()) {
            return screen_capture_hook.Original()(ctx, unk);
        } else {
            return screen_capture_original(ctx, unk);
        }
    }
}

namespace GW {
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
        if (endscene_hook.Empty()) {
            endscene_original = (GwEndScene_t)Scanner::Find(
                "\x55\x8B\xEC\x83\xEC\x28\x56\x8B\xF1\x57\x89\x55\xF8", "xxxxxxxxxxxxx", 0);
            printf("[SCAN] GwEndScene address = %p\n", endscene_original);
            endscene_hook.Detour(endscene_original, endscene_detour);
        }
        if (screen_capture_hook.Empty()) {
            screen_capture_original = (GwEndScene_t)Scanner::Find(
                "\xC3\x39\x86\x94\x00\x00\x00\x74\x0C", "xxxxxxxxx", -69);
            printf("[SCAN] GwScreenCapture address = %p\n", screen_capture_original);
            screen_capture_hook.Detour(screen_capture_original, screen_capture_detour);
        }
    }

    void Render::SetResetCallback(std::function<void(IDirect3DDevice9* device)> callback) {
        reset_callback = callback;
        if (reset_hook.Empty()) {
            GwReset_t source = (GwReset_t)Scanner::Find(
                "\x55\x8B\xEC\x81\xEC\x98\x00\x00\x00\x53\x56\x57\x8B\xF1\x33\xD2", "xxxxxxxxxxxxxxxx", 0);
            printf("[SCAN] GwReset address = %p\n", source);
            reset_hook.Detour(source, reset_detour);
        }
    }

    void Render::RestoreHooks() {
        HookBase::DisableHooks(&endscene_hook);
        HookBase::DisableHooks(&reset_hook);
        HookBase::DisableHooks(&screen_capture_hook);
    }
} // namespace GW
