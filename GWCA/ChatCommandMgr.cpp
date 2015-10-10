#include "ChatCommandMgr.h"
#include "PatternScanner.h"

GWAPI::ChatCommandMgr::ChatCommandMgr(GWAPIMgr *parent) : parent_(parent)
{
	CreateHook((BYTE*)NewCmdCheck);
}

void GWAPI::ChatCommandMgr::CreateHook(BYTE *detour)
{
	PatternScanner scanner(0x401000, 0x4FF000); // Can definitly be optimize but w/e
	DWORD origin = scanner.FindPattern("\x8B\xD1\x68\x8A\x00\x00\x00\x8D\x8D\xE8\xFE\xFF\xFF", "xxxxxxxxxxxxx", -0xC);
	DWORD length = Hook::CalculateDetourLength((BYTE*)origin);

	cmdCheck_ = (CommandCheck_t)hk_cmdCheck_.Detour((BYTE*)origin, detour, length);

	hooked_ = true;
}

void GWAPI::ChatCommandMgr::RestoreHook()
{
	if (!hooked_) return;
	hk_cmdCheck_.Retour();

	hooked_ = false;
}

void GWAPI::ChatCommandMgr::RegisterKey(std::wstring key, std::function<void(std::wstring)> callback, bool override)
{
	callbacks[key] = std::tuple<CallBack_t, bool>(callback, override);
}

void GWAPI::ChatCommandMgr::DeleteKey(std::wstring key)
{
	callbacks.erase(key);
}

void __fastcall GWAPI::ChatCommandMgr::NewCmdCheck(WCHAR *message)
{
	WCHAR channel = *message;

	std::wstring key(message + 1);
	std::wstring arguments(L"");
	unsigned int fPos = key.find_first_of(' ');
	if (fPos != std::wstring::npos)
	{
		key = key.substr(0, fPos);
		arguments = std::wstring(message + fPos + 2);
	}
	ChatCommandMgr *command = GWAPI::GWAPIMgr::instance()->ChatCommands();
	std::tuple<CallBack_t, bool> callback = command->callbacks[key];
	if (std::get<0>(callback) && channel == '/')
	{
		std::get<0>(callback)(arguments);

		if (std::get<1>(callback))
			return command->cmdCheck_(L"");
	}

	return command->cmdCheck_(message);
}
