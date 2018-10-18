#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

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

class StateMachine
{
public:
    std::map<unsigned int, NpcStateInfo> npcsStateInfo;

    Movement Update(Npc&);

    StateMachine() = default;
    ~StateMachine() = default;
};

#endif // !STATE_MACHINE_H