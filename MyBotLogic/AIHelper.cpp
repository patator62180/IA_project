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
        exploreState.npcsStateInfo.insert( { npc.first, {} });
    }

    //TODO function
    for (auto hex : GameManager::getInstance().getMap().getLayout())
        if (hex.type == HexType::TileAttribute_Goal && !hex.areAllEdgesBlocked())
            bb.UpdateGoalTile(hex.ID);
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
        Movement next = exploreState.Update(npc);

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