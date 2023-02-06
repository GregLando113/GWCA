#pragma once

#include <GWCA/Utilities/Hook.h>
#include <GWCA/Utilities/Export.h>

#ifndef EXCEPT_EXPRESSION_LOOP
    #define EXCEPT_EXPRESSION_LOOP EXCEPTION_CONTINUE_SEARCH
#endif
namespace GW {

    struct Module;
    extern Module GameThreadModule;

    namespace GameThread {
        GWCA_API void ClearCalls();

        void Enqueue(std::function<void ()> f);

        typedef HookCallback<> GameThreadCallback;
        GWCA_API void RegisterGameThreadCallback(
            HookEntry *entry,
            const GameThreadCallback& callback);

        GWCA_API void RemoveGameThreadCallback(
            HookEntry *entry);

        GWCA_API bool IsInGameThread();
    };
}
