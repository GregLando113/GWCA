#include "stdafx.h"

#include <GWCA/Context/PreGameContext.h>
#include <GWCA/Managers/Module.h>
#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Scanner.h>

#include <GWCA/Managers/PreGameMgr.h>

namespace
{
	using namespace GW;

	// Getting the context from base address requires at least six offsets that are hard to find.
	// Because of this, we hook 'HandleEvent' and get the context from its first parameter.
	CharacterSelectionContext* character_selection_ctx = nullptr;

	CharacterSelectionContext* GetCharacterSelectionContext()
	{
		if (character_selection_ctx) return character_selection_ctx;

		// The DLL was injected while the client was in the character selection screen.
		// Because of this, the 'HandleEvent' hook was not yet invoked.
		// The following code is a fallback that provides the context from memory.

		auto pre_game_context = PreGameContext::instance();
		if (!pre_game_context) return nullptr;

		auto character_array = &pre_game_context->chars;
		if (!character_array->valid()) return nullptr;

		uint32_t number_of_characters = 0;
		for (auto& character : *character_array)
		{
			auto length = wcslen(character.character_name);
			if (length > 0) number_of_characters++;
		}

		uint32_t ctx_parts[5] = {
			number_of_characters,
			0xFFFFFFFF,
			0x00000000,
			0x00000000,
			0x00000001
		};

		auto address = Scanner::FindOnMemoryPages({ MEM_COMMIT, MEM_PRIVATE, PAGE_READWRITE }, (uint8_t*)&ctx_parts, sizeof(ctx_parts));
		if (!address) return nullptr;

		character_selection_ctx = (CharacterSelectionContext*)(address - 8);
		return character_selection_ctx;
	}

	typedef void(__fastcall* HandleEvent_Ptr)(CharacterSelectionContext* context, void* unused, void* event_ptr);
	HandleEvent_Ptr HandleEvent_Func = NULL;
	HandleEvent_Ptr HandleEvent_Ret = NULL;

	void __fastcall OnHandleEvent(CharacterSelectionContext* context, void* unused, void* event_ptr)
	{
		HookBase::EnterHook(); 
		character_selection_ctx = context;
		HandleEvent_Ret(context, unused, event_ptr);
		HookBase::LeaveHook();
	}

	typedef void(__fastcall* HandleError_Ptr)(char* message, char* path, uint32_t code);
	HandleError_Ptr HandleError_Func = NULL;
	HandleError_Ptr HandleError_Ret = NULL;

	bool ignore_errors = false;

	void __fastcall OnHandleError(char* message, char* path, uint32_t code)
	{
		HookBase::EnterHook();
		if (!ignore_errors) HandleError_Ret(message, path, code);
		HookBase::LeaveHook();
	}

	typedef uintptr_t(*GetLinkedFrameId_Ptr)(uint32_t frame_index, uint32_t unk1_hex1);
	GetLinkedFrameId_Ptr GetLinkedFrameId_Func = NULL;

	typedef void(*FillLoginBuffer_Ptr)(uint32_t frame_id, uint32_t unk1_hex42E, wchar_t* login_buffer, uint32_t unk2_hex0);
	FillLoginBuffer_Ptr FillLoginBuffer_Func = NULL;

	typedef void(__cdecl* LoginAccount_Ptr)(uint32_t frame_index, wchar_t* email, wchar_t* password, wchar_t* unk1_strEmpty, uint32_t unk2_hex40);
	LoginAccount_Ptr LoginAccount_Func = NULL;

	typedef int(__thiscall* SelectCharacter_Ptr)(CharacterSelectionContext* context, wchar_t* character_name, uint32_t unk1_hex1);
	SelectCharacter_Ptr SelectCharacter_Func = NULL;

	typedef void(__fastcall* PlayCharacter_Ptr)(CharacterSelectionContext* context);
	PlayCharacter_Ptr PlayCharacter_Func = NULL;

	FrameArray* frame_array = nullptr;
	CharacterArray* character_array = nullptr;

	void Init()
	{
		HandleEvent_Func = (HandleEvent_Ptr)Scanner::Find("\x8B\xD9\x83\x7E\x08\x00\x75\x00\x83\x7E\x04\x00", "xxxxx?x?xxx?", -21);
		HookBase::CreateHook(HandleEvent_Func, OnHandleEvent, (void**)&HandleEvent_Ret);

		HandleError_Func = (HandleError_Ptr)Scanner::Find("\x89\x45\xFC\x89\x55\xEC\x89\x4D\xE4", "xxxxxxxxx", -13);
		HookBase::CreateHook(HandleError_Func, OnHandleError, (void**)&HandleError_Ret);

		uintptr_t address = NULL;

		address = GW::Scanner::Find("\xE8\x00\x00\x00\x00\x83\xC4\x24\x5E\x8B\x4D\xFC", "x????xxxxxxx", -36);
		GetLinkedFrameId_Func = (GetLinkedFrameId_Ptr)Scanner::FunctionFromNearCall(address);
		FillLoginBuffer_Func = (FillLoginBuffer_Ptr)Scanner::FunctionFromNearCall(address + 9);

		address = Scanner::Find("\xE8\x00\x00\x00\x00\x83\xC4\x24\x5E\x8B\x4D\xFC", "x????xxxxxxx", 0);
		LoginAccount_Func = (LoginAccount_Ptr)Scanner::FunctionFromNearCall(address);

		address = GW::Scanner::Find("\x8B\xCB\x6A\x01\xFF\x36\xE8\x00\x00\x00\x00", "xxxxxxx????", 6);
		SelectCharacter_Func = (SelectCharacter_Ptr)Scanner::FunctionFromNearCall(address);

		address = GW::Scanner::Find("\x83\x7E\x08\x06\x0F\x85\x00\x00\x00\x00\x8B\xCB\xE8\x00\x00\x00\x00", "xxxxxx????xxx????", 12);
		PlayCharacter_Func = (PlayCharacter_Ptr)Scanner::FunctionFromNearCall(address);

		address = GW::Scanner::Find("\x56\x8b\x37\x3b\x35\x00\x00\x00\x00", "xxxxx????", 12);
		frame_array = (FrameArray*)*(uintptr_t*)address;

		address = GW::Scanner::Find("\x89\x35\x00\x00\x00\x00\x3B\xC7", "xx????xx", 2);
		character_array = (CharacterArray*)*(uintptr_t*)address;

		GWCA_INFO("[SCAN] HandleEvent = %p\n", HandleEvent_Func);
		GWCA_INFO("[SCAN] HandleEvent = %p\n", HandleError_Func);
		GWCA_INFO("[SCAN] GetLinkedFrameId = %p\n", GetLinkedFrameId_Func);
		GWCA_INFO("[SCAN] FillLoginBuffer = %p\n", FillLoginBuffer_Func);
		GWCA_INFO("[SCAN] LoginAccount = %p\n", LoginAccount_Func);
		GWCA_INFO("[SCAN] SelectCharacter = %p\n", SelectCharacter_Func);
		GWCA_INFO("[SCAN] PlayCharacter = %p\n", PlayCharacter_Func);
		GWCA_INFO("[SCAN] FrameArray = %p\n", frame_array);
		GWCA_INFO("[SCAN] CharacterArray = %p\n", character_array);

#if _DEBUG
		GWCA_ASSERT(HandleEvent_Func);
		GWCA_ASSERT(HandleError_Func);
		GWCA_ASSERT(GetLinkedFrameId_Func);
		GWCA_ASSERT(FillLoginBuffer_Func);
		GWCA_ASSERT(LoginAccount_Func);
		GWCA_ASSERT(SelectCharacter_Func);
		GWCA_ASSERT(PlayCharacter_Func);
		GWCA_ASSERT(frame_array);
		GWCA_ASSERT(character_array);
#endif
	}
}

namespace GW
{
	Module PreGameModule = {
		"PreGameModule",    // name
		NULL,               // param
		::Init,             // init_module
		NULL,               // exit_module
		NULL,               // enable_hooks
		NULL,               // disable_hooks
	};

	namespace PreGameMgr
	{
		bool IsLoggedOut()
		{
			auto pre_game_context = PreGameContext::instance();
			return pre_game_context && !character_array->valid();
		}

		bool IsSelectingCharacter()
		{
			auto pre_game_context = PreGameContext::instance();
			return pre_game_context && character_array->valid();
		}

		bool IsPlayingCharacter()
		{
			auto pre_game_context = PreGameContext::instance();
			return !pre_game_context && character_array->valid();
		}

		void LoginAccount(wchar_t* email, wchar_t* password)
		{
			static const size_t FILL_COUNT = 512;
			static const wchar_t FILL_CHAR = '?';

			if (!IsLoggedOut()) return;

			int32_t frame_index = -1;
			for (auto frame : *frame_array)
			{
				frame_index++;

				if (!frame) continue;

				auto linked_frame_id = GetLinkedFrameId_Func(frame_index, 0x1);
				if (!linked_frame_id) continue;

				auto login_buffer = std::wstring(FILL_COUNT, FILL_CHAR);

				FillLoginBuffer_Func(linked_frame_id, 0x42E, login_buffer.data(), 0x0);

				auto fill_chars_remaining = std::count(login_buffer.begin(), login_buffer.end(), FILL_CHAR);
				if (fill_chars_remaining < FILL_COUNT) break;
			}

			LoginAccount_Func(frame_index, email, password, L"", 0x40);
		}

		void LogoutAccount()
		{
			if (IsLoggedOut()) return;

			// Logout from character selection would cause a crash. Error: 'Model closed while in render queue'
			ignore_errors = true;

			UI::LogoutParam logoutParam = { 1, 0 };
			UI::SendUIMessage(UI::UIMessage::kLogout, &logoutParam);

			ignore_errors = false;
		}

		CharacterArray* GetCharacterArray()
		{
			if (IsLoggedOut()) return nullptr;
			return character_array;
		}

		void SelectCharacter(wchar_t* character_name)
		{
			if (!IsSelectingCharacter()) return;

			auto context = GetCharacterSelectionContext();
			if (!context) return;

			SelectCharacter_Func(context, character_name, 0x1);
		}

		void PlayCharacter()
		{
			if (!IsSelectingCharacter()) return;

			auto context = GetCharacterSelectionContext();
			if (!context) return;

			PlayCharacter_Func(context);
		}

		void ChangeCharacter()
		{
			if (!IsPlayingCharacter()) return;

			UI::LogoutParam logoutParam = { 1, 1 };
			UI::SendUIMessage(UI::UIMessage::kLogout, &logoutParam);
		}
	}
}
