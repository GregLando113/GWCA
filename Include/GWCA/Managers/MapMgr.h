#pragma once

#include <GWCA/Utilities/Export.h>
#include <GWCA/GameContainers/Array.h>

namespace GW {

    struct AreaInfo;
    struct PathingMap;
    struct MissionMapIcon;

    typedef Array<PathingMap> PathingMapArray;
    typedef Array<MissionMapIcon> MissionMapIconArray;

    namespace Constants {
        enum class MapID;
        enum class District;
        enum class InstanceType;
    }

    struct Module;
    extern Module MapModule;

    namespace Map {

        GWCA_API bool GetIsMapLoaded();

        // Get current map ID.
        GWCA_API Constants::MapID GetMapID();

        // Get current region you are in.
        GWCA_API bool GetIsMapUnlocked(Constants::MapID map_id);

        // Get current region you are in.
        GWCA_API int GetRegion();

        // Get current language you are in.
        GWCA_API int GetLanguage();

        // Get whether current character is observing a match
        GWCA_API bool GetIsObserving();

        // Get the district number you are in.
        GWCA_API int GetDistrict();

        // Get time, in ms, since the instance you are residing in has been created.
        GWCA_API uint32_t GetInstanceTime();

        // Get the instance type (Outpost, Explorable or Loading)
        GWCA_API Constants::InstanceType GetInstanceType();

        // Travel to specified outpost.
        GWCA_API void Travel(Constants::MapID map_id,
            int district = 0, int region = 0, int language = 0);
        GWCA_API void Travel(Constants::MapID map_id,
            Constants::District district, int district_number = 0);

        // Returns array of icons (res shrines, quarries, traders, etc) on mission map.
        // Look at MissionMapIcon struct for more info.
        GWCA_API MissionMapIconArray GetMissionMapIconArray();

        // Returns pointer of collision trapezoid array.
        GWCA_API PathingMapArray GetPathingMap();

        GWCA_API uint32_t GetFoesKilled();
        GWCA_API uint32_t GetFoesToKill();

        GWCA_API AreaInfo *GetMapInfo(Constants::MapID map_id);

        inline AreaInfo *GetCurrentMapInfo() {
            Constants::MapID map_id = GetMapID();
            return GetMapInfo(map_id);
        }

        GWCA_API bool GetIsInCinematic(void);
        GWCA_API void SkipCinematic(void);

        GWCA_API void EnterChallenge();
    };
}
