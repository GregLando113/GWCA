#pragma once

#include <Windows.h>
#include <stdio.h>

namespace GWCA {

	struct MemoryMgr{

		// Agent shit
		static BYTE* agArrayPtr;
		static BYTE* PlayerAgentIDPtr;
		static BYTE* TargetAgentIDPtr;
		static BYTE* MouseOverAgentIDPtr;

		// Map Id
		static BYTE* MapIDPtr;


		// Send Packet Shit
		static BYTE* GSObjectPtr;
		static BYTE* CtoGSSendFunction;

		// Base ptr to get context ptr for gameworld
		static BYTE* BasePointerLocation;

		// Disable/Enable Rendering
		static BYTE* RenderLoopLocation;

		// For gamethread calls
		static BYTE* GameLoopLocation;
		static BYTE* GameLoopReturn;
		static BYTE* GameLoopRestore;

		// For writing PM's in chat.
		static BYTE* WriteChatFunction;

		// Skill timer for effects.
		static BYTE* SkillTimerPtr;
		
		// To extract alcohol level.
		static BYTE* PostProcessEffectFunction;

		// To change target.

		static BYTE* ChangeTargetFunction;

		// Skill structure array.

		static BYTE* SkillArray;
		static BYTE* UseSkillFunction;

		// For move calls that dont glitch your char around.
		static BYTE* MoveFunction;

		static BYTE* WinHandlePtr;

		static BYTE* MapInfoPtr;

		static BYTE* DialogFunc;

		static BYTE* GStoCBase;

		// Basics
		static bool Scan();
		template <typename T> static T ReadPtrChain(DWORD _base,DWORD _amount_of_offsets,...)
		{
			va_list vl;

			va_start(vl, _amount_of_offsets);
			while (_amount_of_offsets--)
			{
				_base = (*(DWORD*)_base);
					if (_base){
						_base += va_arg(vl, DWORD);
					}
					else{
						return NULL;
					}
			}
			va_end(vl);

			return (T)_base;
		}

		// Memory Reads.
		inline static DWORD GetContextPtr(){ return (*(DWORD*)BasePointerLocation) + 0x18; }
		inline static DWORD GetSkillTimer(){ return GetTickCount() + *(DWORD*)SkillTimerPtr; }
		inline static HWND GetGWWindowHandle(){ return *(HWND*)WinHandlePtr; }
		inline static DWORD GetGSObject() { return **(DWORD**)MemoryMgr::GSObjectPtr; }
	};
}