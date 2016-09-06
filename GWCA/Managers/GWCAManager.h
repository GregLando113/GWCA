#pragma once

#include <vector>

namespace GW {

	class GWCABaseManager {
		friend class Api;

	public:
		virtual ~GWCABaseManager() {}

	protected:
		GWCABaseManager();

	private:
		virtual void RestoreHooks() {};
	};

	template <class T>
	class GWCAManager : GWCABaseManager {
	public:
		inline static T& Instance() { return *InstancePtr(); }
		static T* InstancePtr() {
			if (instance_ == nullptr) {
				instance_ = new T();
			}
			return instance_;
		}

	private:
		static T* instance_;
	};

	template <class T>
	T* GWCAManager<T>::instance_ = nullptr;
}
