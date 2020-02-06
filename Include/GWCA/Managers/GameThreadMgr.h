#pragma once

#include <GWCA/Utilities/Export.h>

#ifndef EXCEPT_EXPRESSION_LOOP
    #define EXCEPT_EXPRESSION_LOOP EXCEPTION_CONTINUE_SEARCH
#endif
namespace GW {

    struct Module;
    extern Module GameThreadModule;

    namespace GameThread {
        GWCA_API void ClearCalls();

        GWCA_API void Enqueue(std::function<void ()> f);

        GWCA_API uint32_t AddPermanentCall(std::function<void ()> f);

        GWCA_API void RemovePermanentCall(uint32_t identifier);
    };
}
