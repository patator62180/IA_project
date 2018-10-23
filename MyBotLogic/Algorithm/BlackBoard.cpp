#include "BlackBoard.h"

#include "../GameManager.h"
#include "../GameObject/Map.h"
#include "../Utils/PathHelper.h"
#include "AStar.h"

#include <assert.h>
#include <algorithm>
#include <deque>

void BlackBoard::Init(const size_t size, const size_t npcCount) noexcept
{
    data.resize(DataType::COUNT);
    for(auto& d : data)
        d.resize(size);
}

void BlackBoard::UpdateNpc(NpcStateInfo& stateInfo, const std::set<unsigned int>& seenHexesID) noexcept {
    npcsCurrentHexID.insert(stateInfo.npc.hexID);
    data[DataType::Flood][stateInfo.npc.hexID] = stateInfo.npc.ID;

    updateScoreData(seenHexesID);
    updateFloodData(stateInfo);
}

inline bool BlackBoard::isUninitialized(const DataType dataIndex, const unsigned int hexID) const noexcept {
    return data[dataIndex][hexID] < UNINITIALIZED + 1;
}

inline bool BlackBoard::isFloodedByAnotherNpc(const unsigned int hexID, const unsigned int npcID) const noexcept {
    return data[DataType::Flood][hexID] != npcID;
}

void BlackBoard::AddGoal(const unsigned int goalHexID) noexcept {
    goals.push_back({ goalHexID });
}

bool BlackBoard::isGoalReachable(const unsigned int npcID, const unsigned int goalHexID) {
    return data[DataType::Flood][goalHexID] == npcID;
}

void BlackBoard::setNpcObjectiveHexID(NpcStateInfo& npcInfo) {
    if (findClosestGoal(npcInfo)) {
        AStar::SetBestPath(npcInfo);
    }
    else {
        throw int{};
    }   
}

bool BlackBoard::findClosestGoal(NpcStateInfo& npcInfo) {
    auto bestGoalIter = std::find_if(begin(goals), end(goals), [&npcInfo, this](const Goal& g) {
        return g.available.first && isGoalReachable(npcInfo.floodID, g.hexID);
    });

    if (bestGoalIter != end(goals)) {
        bestGoalIter->available = { false, npcInfo.npc.ID };
        npcInfo.objectiveHexID = bestGoalIter->hexID;
        return true;
    }

    return false;
}

void BlackBoard::updateScoreData(const std::set<unsigned int>& seenHexesID)
{
    auto map = GameManager::getInstance().getMap();

    for (auto ID : seenHexesID)
        if (isUninitialized(DataType::Score, ID))
        {
            auto hex = map.getHexByID(ID);
            switch (hex.type) {
                case HexType::TileAttribute_Default:
                    data[DataType::Score][ID] = hex.EdgeCountType(EdgeType::ObjectType_Window);
                    break;

                case HexType::TileAttribute_Goal:
                    data[DataType::Score][ID] = GOAL_SCORE;
                    break;

                default:
                    break;
            }
        }
}

void BlackBoard::updateFloodData(NpcStateInfo& npcInfo)
{
    auto map = GameManager::getInstance().getMap();
    auto bb = GameManager::getInstance().getAIHelper().blackBoard;
    unsigned int npcIDToFuse = 0;

    std::deque<unsigned int> hexIDToDo{ npcInfo.npc.hexID };

    while (!hexIDToDo.empty()) {
        auto hex = map.getHexByID(hexIDToDo.front());
        hexIDToDo.pop_front();

        for (auto edge : hex.edges) {
            if (!(edge.isBlocked))
            {
                if (isUninitialized(DataType::Flood, edge.leadsToHexID)) {
                    data[DataType::Flood][edge.leadsToHexID] = npcInfo.floodID;
                    hexIDToDo.emplace_back(edge.leadsToHexID);
                }
                else if (isFloodedByAnotherNpc(edge.leadsToHexID, npcInfo.npc.ID))
                    npcIDToFuse = data[DataType::Flood][edge.leadsToHexID];
            }
        }
    }

    if (npcIDToFuse > 0) {
        std::replace_if(begin(data[DataType::Flood]), end(data[DataType::Flood]), [&npcInfo](const unsigned int& val){
            return val == npcInfo.npc.ID;
        }, npcIDToFuse);
        npcInfo.floodID = npcIDToFuse;
    }
}

bool BlackBoard::isNextMovementValid(NpcStateInfo& npcStateInfo)
{
    bool success = npcsCurrentHexID.insert(npcStateInfo.pathRecord.back().toHexID).second;

    if (success) {
        npcsCurrentHexID.erase(npcStateInfo.npc.hexID);
        npcStateInfo.pathRecord.pop_back();
    }

    return success;
}