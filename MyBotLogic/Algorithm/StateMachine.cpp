#include "StateMachine.h"

#include "../GameManager.h"
#include "../Utils/DebugHelper.h"
#include "AStar.h"

#include <cassert>
#include <sstream>

const Movement StateMachine::Update(Npc& npc)
{
    assert(npcsStateInfo.find(npc.ID) != end(npcsStateInfo) && "Npc Explore logic was never defined");
    auto& npcExploreInfo = npcsStateInfo.at(npc.ID);
    Movement result = { HexDirection::CENTER, npc.hexID };

    bool retry = false;
    do
    {
        switch (npcExploreInfo.objective)
        {
        case State::Explore:
            retry = npcExploreInfo.pathRecord.empty();

            if (retry) {
                npcExploreInfo.objective = State::Init;
            }
            else {
                result = getNextValidMovement(npc.ID);

                auto hex = GameManager::getInstance().getMap().getConstHexByID(result.toHexID);
                if(hex.isGoal())
                    npcExploreInfo.objective = State::OnGoal;
                }
            break;

        case State::OnGoal:
            break;

        case State::Init:
            auto hexIDToGo = GameManager::getInstance().getAIHelper().bb.getBestHexIDToGo(npc);
            npcExploreInfo.pathRecord = AStar::FindBestExplorePath(npc, hexIDToGo);
            npcExploreInfo.objective = State::Explore;
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

const Movement StateMachine::getNextValidMovement(const unsigned int& npcID) {
    assert(npcsStateInfo.find(npcID) != end(npcsStateInfo) && "Npc explore logic was never defined");
    auto& npcStateInfo = npcsStateInfo.at(npcID);

    assert(!npcStateInfo.pathRecord.empty() && "Npc explore path record is empty, no available move known");
    auto nextMovement = npcStateInfo.pathRecord.back();

    if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npcID, nextMovement.toHexID)) {
        nextMovement = npcStateInfo.pathRecord.back();
        npcStateInfo.pathRecord.pop_back();
    }
    else {
        nextMovement.direction = HexDirection::CENTER;
    }

    return nextMovement;
}