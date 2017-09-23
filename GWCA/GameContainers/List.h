#ifndef _LIST_H_INC
#define _LIST_H_INC

namespace GW {

    template <typename T>
    struct TLink {

    protected:
        TLink<T> *prevLink; // +h0000
        T        *nextNode; // +h0004
        T        *self;     // +h0008
        DWORD     useless;  // +h000C
    };

    template <typename T>
    struct TList {

    protected:
        size_t   offset;
        TLink<T> link;
    };
}

#endif // _LIST_H_INC