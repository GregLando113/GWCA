#pragma once

#include <functional>

namespace GW
{
    struct HookEntry {};

    struct HookStatus {
        bool blocked = false;
        unsigned int altitude = 0;
    };

    template <typename ...Ts>
    using HookCallback = std::function<void (HookStatus *status, Ts...)>;
}
