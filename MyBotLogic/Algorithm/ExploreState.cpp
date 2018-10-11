#include "ExploreState.h"

#include "../GameManager.h"
#include "../Utils/DebugHelper.h"
#include "AStar.h"

#include <cassert>
#include <sstream>

const Movement ExploreState::Update(Npc& npc)
{
    assert(npcsExploreInfo.find(npc.ID) != end(npcsExploreInfo) && "Npc Explore logic was never defined");
    auto& npcExploreInfo = npcsExploreInfo.at(npc.ID);
    Movement result = { HexDirection::CENTER, npc.hexID };

    bool retry = false;
    do
    {
        switch (npcExploreInfo.subObjective)
        {
        case ExploreObjective::FindGoal:
            retry = npcExploreInfo.pathRecord.empty();

            if (retry) {
                npcExploreInfo.subObjective = ExploreObjective::ExploreInit;
            }
            else {
                result = getNextValidMovement(npc.ID);

                auto hex = GameManager::getInstance().getMap().getConstHexByID(result.toHexID);
                if(hex.isGoal())
                    npcExploreInfo.subObjective = ExploreObjective::FoundGoal;
            }

            break;

        case ExploreObjective::FoundGoal:
            //GameManager::getInstance().getAIHelper().SwitchExploreToGoal(npcExploreInfo.npcID);
            break;

        case ExploreObjective::ExploreInit:
            npcExploreInfo.hexIDToGo = GameManager::getInstance().getAIHelper().bb.getBestHexIDToGo(npc);
            npcExploreInfo.pathRecord = AStar::FindBestExplorePath(npc, npcExploreInfo.hexIDToGo);
            npcExploreInfo.subObjective = ExploreObjective::FindGoal;
            retry = true;
            break;
        }
    } while (retry);

    std::stringstream ss;
    ss << "-----NPC-----" << std::endl;
    ss << "NpcID:" << npc.ID << " ToHexID:" << result.toHexID << " PathSize:" << npcExploreInfo.pathRecord.size();;
    DebugHelper::getInstance().Log(ss.str());

    return result;
}

// TODO Copy/pasta from goalstate getnextvalidmovement
const Movement ExploreState::getNextValidMovement(const unsigned int& npcID) {
    assert(npcsExploreInfo.find(npcID) != end(npcsExploreInfo) && "Npc explore logic was never defined");
    auto& npcAIInfo = npcsExploreInfo.at(npcID);

    assert(!npcAIInfo.pathRecord.empty() && "Npc explore path record is empty, no available move known");
    auto nextMovement = npcAIInfo.pathRecord.back();

    if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npcID, nextMovement.toHexID)) {
        nextMovement = npcAIInfo.pathRecord.back();
        npcAIInfo.pathRecord.pop_back();
    }
    else {
        nextMovement.direction = HexDirection::CENTER;
    }

    return nextMovement;
}