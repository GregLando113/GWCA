#include <Windows.h>
#include <time.h>

#include <GWCA\GWCA.h>
#include <GWCA\Utilities\Hooker.h>
#include <GWCA\Managers\Render.h>

#include "GWCA_Tests.h"


DWORD WINAPI init(HMODULE hModule);

// Dll Entry point
BOOL WINAPI DllMain(_In_ HMODULE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved) {
	DisableThreadLibraryCalls(_HDllHandle);
	if (_Reason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, _HDllHandle, 0, 0);
	}
	return TRUE;
}

// Thread entry point
DWORD WINAPI init(HMODULE hModule) {
	FILE* logfile = stdout;
	AllocConsole();
	FILE* fh;
	freopen_s(&fh, "CONOUT$", "w", stdout);
	freopen_s(&fh, "CONOUT$", "w", stderr);
	SetConsoleTitle("GWCA_Tests");
	// Should we also write to a log file?

	GW::HookBase::Initialize();
	
	if (!GW::Initialize()) {
		// can't even start testing, rip
		FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
		return EXIT_SUCCESS; // can we really call this a success, though ?
	}

	GW::Render::SetResetCallback([](IDirect3DDevice9* device) {
		// we don't actually do anything here, as it will never
		// happen during testing. Still, it's good to set this, 
		// so we can notice a possible crash-issue
	});

	bool done = false;
	std::stack<std::function<bool()>> tests;
	// add a test case
	auto GWCA_Test = [&tests](std::function<void()> f) {
		tests.push([f]() { f(); return true; });
	};
	// wait for n frames
	auto WaitFrames = [&tests](int n) {
		auto lambda = [num = n]() mutable { return (num--) < 0; };
		tests.push(lambda);
	};
	// wait for n milliseconds
	auto WaitMilliseconds = [&tests](int n) {
		auto lambda = [start = clock(), n]() mutable { return (clock() - start) < n; };
		tests.push(lambda);
	};
	// wait until func returns true
	auto WaitUntilTrue = [&tests](std::function<bool()> func) {
		tests.push(func);
	};

	// add all tests here

	GW::Render::SetRenderCallback([&done, &tests](IDirect3DDevice9* device) {
		GW::HookBase::EnableHooks(); // do we need this? we have it in toolbox...
		if (tests.size() > 0) {
			if (tests.top()()) {
				tests.pop();
			}
		} else {
			GW::Render::RestoreHooks();
			GW::Terminate();
			done = true;
		}
	});
	GW::HookBase::EnableHooks(); // do we need this? we have it in toolbox...

	while (!done) {
		Sleep(100);

		// this provides an early exit by pressing VK_END, however
		// it will stop and unload the dll while tests are running, 
		// so the chances of a crash are pretty high
		if (GetAsyncKeyState(VK_END) & 1) done = true;
	}

	FreeConsole();

	GW::HookBase::Deinitialize();

	FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}
