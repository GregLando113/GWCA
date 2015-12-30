#pragma once

#include <Windows.h>

namespace GWAPI {
	
	class MemoryPatcher {
	public:
		MemoryPatcher(LPVOID addr, BYTE *patch, UINT size);
		~MemoryPatcher();

		bool TooglePatch(bool enable);
		bool TooglePatch() { TooglePatch(!enable); };
	private:
		LPVOID addr;

		BYTE *patch;
		BYTE *backup;
		UINT size;

		bool enable;
	};

}
