#pragma once
#ifdef GWAPI_USEDIRECTX

#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Hooker.h"
#include "GWCAManager.h"

namespace GWCA {

	class DirectXMgr : public GWCAManager<DirectXMgr> {
		friend class GWCAManager<DirectXMgr>;

	public:
		typedef HRESULT(WINAPI *EndScene_t)(IDirect3DDevice9* pDevice);
		typedef HRESULT(WINAPI *Reset_t)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

		void CreateRenderHooks(EndScene_t _endscene, Reset_t _reset);

		inline EndScene_t EndsceneReturn() { return endscene_; }
		inline Reset_t ResetReturn() { return reset_; }

	private:
		DirectXMgr();
		void RestoreHooks() override;

		EndScene_t endscene_ = NULL;
		Reset_t reset_ = NULL;

		Hook hk_endscene_;
		Hook hk_reset_;

		bool hooked_ = false;

		DWORD* vtable_start_ = 0;
	};

}

#endif