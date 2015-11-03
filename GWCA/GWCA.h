#pragma once

// this file is used to provide solution-specific defines to the library, 
// add it to your solution directory and leave it empty or define stuff in it (e.g. GWAPI_USEDIRECTX)
// it is included here to force inclusion by other projects 
// (it is already a forced global include in GWCA project)
#include "GWCA_defines.h"


/*
Class used for GWCA initialization, access and destruction

// start by calling
GWCA::Initialize()

// then access through GWCA object
GWCA api;
api().Agent().GetPlayer()

// or directly through static call
GWCA::Api().Agent().GetPlayer()

// Finally destroy GWCA
GWCA::Destruct()

*/

#include "GWAPIMgr.h"
#include "MemoryMgr.h"

namespace GWAPI {
	class GWCA {
	public:
		static bool Initialize();
		static void Destruct();

		static GWAPIMgr& Api() { return *api_; }

		/* Simple accessor */
		GWCA() {}

		/* Destructor */
		~GWCA() {}

		/* Module Accessor */
		GWAPIMgr& operator () () const { return *api_; }

	private:
		static GWAPIMgr* api_;
	};
}
