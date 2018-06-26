#ifndef CINEMATIC_H_INC
#define CINEMATIC_H_INC

#include <Windows.h>

struct Cinematic {
	DWORD h0000;
	DWORD h0004; // pointer to data
	// ...
};

#endif // CINEMATIC_H_INC