#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../GameObject/Npc.h"
#include "InfluenceZone.h"
#include "NpcStateInfo.h"

#include <vector>
#include <map>


class StateMachine
{
public:
    std::map<unsigned int, NpcStateInfo> npcsStateInfo;

    Movement Update(Npc&);

    StateMachine() = default;
    ~StateMachine() = default;
};

#endif // !STATE_MACHINE_H