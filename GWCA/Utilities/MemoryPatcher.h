#pragma once

#include <Windows.h>

namespace GWCA {
	
	class MemoryPatcher {
	public:
		MemoryPatcher(LPVOID addr, BYTE *patch, UINT size);
		~MemoryPatcher();

		bool TooglePatch(bool flag);
		bool TooglePatch() { TooglePatch(!flag); };

		bool GetPatchState() { return flag; };
	private:
		LPVOID addr;

		BYTE *patch;
		BYTE *backup;
		UINT size;

		bool flag;
	};

}
