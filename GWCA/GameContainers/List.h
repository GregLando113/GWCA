#ifndef _LIST_H_INC
#define _LIST_H_INC

namespace GW {

    template <typename T>
    struct TLink {

        bool IsLinked() const { return prevLink != this; }

        void Unlink() {
            RemoveFromList();

            nextNode = (T*)((size_t)this + 1);
            prevLink = this;
        }

        T *Prev() {
            T *prevNode = prevLink->prevLink->nextNode;
            if ((size_t)prevNode & 1)
                return NULL;
            return prevNode;
        }

        T *Next() {
            if ((size_t)nextNode & 1)
                return NULL;
            return nextNode;
        }

        TLink<T> *NextLink() {
            size_t offset = (size_t)this - ((size_t)prevLink->nextNode & ~1);
            return (TLink<T>*) ( ((size_t)nextNode & ~1) + offset );
        }

        TLink<T> *PrevLink() { return prevLink; }

    protected:
        TLink<T> *prevLink; // +h0000
        T        *nextNode; // +h0004

        void RemoveFromList () {
            NextLink()->m_prevLink = m_prevLink;
            m_prevLink->m_nextNode = m_nextNode;
        }
    };

    template <typename T>
    struct TList {

    protected:
        size_t   offset;
        TLink<T> link;
    };
}

#endif // _LIST_H_INC