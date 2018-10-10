#include "AIHelper.h"

#include "GameManager.h"
#include "Utils/PathHelper.h"
#include "Algorithm/AStar.h"

#include <algorithm>
#include <cassert>
#include <utility>

AIHelper::AIHelper(const LevelInfo& levelInfo)
{
    for (auto npc : levelInfo.npcs) {
        npcs.insert({
            npc.first,
            { 
                npc.second.npcID,
                npc.second.tileID,
                npc.second.visionRange,
                npc.second.movementRange,
                npc.second.omniscient
            } });
        npcsCurrentHexID.insert(npc.second.tileID);
    }

    for (auto hex : GameManager::getInstance().getMap().getLayout())
        if (hex.type == HexType::TileAttribute_Goal)
            goalHexIDs.insert(hex.ID);

    //TODO remove -> add to behaviour tree
    for (auto pair : npcs) {
        if (pair.second.omniscient) {
            setBestGoal(pair.second);
        }
        else {
            exploreState.npcsExploreInfo.insert({ pair.first, ExploreInfo{ pair.first } });
        }
    }
}

void AIHelper::Update(const TurnInfo& turnInfo) {
    npcsCurrentHexID.clear();

    for (auto n : turnInfo.npcs) {
        npcs.at(n.first).hexID = n.second.tileID; 
        npcsCurrentHexID.insert(n.second.tileID);
    }
}

void AIHelper::FillActionList(std::vector<Action*>& actionList)
{
    for (auto pair : npcs)
    {
        auto npc = pair.second;

        if(goalState.npcsGoalInfo.find(npc.ID) != end(goalState.npcsGoalInfo))
            actionList.push_back(
                new Move(npc.ID, goalState.Update(npc))
            );
        else {
            actionList.push_back(
                new Move(npc.ID, HexDirection::NW)
               // new Move(npc.ID, exploreState.Update(npc))
            );
        }
    }
}

void AIHelper::setBestGoal(Npc& npc) {
    assert(!goalHexIDs.empty() && "No goal available");
    auto npcHex = GameManager::getInstance().getMap().getConstHexByID(npc.hexID);

    float shortest = std::numeric_limits<float>::max();
    unsigned int npcGoalID = npcHex.ID;

    for (auto goalID : goalHexIDs) {
        auto goalHex = GameManager::getInstance().getMap().getConstHexByID(goalID);
        auto test = PathHelper::DistanceBetween(npcHex.position, goalHex.position);
        if (shortest > test)
        {
            shortest = test;
            npcGoalID = goalID;
        }            
    }

    assert(npcGoalID != npc.hexID && "No goal available... again?");
    goalHexIDs.erase(npcGoalID);
    goalState.npcsGoalInfo.insert({ npc.ID, GoalInfo{ npc.ID, npcGoalID } });
}

void AIHelper::SwitchExploreToGoal(const unsigned int& npcID)
{
    exploreState.npcsExploreInfo.erase(npcID);
    setBestGoal(npcs.at(npcID));
}

bool AIHelper::TryAddNpcCurrentHexID(const unsigned int& npcID, const unsigned int toAdd)
{
    bool success = npcsCurrentHexID.insert(toAdd).second;

    if(success)
        npcsCurrentHexID.erase(npcs.at(npcID).hexID);

    return success;
}

const std::set<unsigned int>& AIHelper::getNpcsCurrentHexID() noexcept {
    return npcsCurrentHexID;
}