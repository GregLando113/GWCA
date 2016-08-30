#pragma once

#include <Windows.h>

#include "GWCAManager.h"

#include "..\Constants\Constants.h"
#include "..\Structures\GameEntities\Map.h"
#include "..\Structures\GameEntities\Pathing.h"


namespace GWCA {
	class MapMgr : public GWCAManager<MapMgr> {
		friend class GWCAManager<MapMgr>;

		struct PAB_ZoneMap {
			const DWORD header = 0xAB;
			DWORD mapid;
			int region;
			int district;
			int language;
			DWORD unk;
		};
	public:
		bool IsMapLoaded();

		// Get current map ID.
		GwConstants::MapID GetMapID();

		// Get current region you are in.
		int GetRegion();

		// Get current language you are in.
		int GetLanguage();

		// Get time, in ms, since the instance you are residing in has been created.
		DWORD GetInstanceTime();

		// Get the instance type (Outpost, Explorable or Loading)
		GwConstants::InstanceType GetInstanceType();

		// Travel to specified outpost.
		void Travel(GwConstants::MapID MapID, int District = 0, int Region = 0, int Language = 0);
		void Travel(GwConstants::MapID MapID, GwConstants::District district, int district_number = 0);

		// Returns array of icons (res shrines, quarries, traders, etc) on mission map.
		// Look at MissionMapIcon struct for more info.
		GW::MissionMapIconArray GetMissionMapIconArray();

		// Returns pointer of collision trapezoid array.
		GW::PathingMapArray GetPathingMap();

	private:
		MapMgr() {}
		void RestoreHooks() override {}
	};
}
