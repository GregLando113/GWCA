#pragma once

#include <Windows.h>

namespace GW {
	namespace Packet {
		struct P037 { // Used to send pings and drawings in the minimap. Related to StoC::P133
			const DWORD header = 0x25;
			DWORD session_id = 0;	// unique for every player and shape. changes for every ping or shape.
			DWORD NumberPts;		// number of points in the following array
			struct {
				short x;	// in world coordinates divided by 100
				short y;	// same
			} points[8];
			DWORD unk[8];
		};
	}
}
