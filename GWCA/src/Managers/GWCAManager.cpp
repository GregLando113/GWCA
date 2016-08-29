#include "GWCAManager.h"

#include "GWCA.h"

GWCA::GWCABaseManager::GWCABaseManager() {
	GWCA::Api::managers.push_back(this);
}
