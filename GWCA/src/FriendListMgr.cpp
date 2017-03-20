#include <GWCA\Managers\FriendListMgr.h>
#include <GWCA\Utilities\Scanner.h>

GW::FriendList* GW::FriendListMgr::GetFriendList() {
	static GW::FriendList* flist = nullptr;
	if (flist == nullptr) {
		/*
		All function that directly use the adresse of friend list are really small or make no sense to scan.
		So we scan a function that call one of this function then follow by transforming rel adr to abs adr.
		*/
		DWORD call = Scanner::Find("\x85\xC0\x74\x19\x6A\xFF\x8D\x50\x08\x8D\x4E\x08", "xxxxxxxxxxxx", -0x18);
		call = *(DWORD*)call + (call + 5 - 1) + 2;
		flist = &(*(GW::FriendList*)*(DWORD*)call);
	}
	return flist;
}

void GW::FriendListMgr::SetFriendListStatus(GW::Constants::OnlineStatus status) {
	typedef void(__fastcall *SetOnlineStatus_t)(DWORD status);
	static SetOnlineStatus_t setstatus_func = nullptr;
	if (setstatus_func == nullptr) {
		setstatus_func = (SetOnlineStatus_t)Scanner::Find("\x8B\xCE\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xC8", "xxxx????x????xx", -0x1C);
	}
	setstatus_func((DWORD)status);
}
