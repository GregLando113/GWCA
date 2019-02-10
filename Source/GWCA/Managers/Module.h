#pragma once

namespace GW {
    struct Module {
        const char  *name;
        void        *param;

        void       (*init_module)();
        void       (*exit_module)();

        void       (*create_hooks)();
        void       (*remove_hooks)();
    };
}
