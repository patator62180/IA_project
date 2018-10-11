#include "AIHelper.h"

#include "GameManager.h"
#include "Utils/PathHelper.h"
#include "Utils/DebugHelper.h"
#include "Algorithm/AStar.h"

#include <algorithm>
#include <cassert>
#include <utility>
#include <sstream>

void AIHelper::Init(const LevelInfo& levelInfo)
{
    bb.Init(levelInfo.rowCount * levelInfo.colCount);

    //TODO function
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
        bb.UpdateNpcTile(npc.second.tileID);
    }

    //TODO function
    for (auto hex : GameManager::getInstance().getMap().getLayout())
        if (hex.type == HexType::TileAttribute_Goal && !hex.areAllEdgesBlocked())
            bb.UpdateGoalTile(hex.ID);
            
    //TODO remove -> add to behaviour tree
    for (auto pair : npcs) {
        if (pair.second.omniscient) {
            auto goalHexID = bb.getBestHexIDToGo(pair.second);
            goalState.npcsGoalInfo.insert({ pair.second.ID, GoalInfo{ pair.second.ID, goalHexID } });
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

    bb.UpdateNpcsVision(turnInfo);
    std::stringstream ss;
    ss << "-----BlackBoard-----" << std::endl << bb;
    DebugHelper::getInstance().Log(ss.str());
}

void AIHelper::FillActionList(std::vector<Action*>& actionList)
{
    for (auto pair : npcs)
    {
        auto npc = pair.second;
        bool isNpcGoal = goalState.npcsGoalInfo.find(npc.ID) != end(goalState.npcsGoalInfo);
        Movement next = isNpcGoal ? goalState.Update(npc) : exploreState.Update(npc);

        actionList.push_back(
            new Move(npc.ID, next.direction)
        );
        bb.UpdateNpcTile(next.toHexID);
    }
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