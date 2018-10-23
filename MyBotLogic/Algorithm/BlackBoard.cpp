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
    mergedPath = static_cast<unsigned int>(npcCount+1);
    data.resize(DataType::COUNT);
    for(auto& d : data)
        d.resize(size);
}

void BlackBoard::UpdateNpc(NpcStateInfo& stateInfo, const std::set<unsigned int>& seenHexesID) noexcept {
    npcsCurrentHexID.insert(stateInfo.npc.hexID);
    data[DataType::Flood][stateInfo.npc.hexID] = stateInfo.npc.ID;

    updateScoreData(seenHexesID);
    updateFloodData(stateInfo.npc);
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
    return data[DataType::Flood][goalHexID] == npcID || data[DataType::Flood][goalHexID] == mergedPath;
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
        return g.available.first && isGoalReachable(npcInfo.npc.ID, g.hexID);
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

void BlackBoard::updateFloodData(const Npc& npc)
{
    auto map = GameManager::getInstance().getMap();
    
    std::vector<unsigned int> hexIDInRadius{ npc.hexID };
    std::vector<unsigned int> temp;
    unsigned int npcIDToFuse = 0;

    for (unsigned int i = 0; i < npc.visionRange; ++i)
    {
        temp.clear();
        for (auto hexID : hexIDInRadius) {
            auto hex = map.getHexByID(hexID);

            for (auto edge : hex.edges) {
                if (!(edge.isBlocked))
                {
                    if (isUninitialized(DataType::Flood, edge.leadsToHexID)) {
                        data[DataType::Flood][edge.leadsToHexID] = npc.ID;
                        temp.emplace_back(edge.leadsToHexID);
                    }
                    else if (isFloodedByAnotherNpc(edge.leadsToHexID, npc.ID))
                        npcIDToFuse = data[DataType::Flood][edge.leadsToHexID];                  
                }
            }
        }
        hexIDInRadius.swap(temp);
    }

    if (npcIDToFuse > 0) {
        std::replace_if(begin(data[DataType::Flood]), end(data[DataType::Flood]), [&npc, &npcIDToFuse](const unsigned int& val){
            return val == npc.ID || val == npcIDToFuse;
        }, mergedPath);

   //     --mergedPath;
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