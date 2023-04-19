#pragma once

namespace GW {
    
    class HookBase {
    protected:
        void* _detourFunc = nullptr;
        void* _retourFunc = nullptr;
        void* _sourceFunc = nullptr;

    public:
        static void Initialize();
        static void Deinitialize();
        
        // static void EnqueueHook(HookBase* base);
        // static void RemoveHook(HookBase* base);
        
        static void EnableHooks(void *target = NULL);
        static void DisableHooks(void *target = NULL);

        static int CreateHook(void *target, void *detour, void **trampoline);
        static void RemoveHook(void *target);

        static void EnterHook();
        static void LeaveHook();
        static int  GetInHookCount();
    };
    
    template <typename T>
    class THook : public HookBase {
    public:
        T Original() { return (T)_retourFunc; };
        bool Valid() { return _retourFunc != nullptr; };
        bool Empty() { return _retourFunc == nullptr; };
        
        T Detour(T source, T detour, const unsigned length = 0);
        T Retour(bool do_cleanup = true);
    };
    
    typedef THook<unsigned char *> Hook; // backward compatibility
}

template <typename T>
T GW::THook<T>::Detour(T source, T detour, const unsigned length) {
    if (Empty()) {
        _sourceFunc = (void*)source;
        _detourFunc = (void*)detour;
        HookBase::EnqueueHook(this);
    }
    return (T)_retourFunc;
}

template <typename T>
T GW::THook<T>::Retour(bool do_cleanup) {
    if (Valid()) {
        HookBase::RemoveHook(this);
    }
    return (T)_sourceFunc;
}
