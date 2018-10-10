#include "ExploreState.h"

#include "../GameManager.h"
#include "AStar.h"

#include <cassert>

const HexDirection ExploreState::Update(Npc* npc)
{
    assert(npcsExploreInfo.find(npc->ID) != end(npcsExploreInfo) && "Npc Explore logic was never defined");
    auto& npcExploreInfo = npcsExploreInfo.at(npc->ID);
    HexDirection result = HexDirection::CENTER;

    bool retry = false;
    do
    {
        switch (npcExploreInfo.subObjective)
        {
        case ExploreObjective::FindGoal:

            retry = false;
            break;

        case ExploreObjective::FoundGoal:
            GameManager::getInstance().getAIHelper().SwitchExploreToGoal(npcExploreInfo.npcID);
            break;

        case ExploreObjective::ExploreInit:
            //  npcExploreInfo.pathRecord = AStar::getBestExplorePath(npc);
            npcExploreInfo.subObjective = ExploreObjective::FindGoal;
            retry = true;
            break;
        }
    } while (retry);

    return result;
}

/*
const HexDirection GoalState::Update(Npc& npc)
{
assert(npcsGoalInfo.find(npc.ID) != end(npcsGoalInfo) && "Npc goal logic was never defined");
auto& npcGoalInfo = npcsGoalInfo.at(npc.ID);
HexDirection result = HexDirection::CENTER;

bool retry = false;
do
{
switch (npcGoalInfo.subObjective)
{
case GoalObjective::Goal_InProgress:
result = getNextValidMovement(npc.ID);
retry = false;
break;

case GoalObjective::Goal_Achieved:
break;

case GoalObjective::Goal_Init:
npcGoalInfo.pathRecord = AStar::getBestPath(npc, npcGoalInfo.goalHexID);
npcGoalInfo.subObjective = GoalObjective::Goal_InProgress;
retry = true;
break;
}
} while (retry);

return result;
}

*/
//

// TODO Copy/pasta from goalstate getnextvalidmovement
const HexDirection ExploreState::getNextValidMovement(const unsigned int& npcID) {
    assert(npcsExploreInfo.find(npcID) != end(npcsExploreInfo) && "Npc explore logic was never defined");
    auto npcAIInfo = npcsExploreInfo.at(npcID);

    assert(!npcAIInfo.pathRecord.empty() && "Npc explore path record is empty, no available move known");
    auto resultingMovHexID = npcAIInfo.pathRecord.back().toHexID;
    HexDirection nextDirection = HexDirection::CENTER;

    if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npcID, resultingMovHexID)) {
        nextDirection = npcAIInfo.pathRecord.back().direction;
        npcAIInfo.pathRecord.pop_back();
    }

    return nextDirection;
}