#include "StateMachine.h"

#include "../GameManager.h"
#include "../Utils/DebugHelper.h"
#include "AStar.h"

#include <cassert>
#include <sstream>

NpcStateInfo::NpcStateInfo(Npc& npc)
    :npc{ std::move(npc) }
{}

Movement StateMachine::Update(Npc& npc)
{
    assert(npcsStateInfo.find(npc.ID) != end(npcsStateInfo) && "Npc Explore logic was never defined");
    auto& npcStateInfo = npcsStateInfo[npc.ID];
    Movement result = { HexDirection::CENTER, npc.hexID };

    std::stringstream ss;
    ss << "-----NPC-----" << std::endl;
    ss << "NpcID:" << npc.ID << "Path:";

    auto map = GameManager::getInstance().getMap();
    auto& bb = GameManager::getInstance().getAIHelper().blackBoard;
    bool retry = false;

    do
    {
        switch (npcStateInfo.objective)
        {
        case State::FollowPath:
        {
            auto temp = npcStateInfo.pathRecord.back();
            //is next hex available?
            if (bb.isNextMovementValid(npcStateInfo))
            {
                // we got a valid move!
                result = temp;

                if (npcStateInfo.pathRecord.empty())
                    npcStateInfo.objective = State::OnGoal;
            }
            else {
                npcStateInfo.objective = State::Blocked;
            }

            retry = false;
            break;
        }
        case State::Blocked:
        {
            // can we do the last move we tried?
            auto temp = npcStateInfo.pathRecord.back();
            if (bb.isNextMovementValid(npcStateInfo)) {
                // we got a valid move!
                result = temp;
                retry = false;
            }
            else {
                npcStateInfo.objective = State::Init;
                retry = true;
            }

            break;
        }
        case State::Init:
            //omniscient
            bb.setNpcObjectiveHexID(npcStateInfo);
            npcStateInfo.objective = State::FollowPath;
            retry = true;
            break;

        case State::OnGoal:
            break;
        }

       
    } while (retry);

    //for (auto i : npcStateInfo.pathRecord) {
    //    ss << i.toHexID << ' ';
    //}

    //ss << std::endl << " ToHexID:" << result.toHexID << " PathSize:" << npcStateInfo.pathRecord.size();;
    //DebugHelper::getInstance().Log(ss.str());

    return result;
}


/*

 //case State::Explore:
        //{
        //    //does npc need a path?
        //    if (npcStateInfo.pathRecord.empty())
        //    {
        //        //then get a new path
        //        npcStateInfo.objective = State::Init;
        //        retry = true;
        //    }
        //    else
        //    {
        //        auto temp = npcStateInfo.pathRecord.back();
        //        //is the next hex available?
        //        if (AIHelper.TryAddNpcCurrentHexID(npcStateInfo))
        //            // we got a valid move!
        //            result = temp;

        //        retry = false;
        //    }

        //    break;
        //}
        //case State::ExploreOriented:
        //    //does npc need a path?
        //    if (npcStateInfo.pathRecord.empty())
        //    {
        //        // is npc on goal?
        //        if (map.getHexByID(npc.hexID).isType(HexType::TileAttribute_Goal)) {
        //            //then we stay on that tile for eternity
        //            npcStateInfo.objective = State::OnGoal;
        //            retry = false;
        //        }
        //        else {
        //            //then we need a new path
        //            npcStateInfo.objective = State::Init;
        //            retry = true;
        //        }
        //    }
        //    else {
        //        //does edge blocks the way? small flaw when generating path with unseen objects
        //        if (map.getHexByID(npc.hexID).edges[npcStateInfo.pathRecord.back().direction].isBlocked) {
        //            //then find a new path
        //            npcStateInfo.objective = State::Init;
        //            retry = true;
        //        }
        //        else {
        //            auto temp = npcStateInfo.pathRecord.back();
        //            //is next hex available?
        //            if (AIHelper.TryAddNpcCurrentHexID(npcStateInfo))
        //                // we got a valid move!
        //                result = temp;

        //            retry = false;
        //        }
        //    }

        //    break;
        //case State::Blocked:
        //    auto nextMovement = npcStateInfo.pathRecord.back();
        //    // can we do the last move we tried?
        //    if (AIHelper.TryAddNpcCurrentHexID(npcStateInfo)) {
        //        // we got a valid move!
        //        result = nextMovement;

        //        //set the previous state before being blocked
        //        if(AIHelper.blackBoard.isHighValue(npcStateInfo.currentHighest.score))
        //            npcStateInfo.objective = State::ExploreOriented;
        //        else
        //            npcStateInfo.objective = State::Explore;

        //        retry = false;
        //    }
        //    else {
        //        // a npc on a goal tile is probably on the hex we want to go... tss! get a new path
        //        AIHelper.blackBoard.setBestPath(npcStateInfo);
        //        retry = true;
        //    }
        //
        //    break;

        //case State::OnGoal:
        //    // we got nothing to do! Pheww!
        //    break;

        //case State::Init:
        //    // find a path to the best influence hex
        //    AIHelper.blackBoard.setBestPath(npcStateInfo);

        //    //true if we cant be closer to the goal
        //    if (npcStateInfo.pathRecord.empty())
        //        //is npc blocked by another npc?
        //        if (AIHelper.isHexIDOccupied(npcStateInfo.currentHighest.hexID)) {
        //            //just wait for the next turn
        //            //should be setting npc objective to blocked but we cant have any path since the npc may be blocking the only way
        //            //TODO refacto AStar to maybe ignore npc position and let the state machine handle it
        //            retry = false;
        //            break;
        //        }
        //        else
        //        //then so go explore!
        //        AIHelper.blackBoard.setBestPathToUnvisited(npcStateInfo);

        //    retry = true;
        //    break;
        //}

*/