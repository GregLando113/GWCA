#pragma once

#include <Windows.h>

namespace GWCA {

	namespace GW {

		// class gw_array
		// generic game container seen throughout the games memory.
		// reccomended to only use references to maintain up-to-date information ( gw_array<T>& )

		template <typename T>
		class gw_array {
		public:

			enum class Exception {
				kOutOfBounds,   // index is beyond the current size of the array
				kInvalidArray   // array pointer is null
			};

			typedef T* iterator;
			typedef const T* const_iterator;

			iterator begin() { return array_; }
			const_iterator begin() const { return array_; }
			iterator end() { return array_ + current_size_; }
			const_iterator end() const { return array_ + current_size_; }

			T& index(DWORD index) {
				if (index >= size() || index < 0)
					throw Exception::kOutOfBounds;
				if (!valid())
					throw Exception::kInvalidArray;
				return array_[index];
			}

			T& operator[](DWORD index) {
				return this->index(index);
			}

			bool valid() {
				return array_ != nullptr;
			}

			DWORD size() const { return current_size_; }
			DWORD size_allocated() const { return allocated_size_; }

		protected:
			T* array_;              // + 0x0000
			DWORD allocated_size_;  // + 0x0004
			DWORD current_size_;    // + 0x0008
			DWORD default_size_;    // + 0x000C
		}; // Sz: 0x0010
	}
}
