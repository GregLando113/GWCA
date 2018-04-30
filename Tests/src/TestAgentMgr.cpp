#include <GWCA\Managers\AgentMgr.h>

#include "catch.hpp"

TEST_CASE("AgentMgr") {
	REQUIRE(GW::Agents::GetPlayerId() > 0);
}
