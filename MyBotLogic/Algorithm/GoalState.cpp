#include "GoalState.h"

#include "../GameManager.h"
#include "AStar.h"

#include <cassert>

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

            retry = npcGoalInfo.pathRecord.empty();

            if (retry) {
                npcGoalInfo.subObjective = GoalObjective::Goal_Init;
            }
            else {
                result = getNextValidMovement(npc);             
            }

            break;

        case GoalObjective::Goal_Achieved:
            break;

        case GoalObjective::Goal_Init:
            npcGoalInfo.pathRecord = AStar::FindBestPath(npc, npcGoalInfo.goalHexID);
            npcGoalInfo.subObjective = GoalObjective::Goal_InProgress;
            retry = true;
            break;
        }
    } while (retry);

    return result;
}

// TODO Copy/pasta from explorestate getnextvalidmovement
const HexDirection GoalState::getNextValidMovement(Npc& npc) {
    assert(npcsGoalInfo.find(npc.ID) != end(npcsGoalInfo) && "Npc goal logic was never defined");
    auto& npcAIInfo = npcsGoalInfo.at(npc.ID);

    assert(!npcAIInfo.pathRecord.empty() && "Npc goal path record is empty, no available move known");
    auto resultingMovHexID = npcAIInfo.pathRecord.back().toHexID;
    HexDirection nextDirection = HexDirection::CENTER;

    if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npc.ID, resultingMovHexID)) {
        nextDirection = npcAIInfo.pathRecord.back().direction;
        npcAIInfo.pathRecord.pop_back();

        if (npcAIInfo.goalHexID == resultingMovHexID)
            npcAIInfo.subObjective = GoalObjective::Goal_Achieved;
    }

    return nextDirection;
}