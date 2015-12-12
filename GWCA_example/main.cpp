#include <Windows.h>

#include "../GWCA/GWCA.h"

void PrintCoords() {

	// Grab API object. Always statically allocate this as a local variable.
	// While this object is allocated, you have ownership of the api.
	// All other threads will wait for this function to complete if trying to access GWCA
	GWAPI::GWCA api;

	// Get Player Agent Structure.
	GWAPI::GW::Agent* player = api().Agents().GetPlayer();

	// Print coords.
	printf("Player: %f %f", player->pos.x, player->pos.y);
}

int main() {
	// create console. You can delete this (and the FreeConsole) but then you won't have access to printf.
	AllocConsole();
	FILE* fh;
	freopen_s(&fh, "CONOUT$", "w", stdout);
	freopen_s(&fh, "CONOUT$", "w", stderr);
	SetConsoleTitleA("GWCA++ Debug Console");

	// Initialize API, exit out if it failed.
	if (!GWAPI::GWCA::Initialize()) {
		FreeConsole();
		return 0;
	}
	
	PrintCoords();

	FreeConsole();
}

// Do all your startup things here instead.
void init(HMODULE hModule) {
	if (*(DWORD*)0x00DE0000 != NULL) {
		MessageBoxA(0, "Error: Guild Wars already injected!", "GWCA++ Example", 0);
		FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
	}

	main();

	GWAPI::GWCA::Destruct();
	FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

// DLL entry point, not safe to stay in this thread for long.
BOOL WINAPI DllMain(_In_ HMODULE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved) {
	if (_Reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(_HDllHandle);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, _HDllHandle, 0, 0);
	}
	return TRUE;
}
