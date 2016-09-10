#pragma once

#include <vector>
#include <functional>

#include "GWCAManager.h"
#include <GWCA\Utilities\Hooker.h>

namespace GW {

	// Shoutouts to DarthTon @ unknowncheats.me for this class.

	class GameThreadMgr : public GWCAManager<GameThreadMgr> {
		friend class GWCAManager<GameThreadMgr>;
		friend class Api;

	public:

		// For use only in gameloop hook.
		void __stdcall CallFunctions();

		void Enqueue(std::function<void()> f) {
			EnterCriticalSection(&criticalsection_);
			calls_.emplace_back(f);
			LeaveCriticalSection(&criticalsection_);
		}

		// i think this is better
		void AddPermanentCall(std::function<void()> f) {
			EnterCriticalSection(&criticalsection_);
			calls_permanent_.emplace_back(f);
			LeaveCriticalSection(&criticalsection_);
		}

		
		static void __fastcall gameLoopHook(void*);
		static void renderHook();
		void ToggleRenderHook();

	private:

		GameThreadMgr();
		void RestoreHooks() override;

		std::vector<std::function<void(void)> > calls_;
		std::vector<std::function<void(void)> > calls_permanent_;
		static CRITICAL_SECTION criticalsection_;
		bool render_state_;
		Hook hk_game_thread_;
	};
}
