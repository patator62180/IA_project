#include "GoalState.h"

#include "../GameManager.h"
#include "AStar.h"

#include <cassert>

const Movement GoalState::Update(Npc& npc)
{
    assert(npcsGoalInfo.find(npc.ID) != end(npcsGoalInfo) && "Npc goal logic was never defined");
    auto& npcGoalInfo = npcsGoalInfo.at(npc.ID);
    Movement result = { HexDirection::CENTER, npc.hexID };

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
const Movement GoalState::getNextValidMovement(Npc& npc) {
    assert(npcsGoalInfo.find(npc.ID) != end(npcsGoalInfo) && "Npc explore logic was never defined");
    auto& npcAIInfo = npcsGoalInfo.at(npc.ID);

    assert(!npcAIInfo.pathRecord.empty() && "Npc explore path record is empty, no available move known");
    auto nextMovement = npcAIInfo.pathRecord.back();

    if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npc.ID, nextMovement.toHexID)) {
        nextMovement = npcAIInfo.pathRecord.back();
        npcAIInfo.pathRecord.pop_back();

        if (npcAIInfo.goalHexID == nextMovement.toHexID)
            npcAIInfo.subObjective = GoalObjective::Goal_Achieved;
    }
    else {
        nextMovement.direction = HexDirection::CENTER;
    }

    return nextMovement;
}