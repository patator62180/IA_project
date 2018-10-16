#include "StateMachine.h"

#include "../GameManager.h"
#include "../Utils/DebugHelper.h"
#include "AStar.h"

#include <cassert>
#include <sstream>

NpcStateInfo::NpcStateInfo(Npc& npc)
    :npc{ std::move(npc) }
{}

const Movement StateMachine::Update(Npc& npc)
{
    assert(npcsStateInfo.find(npc.ID) != end(npcsStateInfo) && "Npc Explore logic was never defined");
    auto& npcStateInfo = npcsStateInfo[npc.ID];
    Movement result = { HexDirection::CENTER, npc.hexID };

    std::stringstream ss;
    ss << "-----NPC-----" << std::endl;
    ss << "NpcID:" << npc.ID << "Path:";

    auto map = GameManager::getInstance().getMap();
    bool retry = false;

    do
    {
        switch (npcStateInfo.objective)
        {
        case State::Explore:
        {
            if (npcStateInfo.pathRecord.empty())
            {
                GameManager::getInstance().getAIHelper().bb.setBestPath(npcStateInfo);
                retry = true;
                break;
            }
   
            auto temp = getNextValidMovement(npc.ID);

            //is next hex available?
            if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npc.ID, temp.toHexID)) {
                // we got a valid move!
                result = temp;
                npcStateInfo.pathRecord.pop_back();
                retry = false;
            }
            else
            {
                // skip turn, a npc is probably on the hex we want to go
                npcStateInfo.objective = State::Blocked;
                retry = false;
            }

            break;
        }
        case State::ExploreOriented:
            //is npc path empty?
            if (npcStateInfo.pathRecord.empty())
            {
                // is npc on goal?
                if (map.getHexByID(npc.hexID).isType(HexType::TileAttribute_Goal)) {
                    //yes, then we stay on that tile for eternity
                    npcStateInfo.objective = State::OnGoal;
                    retry = false;
                }
                else {
                    //no, then we need a new path to the same goal
                    GameManager::getInstance().getAIHelper().bb.setBestPath(npcStateInfo);

                    //if no path
                    if (npcStateInfo.pathRecord.empty()) {
                        //we are already at the best distance from goal, we need to explore to find a path
                        npcStateInfo.objective = State::Explore;
                    }

                    retry = true;           
                }
            }
            else {
                auto temp = getNextValidMovement(npc.ID);

                //is next hex available?
                if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npc.ID, temp.toHexID)) {
                    // we got a valid move!
                    result = temp;
                    npcStateInfo.pathRecord.pop_back();
                }
                else
                {
                    // skip turn, a npc is probably on the hex we want to go
                    npcStateInfo.objective = State::Blocked;
                }

                retry = false;
            }

            break;
        case State::Blocked:
            auto temp = getNextValidMovement(npc.ID);

            // can we do the last move we tried?
            if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npc.ID, temp.toHexID)) {
                // we got a valid move!
                result = temp;
                npcStateInfo.pathRecord.pop_back();

                if(GameManager::getInstance().getAIHelper().bb.isHighValue(npcStateInfo.influenceZone.currentHighest.score))
                    npcStateInfo.objective = State::ExploreOriented;
                else
                    npcStateInfo.objective = State::Explore;

                retry = false;
            }
            else {
                // a npc on a goal tile is probably on the hex we want to go... tss! get a new path
                npcStateInfo.objective = State::Init;
                retry = true;
            }
               
            break;

        case State::OnGoal:
            // we got nothing to do! Pheww!
            break;

        case State::Init:
            // is npc omniscient?
            if (npcStateInfo.npc.omniscient) {
                // get a goal champ!
                npcStateInfo.influenceZone.currentHighest = GameManager::getInstance().getAIHelper().bb.getBestGoal(npc.ID);
                AStar::SetBestPath(npcStateInfo);
                npcStateInfo.objective = State::ExploreOriented;
            }
            else {
                // find a path to the best influence hex
                GameManager::getInstance().getAIHelper().bb.setBestPath(npcStateInfo);
            }
  
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
    auto& npcStateInfo = npcsStateInfo[npcID];

    assert(!npcStateInfo.pathRecord.empty() && "Npc explore path record is empty, no available move known");
    auto nextMovement = npcStateInfo.pathRecord.back();

    return nextMovement;
}