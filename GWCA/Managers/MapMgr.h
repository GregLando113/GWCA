#pragma once

#include <Windows.h>

#include "GWCAManager.h"
#include <GWCA\Constants\Constants.h>
#include <GWCA\GameEntities\Map.h>
#include <GWCA\GameEntities\Pathing.h>

namespace GW {
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
		GW::Constants::MapID GetMapID();

		// Get current region you are in.
		int GetRegion();

		// Get current language you are in.
		int GetLanguage();

		// Get time, in ms, since the instance you are residing in has been created.
		DWORD GetInstanceTime();

		// Get the instance type (Outpost, Explorable or Loading)
		Constants::InstanceType GetInstanceType();

		// Travel to specified outpost.
		void Travel(Constants::MapID MapID, int District = 0, int Region = 0, int Language = 0);
		void Travel(Constants::MapID MapID, Constants::District district, int district_number = 0);

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
