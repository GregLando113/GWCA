#include "PatternScanner.h"

#include <stdint.h>
#include <Psapi.h>
#include <immintrin.h>


/* 
	Credits to DarthTon @ unknowncheats.me for this FindPattern Implementation.
	See http://www.unknowncheats.me/forum/c-and-c/125497-findpattern-benchmark.html
*/

const void* GWAPI::PatternScanner::FindPattern(const char* pattern,const char* mask,const DWORD end_offset)
{
	struct PartData
	{
		int32_t mask;
		__m128i needle;
	};

	const uint8_t* result = nullptr;
	auto len = strlen(mask);
	auto first = strchr(mask, '?');
	size_t len2 = (first != nullptr) ? (first - mask) : len;
	auto firstlen = min(len2, 16);
	intptr_t num_parts = (len < 16 || len % 16) ? (len / 16 + 1) : (len / 16);
	PartData parts[4];
	memset(parts, 0, 4 * sizeof(PartData));

	for (intptr_t i = 0; i < num_parts; ++i, len -= 16)
	{
		for (size_t j = 0; j < min(len, 16) - 1; ++j)
			if (mask[16 * i + j] == 'x')
				_bittestandset((LONG*)&parts[i].mask, j);

		parts[i].needle = _mm_loadu_si128((const __m128i*)(pattern + i * 16));
	}

#pragma omp parallel for
	for (intptr_t i = 0; i < static_cast<intptr_t>(size_) / 32 - 1; ++i)
	{
		auto block = _mm256_loadu_si256((const __m256i*)base_ + i);
		if (_mm256_testz_si256(block, block))
			continue;

		auto offset = _mm_cmpestri(parts->needle, firstlen, _mm_loadu_si128((const __m128i*)(base_ + i * 32)), 16, _SIDD_CMP_EQUAL_ORDERED);
		if (offset == 16)
		{
			offset += _mm_cmpestri(parts->needle, firstlen, _mm_loadu_si128((const __m128i*)(base_ + i * 32 + 16)), 16, _SIDD_CMP_EQUAL_ORDERED);
			if (offset == 32)
				continue;
		}

		for (intptr_t j = 0; j < num_parts; ++j)
		{
			auto hay = _mm_loadu_si128((const __m128i*)(base_ + (2 * i + j) * 16 + offset));
			auto bitmask = _mm_movemask_epi8(_mm_cmpeq_epi8(hay, parts[j].needle));
			if ((bitmask & parts[j].mask) != parts[j].mask)
				goto next;
		}

		result = base_ + 32 * i + offset + end_offset;
		break;

	next:;
	}

	return result;
}

GWAPI::PatternScanner::PatternScanner(HMODULE _module)
{
	MODULEINFO info;
	if (!GetModuleInformation(GetCurrentProcess(), _module, &info, sizeof(MODULEINFO)))
		throw 1;

	base_ = (const BYTE*)info.lpBaseOfDll;
	size_ = (DWORD)info.SizeOfImage;
}

GWAPI::PatternScanner::PatternScanner(char* moduleName /*= NULL*/)
{
	MODULEINFO info;
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(moduleName), &info, sizeof(MODULEINFO)))
		throw 1;

	base_ = (const BYTE*)info.lpBaseOfDll;
	size_ = (const DWORD)info.SizeOfImage;
}

GWAPI::PatternScanner::PatternScanner(DWORD _start, DWORD _size) : base_((const BYTE*)_start), size_(_size)
{

}
