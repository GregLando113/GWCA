#pragma once

#include <Windows.h>

namespace GW {

	namespace HookInternal {
		BYTE* Detour(BYTE* source, BYTE* detour, DWORD length);
		void Retour(BYTE* source, BYTE* retour_func, DWORD length);
		DWORD CalculateDetourLength(BYTE* _source);
	}

	template <typename T>
	class THook {
		T retour_func_ = nullptr;
		T source_ = nullptr;
		DWORD length_ = 0;
	public:
		T Original() { return (T)retour_func_; };
		bool Valid() { return retour_func_ != nullptr; };
		bool Empty() { return retour_func_ == nullptr; };

		T Detour(T _source, T _detour, const DWORD _length = 0) {
			if (Empty()) {
				source_ = _source;
				length_ = (_length > 0 ? _length : HookInternal::CalculateDetourLength((BYTE*)_source));
				retour_func_ = (T)HookInternal::Detour((BYTE*)source_, (BYTE*)_detour, length_);
			}
			return retour_func_;
		}

		T Retour(bool do_cleanup = true) {
			if (Valid()) {
				HookInternal::Retour((BYTE*)source_, (BYTE*)retour_func_, length_);
				retour_func_ = nullptr;
			}
			return source_;
		}
	};

	typedef THook<BYTE*> Hook; // backward compatibility
}
