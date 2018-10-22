#include "AIHelper.h"

#include "GameManager.h"
#include "Utils/DebugHelper.h"
#include "Algorithm/BehaviourTree.h"

#include <ostream>
#include <algorithm>
#include <cassert>
#include <sstream>

map<unsigned int, NpcStateInfo> AIHelper::npcsInfo = map<unsigned int, NpcStateInfo>();

void AIHelper::Init(const LevelInfo& levelInfo)
{
    blackBoard.Init(levelInfo.rowCount * levelInfo.colCount);
	// L'état des Npc qui sera lié aux behavior tree
	BehaviourTree bt = BehaviourTree();

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
        blackBoard.UpdateNpc(stateMachine.npcsStateInfo[n.first], n.second.visibleTiles);

        ss << "-----Influence-----" << " NpcID:" << n.first << std::endl;;// << stateMachine.npcsStateInfo.at(n.first).influenceZone << std::endl;

        for(auto data : stateMachine.npcsStateInfo[n.first].influenceZone.data)
                ss << "HexID:" << data.hexID << ' '
                << "Score:" << data.score << std::endl;
    }

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

bool AIHelper::TryAddNpcCurrentHexID(NpcStateInfo& npcStateInfo)
{
    bool success = npcsCurrentHexID.insert(npcStateInfo.pathRecord.back().toHexID).second;

    if (success) {
        npcsCurrentHexID.erase(npcStateInfo.npc.hexID);
        npcStateInfo.pathRecord.pop_back();
    }
    else {
        npcStateInfo.objective = State::Blocked;
    }

    return success;
}

bool AIHelper::isHexIDOccupied(const unsigned int hexIDtoLook)
{
    return npcsCurrentHexID.find(hexIDtoLook) != end(npcsCurrentHexID);
}

const std::set<unsigned int>& AIHelper::getNpcsCurrentHexID() noexcept {
    return npcsCurrentHexID;
}
