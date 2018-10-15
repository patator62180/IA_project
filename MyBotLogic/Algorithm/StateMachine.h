#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../GameObject/Npc.h"
#include "InfluenceZone.h"

#include <vector>
#include <map>

enum State { Init, Explore, GoToGoal, OnGoal };

struct NpcStateInfo {
    Npc npc;
    State objective = State::Init;
    std::vector<Movement> pathRecord;
    InfluenceZone influenceZone;

    NpcStateInfo(const Npc&);
};

class StateMachine
{
public:
    std::map<unsigned int, NpcStateInfo> npcsStateInfo;

    const Movement Update(Npc&);

    StateMachine() = default;
    ~StateMachine() = default;
private:
    const Movement getNextValidMovement(const unsigned int);
};

#endif // !STATE_MACHINE_H