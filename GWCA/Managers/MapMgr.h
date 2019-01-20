#pragma once

namespace GW {
    namespace Map {

        GWCA_API bool IsMapLoaded();

        // Get current map ID.
        GWCA_API GW::Constants::MapID GetMapID();

        // Get current region you are in.
        GWCA_API int GetRegion();

        // Get current language you are in.
        GWCA_API int GetLanguage();

        // Get the district number you are in.
        GWCA_API int GetDistrict();

        // Get time, in ms, since the instance you are residing in has been created.
        GWCA_API DWORD GetInstanceTime();

        // Get the instance type (Outpost, Explorable or Loading)
        GWCA_API Constants::InstanceType GetInstanceType();

        // Travel to specified outpost.
        GWCA_API void Travel(Constants::MapID MapID, int District = 0, int Region = 0, int Language = 0);
        GWCA_API void Travel(Constants::MapID MapID, Constants::District district, int district_number = 0);

        // Returns array of icons (res shrines, quarries, traders, etc) on mission map.
        // Look at MissionMapIcon struct for more info.
        GWCA_API GW::MissionMapIconArray GetMissionMapIconArray();

        // Returns pointer of collision trapezoid array.
        GWCA_API GW::PathingMapArray GetPathingMap();

        GWCA_API DWORD GetFoesKilled();
        GWCA_API DWORD GetFoesToKill();

        GWCA_API GW::AreaInfo& GetMapInfo(Constants::MapID MapID);

        inline GW::AreaInfo& GetCurrentMapInfo() {
            Constants::MapID map_id = GetMapID();
            return GetMapInfo(map_id);
        }

        GWCA_API bool GetIsInCinematic(void);
        GWCA_API void SkipCinematic(void);
    };
}
