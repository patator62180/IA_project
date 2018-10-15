#include "AIHelper.h"

#include "GameManager.h"
#include "Utils/DebugHelper.h"

#include <ostream>
#include <algorithm>
#include <cassert>
#include <sstream>

void AIHelper::Init(const LevelInfo& levelInfo)
{
    bb.Init(levelInfo.rowCount * levelInfo.colCount);

    for (auto npc : levelInfo.npcs)
        stateMachine.npcsStateInfo.insert( { npc.first, Npc{
                npc.second.npcID,
                npc.second.tileID,
                npc.second.visionRange,
                npc.second.movementRange,
                npc.second.omniscient
            } });
}

void AIHelper::Update(const TurnInfo& turnInfo) {
    npcsCurrentHexID.clear();

    std::stringstream ss;

    for (auto n : turnInfo.npcs) {
        npcsCurrentHexID.insert(n.second.tileID);
        bb.UpdateNpc(stateMachine.npcsStateInfo.at(n.first), n.second.visibleTiles);

        ss << "-----Influence-----" << " NpcID:" << n.first << std::endl;;// << stateMachine.npcsStateInfo.at(n.first).influenceZone << std::endl;

        for(auto data : stateMachine.npcsStateInfo.at(n.first).influenceZone.dataInTime)
                ss << "HexID:" << data.hexID << ' '
                << "Score:" << data.score << std::endl;
    }

    ss << "-----BlackBoard-----" << std::endl
        << bb << std::endl;
    DebugHelper::getInstance().Log(ss.str());
}

void AIHelper::FillActionList(std::vector<Action*>& actionList)
{
    for (auto& pair : stateMachine.npcsStateInfo)
    {
        Movement next = stateMachine.Update(pair.second.npc);

        actionList.push_back(
            new Move(pair.second.npc.ID, next.direction)
        );

        pair.second.npc.hexID = next.toHexID;
    }
}

bool AIHelper::TryAddNpcCurrentHexID(const unsigned int npcID, const unsigned int toAdd)
{
    bool success = npcsCurrentHexID.insert(toAdd).second;

    if(success)
        npcsCurrentHexID.erase(stateMachine.npcsStateInfo.at(npcID).npc.hexID);

    return success;
}

bool AIHelper::isHexIDOccupied(const unsigned int hexIDtoLook)
{
    return npcsCurrentHexID.find(hexIDtoLook) != end(npcsCurrentHexID);
}

const std::set<unsigned int>& AIHelper::getNpcsCurrentHexID() noexcept {
    return npcsCurrentHexID;
}