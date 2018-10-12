#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../GameObject/Npc.h"

#include <vector>
#include <map>

enum State { Init, Explore, ExploreOriented, OnGoal };

class StateInfo {
public:
    State objective = State::Init;
    std::vector<Movement> pathRecord;
    //InfluenceZone

    StateInfo() = default;
};

class StateMachine
{
public:
    std::map<unsigned int, StateInfo> npcsStateInfo;

    const Movement Update(Npc&);

    StateMachine() = default;
    ~StateMachine() = default;
private:
    const Movement getNextValidMovement(const unsigned int&);
};

#endif // !STATE_MACHINE_H