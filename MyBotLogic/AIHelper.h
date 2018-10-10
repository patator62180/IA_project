#ifndef AI_HELPER_H
#define AI_HELPER_H

#include "Globals.h"
#include "LevelInfo.h"
#include "TurnInfo.h"
#include "GameObject/Npc.h"
#include "Algorithm/GoalState.h"
#include "Algorithm/ExploreState.h"

#include <vector>
#include <set>
#include <map>

using GoalHexIDs = std::set<unsigned int>;

class AIHelper
{
private:
    std::map<unsigned int, Npc> npcs;

    GoalState goalState; // remove -> behaviour tree
    ExploreState exploreState; //remove -> behaviour tree
    GoalHexIDs goalHexIDs;
    std::set<unsigned int> npcsCurrentHexID;

public:
    AIHelper() = default;
    AIHelper(const LevelInfo&);

    void Update(const TurnInfo&);
    void FillActionList(std::vector<Action*>&);

    void SwitchExploreToGoal(const unsigned int&); // remove -> behaviour tree
    bool TryAddNpcCurrentHexID(const unsigned int&, const unsigned int);

    const std::set<unsigned int>& getNpcsCurrentHexID() noexcept;

    ~AIHelper() = default;

private:
    void setBestGoal(Npc&);
};

#endif // AI_HELPER_H