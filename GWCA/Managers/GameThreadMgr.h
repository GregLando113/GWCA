#pragma once

#include <functional>

#include <GWCA\Utilities\Hooker.h>

namespace GW {

	// Shoutouts to DarthTon @ unknowncheats.me for this class.
	namespace GameThread {
		void Initialize();
		void ClearCalls();
		void RestoreHooks();

		void Enqueue(std::function<void()> f);

		DWORD AddPermanentCall(std::function<void()> f);

		void RemovePermanentCall(DWORD identifier);
		
		static void renderHook();
		void ToggleRenderHook();
	};
}
