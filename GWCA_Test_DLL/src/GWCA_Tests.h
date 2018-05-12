#pragma once

#include <time.h>
#include <stack>
#include <functional>

namespace GWCA_Tests {
	static std::stack<std::function<bool()>> tests;

	static void GWCA_Test(std::function<void()> f) {
		tests.push([f]() { f(); return true; });
	}

	static void WaitFrames(int n) {
		tests.push([num = n]() mutable { 
			return (num--) < 0; 
		});
	}

	static void WaitMilliseconds(int n) {
		tests.push([start = clock(), n]() mutable {
			return (clock() - start) < n; 
		});
	}

	static void WaitUntilTrue(std::function<bool()> f) {
		tests.push(f);
	}

	// returns true if done testing
	static bool OnRender() {
		if (tests.size() > 0) {
			if (tests.top()()) {
				tests.pop();
			}
		}
		return tests.size() == 0;
	}
}
