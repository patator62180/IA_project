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
            //does npc have path?
            if (npcStateInfo.pathRecord.empty())
            {
                //get one and retry, could lead to infinite loop but we good for now
                GameManager::getInstance().getAIHelper().bb.setBestPath(npcStateInfo);
                retry = true;
                break;
            }
   
            auto temp = npcStateInfo.pathRecord.back();
            //is next hex available?
            if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npcStateInfo))
                // we got a valid move!
                result = temp;
            else
                // skip turn, a npc is probably on the hex we want to go
                npcStateInfo.objective = State::Blocked;

            retry = false;

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

                    //does the npc have a path?
                    if (npcStateInfo.pathRecord.empty()) {
                        //then we are already at the best distance from goal, we need to explore to find a path
                        npcStateInfo.objective = State::Explore;
                    }

                    retry = true;           
                }
            }
            else {
                //check if edge blocks, small flaw when generating path with unseen objects 
                if (map.getHexByID(npc.hexID).edges[npcStateInfo.pathRecord.back().direction].isBlocked) {
                    //find a new path
                    AStar::SetBestPath(npcStateInfo);
                    //true if the npc hex is the closest to the goal, it means we dont know a better path
                    if (npcStateInfo.pathRecord.back().direction == HexDirection::CENTER) {
                        //so go explore!
                        GameManager::getInstance().getAIHelper().bb.setBestPathToUnvisited(npcStateInfo);
                        npcStateInfo.objective = State::Explore;
                    }

                    retry = true;
                }
                else {
                    auto temp = npcStateInfo.pathRecord.back();
                    //is next hex available?
                    if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npcStateInfo))
                        // we got a valid move!
                        result = temp;
                    else
                        // skip turn, a npc is probably on the hex we want to go
                        npcStateInfo.objective = State::Blocked;

                    retry = false;
                }
            }

            break;
        case State::Blocked:
            auto temp = npcStateInfo.pathRecord.back();

            // can we do the last move we tried?
            if (GameManager::getInstance().getAIHelper().TryAddNpcCurrentHexID(npcStateInfo)) {
                // we got a valid move!
                result = temp;

                //set the previous state before being blocked
                if(GameManager::getInstance().getAIHelper().bb.isHighValue(npcStateInfo.influenceZone.currentHighest.score))
                    npcStateInfo.objective = State::ExploreOriented;
                else
                    npcStateInfo.objective = State::Explore;

                retry = false;
            }
            else {
                // a npc on a goal tile is probably on the hex we want to go... tss! get a new path
                GameManager::getInstance().getAIHelper().bb.setBestPath(npcStateInfo);
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