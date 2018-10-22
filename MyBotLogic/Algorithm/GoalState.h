#ifndef GOAL_STATE_H
#define GOAL_STATE_H

#include "../GameObject/Npc.h"

#include <vector>
#include <map>

enum GoalObjective { Goal_Init, Goal_InProgress, Goal_Achieved };

struct GoalInfo {
    unsigned int npcID;
    unsigned int goalHexID;
    GoalObjective subObjective = GoalObjective::Goal_Init;
    std::vector<Movement> pathRecord;

    GoalInfo() = default;
    GoalInfo(const unsigned int npcID, const unsigned int goalHexID)
        : npcID{ npcID }, goalHexID{ goalHexID }
    {}
};

class GoalState
{
public:
    std::map<unsigned int, GoalInfo> npcsGoalInfo;

    const HexDirection Update(Npc&);

    GoalState() = default;
    ~GoalState() = default;
private:
    const HexDirection getNextValidMovement(Npc&);
};

#endif // GOAL_STATE_H