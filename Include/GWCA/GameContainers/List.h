#pragma once

namespace GW {

    template <typename T>
    struct TLink {

        bool IsLinked() const { return prev_link != this; }

        void Unlink() {
            RemoveFromList();

            next_node = (T*)((size_t)this + 1);
            prev_link = this;
        }

        T *Prev() {
            T *prevNode = prev_link->prev_link->next_node;
            if ((size_t)prevNode & 1)
                return NULL;
            return prevNode;
        }

        T *Next() {
            if ((size_t)next_node & 1)
                return NULL;
            return next_node;
        }

        TLink<T> *NextLink() {
            size_t offset = (size_t)this - ((size_t)prev_link->next_node & ~1);
            return (TLink<T>*) ( ((size_t)next_node & ~1) + offset );
        }

        TLink<T> *PrevLink() { return prev_link; }

    protected:
        TLink<T> *prev_link; // +h0000
        T        *next_node; // +h0004

        void RemoveFromList() {
            NextLink()->prev_link = prev_link;
			prev_link->next_node = next_node;
        }
    };

    template <typename T>
    struct TList {
        TLink<T>* Get() {
            return &link;
        }
    protected:
        size_t   offset;
        TLink<T> link;
    };
}
