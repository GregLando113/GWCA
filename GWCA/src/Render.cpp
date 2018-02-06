#include "../Managers/Render.h"

#include <stdio.h>

#include "../Utilities/Hooker.h"
#include "../Utilities/Scanner.h"

namespace {
	struct gwdx
	{
		char pad_0000[24]; //0x0000
		void* unk; //0x0018 might not be a func pointer, seems like it tho lol
		char pad_001C[44]; //0x001C
		wchar_t gpuname[32]; //0x0048
		char pad_0088[8]; //0x0088
		IDirect3DDevice9* device; //0x0090 IDirect3DDevice9*
		char pad_0094[12]; //0x0094
		unsigned int framecount; //0x00A0
		char pad_00A4[2936]; //0x00A4
		DWORD viewport_width;	// 0x0C1C
		DWORD viewport_height;	// 0x0C20
		char pad_0C24[148];	// 0xC24
		DWORD window_width;		// 0x0CB8
		DWORD window_height;	// 0x0CBC
		char pad_0CC0[952];		// 0x0CC0
	}; //Size: 0x1074
	gwdx* gwdx_ptr = nullptr;

	typedef bool(__fastcall *GwEndScene_t)(gwdx* ctx, void* unk);
	typedef bool(__fastcall *GwReset_t)(gwdx* ctx);

	GwEndScene_t endscene_original = nullptr;
	GwEndScene_t screen_capture_original = nullptr;

	GW::THook<GwEndScene_t> endscene_hook;
	GW::THook<GwEndScene_t> screen_capture_hook;
	GW::THook<GwReset_t> reset_hook;

	std::function<void(IDirect3DDevice9*)> render_callback;
	std::function<void(IDirect3DDevice9*)> reset_callback;

	bool __fastcall endscene_detour(gwdx* ctx, void* unk) {
		gwdx_ptr = ctx;
		render_callback(ctx->device);
		if (endscene_hook.Valid()) {
			return endscene_hook.Original()(ctx, unk);
		} else {
			return endscene_original(ctx, unk);
		}
	}

	bool __fastcall reset_detour(gwdx* ctx) {
		gwdx_ptr = ctx;
		reset_callback(ctx->device);
		return reset_hook.Original()(ctx);
	}

	bool __fastcall screen_capture_detour(gwdx* ctx, void* unk) {
		render_callback(ctx->device);
		if (screen_capture_hook.Valid()) {
			return screen_capture_hook.Original()(ctx, unk);
		} else {
			return screen_capture_original(ctx, unk);
		}
	}
}

int GW::Render::GetIsFullscreen() {
	// this is hacky and might be unreliable
	if (gwdx_ptr == nullptr) return -1;
	if (gwdx_ptr->viewport_height == gwdx_ptr->window_height
		&& gwdx_ptr->viewport_width == gwdx_ptr->window_width) return 1;
	return 0;
}

int GW::Render::GetViewportWidth() {
	if (gwdx_ptr == nullptr) return -1;
	return gwdx_ptr->viewport_width;
}

int GW::Render::GetViewportHeight() {
	if (gwdx_ptr == nullptr) return -1;
	return gwdx_ptr->viewport_height;
}

void GW::Render::SetRenderCallback(std::function<void(IDirect3DDevice9*)> callback) {
	render_callback = callback;
	if (endscene_hook.Empty()) {
		endscene_original = (GwEndScene_t)GW::Scanner::Find("\x55\x8B\xEC\x83\xEC\x28\x56\x8B\xF1\x57\x89\x55\xF8", "xxxxxxxxxxxxx", 0);
		printf("GwEndScene address = %p\n", endscene_original);
		endscene_hook.Detour(endscene_original, endscene_detour);
	}
	if (screen_capture_hook.Empty()) {
		screen_capture_original = (GwEndScene_t)GW::Scanner::Find("\xC3\x39\x86\x94\x00\x00\x00\x74\x0C", "xxxxxxxxx", -69);
		printf("GwScreenCapture address = %p\n", screen_capture_original);
		screen_capture_hook.Detour(screen_capture_original, screen_capture_detour);
	}
}

void GW::Render::SetResetCallback(std::function<void(IDirect3DDevice9* device)> callback) {
	reset_callback = callback;
	if (reset_hook.Empty()) {
		GwReset_t source = (GwReset_t)GW::Scanner::Find("\x55\x8B\xEC\x81\xEC\x98\x00\x00\x00\x53\x56\x57\x8B\xF1\x33\xD2", "xxxxxxxxxxxxxxxx", 0);
		printf("GwReset address = %p\n", source);
		reset_hook.Detour(source, reset_detour);
	}
}

void GW::Render::RestoreHooks() {
	endscene_hook.Retour();
	reset_hook.Retour();
	screen_capture_hook.Retour();
}
