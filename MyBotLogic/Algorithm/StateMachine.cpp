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
    auto& npcStateInfo = npcsStateInfo.at(npc.ID);
    Movement result = { HexDirection::CENTER, npc.hexID };

    std::stringstream ss;
    ss << "-----NPC-----" << std::endl;
    ss << "NpcID:" << npc.ID << "Path:";

    bool retry = false;
    do
    {
        switch (npcStateInfo.objective)
        {
        case State::Explore:
            if (npcStateInfo.pathRecord.empty()) {
                if (GameManager::getInstance().getMap().getHexByID(npc.hexID).isType(HexType::TileAttribute_Goal))
                {                 
                    npcStateInfo.objective = State::OnGoal;
                    retry = false;
                }
                else
                {             
                    npcStateInfo.objective = State::Init;
                    retry = true;
                }
            }
            else {
                auto temp = getNextValidMovement(npc.ID);

                if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npc.ID, temp.toHexID)) {
                    result = temp;
                    npcStateInfo.pathRecord.pop_back();
                }                 
                else 
                    npcStateInfo.objective = State::Blocked;
                
                retry = false;
            }
            break;

        case State::Blocked:
            auto temp = getNextValidMovement(npc.ID);

            if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npc.ID, temp.toHexID)) {
                result = temp;
                npcStateInfo.pathRecord.pop_back();
                npcStateInfo.objective = State::Explore;
                retry = false;
            }
            else {
                npcStateInfo.objective = State::Init;
                retry = true;
            }
               
            break;

        case State::OnGoal:
            break;

        case State::Init:
            if (npcStateInfo.npc.omniscient)
                npcStateInfo.influenceZone.currentHighest.hexID = GameManager::getInstance().getAIHelper().bb.getBestGoal(npc.ID);
            else 
                GameManager::getInstance().getAIHelper().bb.setBestInfluenceHex(npcStateInfo);    

            npcStateInfo.objective = State::Explore;
            npcStateInfo.pathRecord = AStar::FindBestPath(npcStateInfo);          
            retry = true;
            break;
        }
    } while (retry);

    for (auto i : npcStateInfo.pathRecord) {
        ss << i.toHexID << ' ';
    }

    ss << std::endl << " ToHexID:" << result.toHexID << " PathSize:" << npcStateInfo.pathRecord.size();;
    DebugHelper::getInstance().Log(ss.str());

    return result;
}

const Movement StateMachine::getNextValidMovement(const unsigned int npcID) {
    auto& npcStateInfo = npcsStateInfo.at(npcID);

    assert(!npcStateInfo.pathRecord.empty() && "Npc explore path record is empty, no available move known");
    auto nextMovement = npcStateInfo.pathRecord.back();

    ////CHECK IF HEX IS AVAILABLE
    //if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npcID, nextMovement.toHexID)) {
    //    nextMovement = npcStateInfo.pathRecord.back();
    //    npcStateInfo.pathRecord.pop_back();
    //}
    //else {
    //    nextMovement.direction = HexDirection::CENTER;
    //}

    return nextMovement;
}