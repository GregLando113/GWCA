#ifndef _GADGET_CONTEXT_INC
#define _GADGET_CONTEXT_INC

#include <Windows.h>
#include <GWCA\GameContainers\Array.h>

namespace GW {
    struct GadgetInfo {
        /* +h0000 */ DWORD h0000;
        /* +h0004 */ DWORD h0004;
        /* +h0008 */ DWORD h0008;
        /* +h000C */ wchar *NameString;
    };

    struct GadgetContext {
        /* +h0000 */ Array<GadgetInfo> GadgetInfo;
        // ...
    };
}

#endif // _GADGET_CONTEXT_INC