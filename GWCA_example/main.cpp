#include <Windows.h>

#include "../GWCA/GWCA.h"

#include "../GWCA/Managers/AgentMgr.h"

void PrintCoords() {

	// Get Player Agent Structure.
	GWCA::GW::Agent* player = GWCA::Agents().GetPlayer();

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


	
	PrintCoords();

	FreeConsole();
}

// Do all your startup things here instead.
void init(HMODULE hModule) {


	main();

	GWCA::Api::Destruct();
	FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
}

// DLL entry point, not safe to stay in this thread for long. 
// Make sure what you do is non-blocking and only initialize the api. Either hook a function or spawn a thread after to do iterative work.
BOOL WINAPI DllMain(_In_ HMODULE _HDllHandle, _In_ DWORD _Reason, _In_opt_ LPVOID _Reserved) {
	if (_Reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(_HDllHandle);

		if (*(DWORD*)0x00DE0000 != NULL) {
			MessageBoxA(0, "Error: Guild Wars already injected!", "GWCA++ Example", 0);
			FreeLibraryAndExitThread(_HDllHandle, EXIT_SUCCESS);
		}

		// Initialize API, exit out if it failed.
		if (!GWCA::Api::Initialize()) {
			return FALSE;
		}


		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, _HDllHandle, 0, 0);
	}
	return TRUE;
}
