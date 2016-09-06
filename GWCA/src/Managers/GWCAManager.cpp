#include <GWCA\Managers\GWCAManager.h>

#include <GWCA\GWCA.h>

GW::GWCABaseManager::GWCABaseManager() {
	GW::Api::managers.push_back(this);
}
