#include "AIHelper.h"

#include "Utils/DebugHelper.h"
#include "Globals.h"

#include <ostream>
#include <algorithm>
#include <cassert>
#include <sstream>

void AIHelper::Init(const LevelInfo& levelInfo)
{
    blackBoard.Init(levelInfo.rowCount * levelInfo.colCount, levelInfo.npcs.size());

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
    blackBoard.npcsCurrentHexID.clear();

    std::stringstream ss;

    for (auto n : turnInfo.npcs)
        blackBoard.UpdateNpc(stateMachine.npcsStateInfo[n.first], n.second.visibleTiles);

    ss << "-----BlackBoard-----" << std::endl
        << blackBoard << std::endl;
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

//bool AIHelper::TryAddNpcCurrentHexID(NpcStateInfo& npcStateInfo)
//{
//    bool success = npcsCurrentHexID.insert(npcStateInfo.pathRecord.back().toHexID).second;
//
//    if (success) {
//        npcsCurrentHexID.erase(npcStateInfo.npc.hexID);
//        npcStateInfo.pathRecord.pop_back();
//    }
//
//    return success;
//}
//
//bool AIHelper::isHexIDOccupied(const unsigned int hexIDtoLook)
//{
//    return npcsCurrentHexID.find(hexIDtoLook) != end(npcsCurrentHexID);
//}
//
//const std::set<unsigned int>& AIHelper::getNpcsCurrentHexID() noexcept {
//    return npcsCurrentHexID;
//}