#include <GWCA\Managers\MapMgr.h>

#include <GWCA\Utilities\Scanner.h>
#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\MapContext.h>
#include <GWCA\Context\AgentContext.h>
#include <GWCA\Context\WorldContext.h>

#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MemoryMgr.h>

bool GW::Map::IsMapLoaded() {
	return GameContext::instance()->map != nullptr;
}

void GW::Map::Travel(GW::Constants::MapID MapID,
	int District /*= 0*/, int Region /*= 0*/, int Language /*= 0*/) {

	if (GetInstanceType() == GW::Constants::InstanceType::Loading) return;

	static PAB_ZoneMap* pak = new PAB_ZoneMap();

	pak->mapid = static_cast<DWORD>(MapID);
	pak->district = District;
	pak->region = Region;
	pak->language = Language;
	pak->unk = 0;

	CtoS::SendPacket<PAB_ZoneMap>(pak);
}

void GW::Map::Travel(GW::Constants::MapID MapID, GW::Constants::District district, int district_number) {
	switch (district) {
	case Constants::District::Current:
		Travel(MapID, district_number, GetRegion(), GetLanguage());
		break;
	case Constants::District::International:
		Travel(MapID, district_number, Constants::Region::International, 0);
		break;
	case Constants::District::American:
		Travel(MapID, district_number, Constants::Region::America, 0);
		break;
	case Constants::District::EuropeEnglish:
		Travel(MapID, district_number, Constants::Region::Europe, Constants::EuropeLanguage::English);
		break;
	case Constants::District::EuropeFrench:
		Travel(MapID, district_number, Constants::Region::Europe, Constants::EuropeLanguage::French);
		break;
	case Constants::District::EuropeGerman:
		Travel(MapID, district_number, Constants::Region::Europe, Constants::EuropeLanguage::German);
		break;
	case Constants::District::EuropeItalian:
		Travel(MapID, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Italian);
		break;
	case Constants::District::EuropeSpanish:
		Travel(MapID, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Spanish);
		break;
	case Constants::District::EuropePolish:
		Travel(MapID, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Polish);
		break;
	case Constants::District::EuropeRussian:
		Travel(MapID, district_number, Constants::Region::Europe, Constants::EuropeLanguage::Russian);
		break;
	case Constants::District::AsiaKorean:
		Travel(MapID, district_number, Constants::Region::Korea, 0);
		break;
	case Constants::District::AsiaChinese:
		Travel(MapID, district_number, Constants::Region::China, 0);
		break;
	case Constants::District::AsiaJapanese:
		Travel(MapID, district_number, Constants::Region::Japan, 0);
		break;
	}
}

DWORD GW::Map::GetInstanceTime() {
	return GameContext::instance()->agent->instancetimer;
}

GW::Constants::MapID GW::Map::GetMapID() {
	static DWORD* mapid_ptr = nullptr;
	if (mapid_ptr == nullptr) {
		// For Map IDs
		BYTE* addr = (BYTE*)Scanner::Find("\xB0\x7F\x8D\x55", "xxxx", 0);
		printf("MapIDPtr = %p\n", addr);
		if (addr) {
			mapid_ptr = *(DWORD**)(addr + 0x46);
		}
	}
	return (GW::Constants::MapID)(*mapid_ptr);
}

namespace {
	BYTE* GetMapInfoPtr() {
		static BYTE* MapInfoPtr = nullptr;
		if (MapInfoPtr == nullptr) {
			MapInfoPtr = (BYTE*)GW::Scanner::Find("\xC3\x8B\x75\xFC\x8B\x04\xB5", "xxxxxxx", 0);
			if (MapInfoPtr) {
				printf("MapInfoPtr = %p\n", MapInfoPtr);
				MapInfoPtr = *(BYTE**)(MapInfoPtr + 7);
			}
		}
		return MapInfoPtr;
	}
}

int GW::Map::GetRegion() {
	return *(int*)(GetMapInfoPtr() + 0x10);
}

int GW::Map::GetLanguage() {
	return *(int*)(GetMapInfoPtr() + 0xC);
}

GW::Constants::InstanceType GW::Map::GetInstanceType() {
	return *(GW::Constants::InstanceType*)(MemoryMgr::agArrayPtr - 0xF0);
}

GW::MissionMapIconArray GW::Map::GetMissionMapIconArray() {
	return GameContext::instance()->world->missionmapicons;
}

GW::PathingMapArray GW::Map::GetPathingMap() {
	return GameContext::instance()->map->sub1->sub2->pmaps;
}

DWORD GW::Map::GetFoesKilled() {
	return GameContext::instance()->world->foes_killed;
}

DWORD GW::Map::GetFoesToKill() {
	return GameContext::instance()->world->foes_to_kill;
}

GW::AreaInfo& GW::Map::GetMapInfo(Constants::MapID MapID) {
	static GW::AreaInfo *AreasInfo;
	if (!AreasInfo) {
		GW::AreaInfo **tmp = (GW::AreaInfo**)GW::Scanner::Find("\x8B\xC6\xC1\xE0\x05\x2B\xC6\x5E\x8D\x04", "xxxxxxxxxx", 11);
		assert(tmp);
		AreasInfo = *tmp;
	}
	assert(AreasInfo);
	return AreasInfo[(DWORD)MapID];
}
