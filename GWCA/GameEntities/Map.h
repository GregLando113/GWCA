#pragma once

#include <Windows.h>

#include <GWCA\GameContainers\Array.h>

namespace GW {
	struct MissionMapIcon { // MapOverlay from GWCA
		long index;
		float X;
		float Y;
		long unknown1; // = 0
		long unknown2; // = 0
		long option; // Affilitation/color. Enum { 0 = gray, blue, red, yellow, teal, purple, green, gray };
		long unknown3; // = 0
		long modelId; // Model of the displayed icon in the Minimap
		long unknown4; // = 0
		void* unknown5; // May concern the name
	};

	using MissionMapIconArray = Array<MissionMapIcon>;
}
