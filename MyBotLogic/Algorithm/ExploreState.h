#ifndef EXPLORE_STATE_H
#define EXPLORE_STATE_H

#include "../GameObject/Npc.h"

#include <vector>
#include <map>

enum ExploreObjective { ExploreInit, FindGoal, FoundGoal };

class ExploreInfo {
public:
    unsigned int npcID;
    unsigned int hexIDToGo;
    ExploreObjective subObjective = ExploreObjective::ExploreInit;
    std::vector<Movement> pathRecord;

    ExploreInfo() = default;
    ExploreInfo(const unsigned int npcID)
        : npcID{ npcID }
    {}
};

class ExploreState
{
public:
    std::map<unsigned int, ExploreInfo> npcsExploreInfo;

    const Movement Update(Npc&);

    ExploreState() = default;
    ~ExploreState() = default;
private:
    const Movement getNextValidMovement(const unsigned int&);
};

#endif // !EXPLORE_STATE_H