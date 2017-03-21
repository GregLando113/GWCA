#include "..\Utilities\Hooker.h"

extern "C" {
#include "..\..\Dependencies\disasm\ld32.h"
}

DWORD GW::HookInternal::CalculateDetourLength(BYTE* _source) {

	DWORD len = 0;
	DWORD current_op;

	do {
		current_op = length_disasm((void*)_source);
		if (current_op != 0) {
			len += current_op;
			_source += current_op;
		}
	} while (len < 5);

	return len;
}

BYTE* GW::HookInternal::Detour(BYTE* source, BYTE* detour, const DWORD length) {
	DWORD old_protection;

	BYTE* retour_func = (BYTE*)malloc(length + 5);
	VirtualProtect(retour_func, length + 5, PAGE_EXECUTE_READWRITE, &old_protection);

	memcpy(retour_func, source, length);

	retour_func += length;

	retour_func[0] = 0xE9;
	*(DWORD*)(retour_func + 1) = (DWORD)((source + length) - (retour_func + 5));

	VirtualProtect(source, length, PAGE_EXECUTE_READWRITE, &old_protection);

	source[0] = 0xE9;
	*(DWORD*)(source + 1) = (DWORD)(detour - (source + 5));

	if (length != 5)
		for (DWORD i = 5; i < length; i++)
			source[i] = 0x90;

	VirtualProtect(source, length, old_protection, &old_protection);

	retour_func -= length;

	return retour_func;
}

void GW::HookInternal::Retour(BYTE* source, BYTE* retour_func, DWORD length) {
	DWORD old_protection;
	VirtualProtect(source, length, PAGE_READWRITE, &old_protection);
	memcpy(source, retour_func, length);
	free(retour_func);
	VirtualProtect(source, length, old_protection, &old_protection);
}
