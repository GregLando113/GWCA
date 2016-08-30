#pragma once
#include <Windows.h>

#include "..\GameEntities\Party.h"

namespace GWCA {
using namespace GW;
class PartyContext
{
public:
  char pad_0x0000[0x4]; //0x0000
  gw_array<void*> unk1_arr; //0x0004
  class PartyState { //0x0014
    DWORD state;
  public:
    bool InHardMode(){ return (state & 0x10) > 0; }
    bool IsDefeated(){ return (state & 0x20) > 0; }
  }partystate;
  char pad_0x0018[0x10]; //0x0018
  DWORD invitecount; // 0x0028
  char pad_0x002C[0x28]; // 0x002C
  PartyInfo* partyinfo; //0x0054
};//Size=0x0058
}
