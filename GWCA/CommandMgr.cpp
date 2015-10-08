#include "CommandMgr.h"
#include "PatternScanner.h"

GWAPI::CommandMgr::CommandMgr(GWAPIMgr *parent) : parent_(parent)
{
	CreateHook((BYTE*)NewCmdCheck);
}

void GWAPI::CommandMgr::CreateHook(BYTE *detour)
{
	PatternScanner scanner(0x401000, 0x4FF000); // Can definitly be optimize but w/e
	DWORD origin = scanner.FindPattern("\x8B\xD1\x68\x8A\x00\x00\x00\x8D\x8D\xE8\xFE\xFF\xFF", "xxxxxxxxxxxxx", -0xC);
	DWORD length = Hook::CalculateDetourLength((BYTE*)origin);

	cmdCheck_ = (CommandCheck_t)hk_cmdCheck_.Detour((BYTE*)origin, detour, length);

	hooked_ = true;
}

void GWAPI::CommandMgr::RestoreHook()
{
	if (!hooked_) return;
	hk_cmdCheck_.Retour();

	hooked_ = false;
}

void GWAPI::CommandMgr::RegistrerKey(std::wstring key, CallBack_t callback, bool override)
{
	callbacks[key] = std::tuple<CallBack_t, bool>(callback, override);
}

void GWAPI::CommandMgr::DeleteKey(std::wstring key)
{
	callbacks.erase(key);
}

void __fastcall GWAPI::CommandMgr::NewCmdCheck(WCHAR *message)
{
	WCHAR channel = *message, *arguments = NULL;

	std::wstring key(++message);
	unsigned int fPos = key.find_first_of(' ');
	if (fPos != std::wstring::npos)
	{
		key = key.substr(0, fPos);
		arguments = message + fPos;
	}
	CommandMgr *command = GWAPI::GWAPIMgr::instance()->Command();
	std::tuple<CallBack_t, bool> callback = command->callbacks[key];
	if (std::get<0>(callback) && channel == '/')
	{
		std::get<0>(callback)(arguments);

		if (std::get<1>(callback))
			return command->cmdCheck_(L"");
	}

	return command->cmdCheck_(--message);
}
