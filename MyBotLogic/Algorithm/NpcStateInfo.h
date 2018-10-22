#pragma once
#include "../GameObject/Npc.h"
#include "InfluenceZone.h"

#include <vector>
#include <map>

enum State { Init, Explore, ExploreOriented, Blocked, OnGoal };

struct NpcStateInfo {
	Npc npc;
	State objective = State::Init;
	std::vector<Movement> pathRecord;
	InfluenceZone influenceZone;
	InfluenceHex currentHighest;

	NpcStateInfo() = default;
	NpcStateInfo(Npc&);

	~NpcStateInfo() = default;

};