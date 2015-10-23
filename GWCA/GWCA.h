#pragma once

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
