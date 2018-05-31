#ifndef _ARRAY_H_INC
#define _ARRAY_H_INC

#include <Windows.h>
#include <assert.h>

namespace GW {

    // class Array
    // generic game container seen throughout the games memory.
    // reccomended to only use references to maintain up-to-date information ( Array<T>& )

    template <typename T>
    class Array {
    public:
        enum class Exception {
            kOutOfBounds,   // index is beyond the current size of the array
            kInvalidArray   // array pointer is null
        };

        typedef       T* iterator;
        typedef const T* const_iterator;

        iterator       begin()       { return _array; }
        const_iterator begin() const { return _array; }
        iterator       end()         { return _array + _size; }
        const_iterator end()   const { return _array + _size; }

		Array()
			: _array(nullptr)
			, _capacity(0)
			, _size(0)
			, _growth(0)
		{}

        T& at(DWORD index) {
			assert(_array && index < _size);
            return _array[index];
        }

        T& operator[](DWORD index) {
            return at(index);
        }

        bool valid() { return _array != nullptr; }
		void clear() { _size = 0; }

        DWORD size()     const { return _size; }
        DWORD capacity() const { return _capacity; }

		T& find_first(bool* found, bool(*cmpfn)(T&))
		{
			for (DWORD i = 0; i < _size; ++i) {
				if (cmpfn(_array[i])) {
					return _array[i];
				}
			}
		}

    protected:
        T*    _array;       // +h0000
        DWORD _capacity;   // +h0004
        DWORD _size;       // +h0008
        DWORD _growth;     // +h000C
    }; // Size: 0x0010
}

#endif // _ARRAY_H_INC
