#pragma once

#include <assert.h>
#include <cstdint>



namespace GW {

    // class Array
    // generic game container seen throughout the games memory.
    // reccomended to only use references to maintain up-to-date information ( Array<T>& )

    template <typename T>
    class Array {
    public:
        typedef       T* iterator;
        typedef const T* const_iterator;

        iterator       begin()       { return m_buffer; }
        const_iterator begin() const { return m_buffer; }
        iterator       end()         { return m_buffer + m_size; }
        const_iterator end()   const { return m_buffer + m_size; }

        Array()
            : m_buffer(nullptr)
            , m_capacity(0)
            , m_size(0)
            , m_param(0)
        {}

        T& at(uint32_t index) {
            assert(m_buffer && index < m_size);
            return m_buffer[index];
        }

        const T& at(uint32_t index) const {
            assert(m_buffer && index < m_size);
            return m_buffer[index];
        }

        T& operator[](uint32_t index) {
            return at(index);
        }

        const T& operator[](uint32_t index) const {
            assert(m_buffer && index < m_size);
            return m_buffer[index];
        }

        bool valid() const { return m_buffer != nullptr; }
        void clear() { m_size = 0; }

        uint32_t size()     const { return m_size; }
        uint32_t capacity() const { return m_capacity; }

        T& find_first(bool* found, bool(*cmpfn)(T&))
        {
            for (uint32_t i = 0; i < m_size; ++i) {
                if (cmpfn(m_buffer[i])) {
                    return m_buffer[i];
                }
            }
        }

    public:
        T*       m_buffer;    // +h0000
        uint32_t m_capacity;  // +h0004
        uint32_t m_size;      // +h0008
        uint32_t m_param;     // +h000C
    }; // Size: 0x0010
}
