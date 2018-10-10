#ifndef EXPLORE_STATE_H
#define EXPLORE_STATE_H

#include "../GameObject/Npc.h"

#include <vector>
#include <map>

enum ExploreObjective { ExploreInit, FindGoal, FoundGoal };

class ExploreInfo {
public:
    unsigned int npcID;
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

    const HexDirection Update(Npc*);

    ExploreState() = default;
    ~ExploreState() = default;
private:
    const HexDirection getNextValidMovement(const unsigned int&);
};

#endif // !EXPLORE_STATE_H