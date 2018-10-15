#include "StateMachine.h"

#include "../GameManager.h"
#include "../Utils/DebugHelper.h"
#include "AStar.h"

#include <cassert>
#include <sstream>

NpcStateInfo::NpcStateInfo(const Npc& npc)
    :npc{ npc }, influenceZone{ npc.visionRange }
{}

const Movement StateMachine::Update(Npc& npc)
{
    assert(npcsStateInfo.find(npc.ID) != end(npcsStateInfo) && "Npc Explore logic was never defined");
    auto& npcExploreInfo = npcsStateInfo.at(npc.ID);
    Movement result = { HexDirection::CENTER, npc.hexID };

    std::stringstream ss;
    ss << "-----NPC-----" << std::endl;
    ss << "NpcID:" << npc.ID << "Path:";

    bool retry = false;
    do
    {
        switch (npcExploreInfo.objective)
        {
        case State::Explore:
            retry = npcExploreInfo.pathRecord.empty();// || npcExploreInfo.influenceZone.hasDiscoveredBetterInfluence();

            if (retry) {
                npcExploreInfo.objective = State::Init;
            }
            else {
                result = getNextValidMovement(npc.ID);

                auto hex = GameManager::getInstance().getMap().getHexByID(result.toHexID);
                if (hex.isType(HexType::TileAttribute_Goal))
                    npcExploreInfo.objective = State::OnGoal;
                else if (hex.ID == npc.hexID) {
                    GameManager::getInstance().getAIHelper().bb.setBestInfluenceHex(npcExploreInfo);
                    npcExploreInfo.pathRecord = AStar::FindBestPath(npcExploreInfo);
                }
            }
            break;

        case State::OnGoal:
            break;

        case State::GoToGoal:
            result = getNextValidMovement(npc.ID);
            {
                auto hex = GameManager::getInstance().getMap().getHexByID(result.toHexID);
                if (hex.isType(HexType::TileAttribute_Goal))
                    npcExploreInfo.objective = State::OnGoal;
            }
            retry = false;
            break;

        case State::Init:
            if (npcExploreInfo.npc.omniscient) {
                npcExploreInfo.influenceZone.currentHighest.hexID = GameManager::getInstance().getAIHelper().bb.getBestGoal(npc.ID);
                npcExploreInfo.objective = State::GoToGoal;
            }
            else {
                GameManager::getInstance().getAIHelper().bb.setBestInfluenceHex(npcExploreInfo);
                npcExploreInfo.objective = State::Explore;
            }           
            npcExploreInfo.pathRecord = AStar::FindBestPath(npcExploreInfo);          
            retry = true;
            break;
        }
    } while (retry);

    for (auto i : npcExploreInfo.pathRecord) {
        ss << i.toHexID << ' ';
    }

    ss << std::endl << " ToHexID:" << result.toHexID << " PathSize:" << npcExploreInfo.pathRecord.size();;
    DebugHelper::getInstance().Log(ss.str());

    return result;
}

const Movement StateMachine::getNextValidMovement(const unsigned int npcID) {
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