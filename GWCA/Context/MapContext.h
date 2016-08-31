#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\gw_array.h>
#include <GWCA\GameEntities\Pathing.h>

namespace GW {

	class MapContext {
		MapContext() {}
		MapContext(const MapContext&) {}
	public:
		float mapboundaries[5];
		BYTE pad1[0x18];
		gw_array<void*> spawns1; // Seem to be arena spawns. struct is X,Y,unk 4 byte value,unk 4 byte value.
		gw_array<void*> spawns2; // Same as above
		gw_array<void*> spawns3; // Same as above
		float unk1[6]; // Some trapezoid i think.
		struct sub1 {
			struct sub2 {
				BYTE pad1[0x18];
				PathingMapArray pmaps;
			} *sub2;
			//... Bunch of arrays and shit
		} *sub1;
		//... Player coords and shit beyond this point if they are desirable :p
	};
}
