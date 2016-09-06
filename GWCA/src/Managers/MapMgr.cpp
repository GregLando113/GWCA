#include <GWCA\Managers\MapMgr.h>

#include <GWCA\Context\GameContext.h>
#include <GWCA\Context\MapContext.h>
#include <GWCA\Context\AgentContext.h>
#include <GWCA\Context\WorldContext.h>

#include <GWCA\Managers\CtoSMgr.h>
#include <GWCA\Managers\MemoryMgr.h>


bool GW::MapMgr::IsMapLoaded() {
	return GameContext::instance()->map != nullptr;
}

void GW::MapMgr::Travel(GW::Constants::MapID MapID,
	int District /*= 0*/, int Region /*= 0*/, int Language /*= 0*/) {

	static PAB_ZoneMap* pak = new PAB_ZoneMap();

	pak->mapid = static_cast<DWORD>(MapID);
	pak->district = District;
	pak->region = Region;
	pak->language = Language;
	pak->unk = 0;

	CtoSMgr::Instance().SendPacket<PAB_ZoneMap>(pak);
}

void GW::MapMgr::Travel(GW::Constants::MapID MapID, GW::Constants::District district, int district_number) {
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

DWORD GW::MapMgr::GetInstanceTime() {
	return GameContext::instance()->agent->instancetimer;
}

GW::Constants::MapID GW::MapMgr::GetMapID() {
	return static_cast<GW::Constants::MapID>(*(DWORD*)MemoryMgr::MapIDPtr);
}

int GW::MapMgr::GetRegion() { 
	return *(int*)(MemoryMgr::MapInfoPtr + 0x10); 
}

int GW::MapMgr::GetLanguage() {
	return *(int*)(MemoryMgr::MapInfoPtr + 0xC); 
}

GW::Constants::InstanceType GW::MapMgr::GetInstanceType() {
	return *(GW::Constants::InstanceType*)(MemoryMgr::agArrayPtr - 0xF0);
}

GW::MissionMapIconArray GW::MapMgr::GetMissionMapIconArray() {
	return GameContext::instance()->world->missionmapicons;
}

GW::PathingMapArray GW::MapMgr::GetPathingMap() {
	return GameContext::instance()->map->sub1->sub2->pmaps;
}
