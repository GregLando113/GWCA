#pragma once
#include <Windows.h>
#include "..\GameContainers\gw_array.h"

namespace GWCA {
  namespace GW {

    struct PathingTrapezoid {
      DWORD ID;
      PathingTrapezoid* Adjacent[4];
      DWORD idekwhyisthisherebutok;
      float XTL;
      float XTR;
      float YT;
      float XBL;
      float XBR;
      float YB;
    };

    struct PathingMap {
      DWORD zplane; // ground plane = UINT_MAX, rest 0 based index
      DWORD unkafterzplane;
      void* unk1ptr;
      DWORD unk1size;
      void* unk2ptr;
      DWORD unk2size;
      PathingTrapezoid* trapezoids;
      DWORD trapezoidcount;
      DWORD restunknown[0xD];
    };

    using PathingMapArray = gw_array<PathingMap>;
    
  }
}
