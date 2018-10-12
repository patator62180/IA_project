#include "BlackBoard.h"

#include "../GameManager.h"
#include "../GameObject/Map.h"
#include "../Utils/PathHelper.h"

#include <assert.h>

void BlackBoard::Init(const size_t& size) noexcept
{
    data = BlackBoardData(size, UNINITIALIZED);
}

void BlackBoard::UpdateNpcsVision(const TurnInfo& turnInfos) noexcept {
    auto map = GameManager::getInstance().getMap();

    for (auto npcInfo : turnInfos.npcs)
        for (auto hexID : npcInfo.second.visibleTiles)
            if (needUpdate(data[hexID])) {
                auto hex = map.getConstHexByID(hexID);
                auto score = getTempVisionScore(hex, turnInfos.turnNb);
                data[hex.ID] = score;
            }
}

void BlackBoard::UpdateNpcTile(const unsigned int& npcHexID) noexcept {
    auto npcHex = GameManager::getInstance().getMap().getConstHexByID(npcHexID);
    data[npcHexID] = MIN_REAL_SCORE + getRealVisionScore(npcHex);
}

void BlackBoard::UpdateGoalTile(const unsigned int& goalHexID) noexcept {
    data[goalHexID] = GOAL_SCORE;
    goals.push_back({ goalHexID });
}

const int BlackBoard::getTempVisionScore(const Hex& h, const unsigned int& turnNb) const noexcept {
    return MIN_TEMP_SCORE + getRealVisionScore(h) + turnNb * TURN_WEIGHT;
}

const int BlackBoard::getRealVisionScore(const Hex& h) const noexcept {
    return h.getEdgeCanSeeThroughCount();
}

const bool BlackBoard::needUpdate(const int& score) noexcept {
    return score == UNINITIALIZED;// || !isRealScore(score);
}

const unsigned int BlackBoard::getBestHexIDToGo(const Npc& npc) noexcept {

    //auto npcHex = GameManager::getInstance().getMap().getConstHexByID(npc.hexID);
    //auto adjacentHexIDs = npcHex.getValidAdjacentHexIDs();

    //InfluenceMap influenceMap;
    //for (auto id : adjacentHexIDs)
    //    if(!needUpdate(data[id]))
    //        influenceMap.insert({ id, data[id] });

    //InfluenceMap influenceMap;
    //for (auto id : adjacentHexIDs)
    //        influenceMap.insert({ id, data[id] });

    //if(influenceMap.empty())
    //    return *std::max_element(begin(data), end(data));

    auto maxIter = std::max_element(begin(data), end(data), [](const int& r, const int& l) {
        return r < l;
    });

    return (*maxIter == GOAL_SCORE) ? FindClosestGoal(npc) : *maxIter;
}

const bool BlackBoard::isRealScore(const int& score) {
    return MIN_REAL_SCORE <= score && score >= MAX_REAL_SCORE;
}

unsigned int BlackBoard::FindClosestGoal(const Npc& npc) {
    assert(!goals.empty() && "No goal available");
    auto npcHex = GameManager::getInstance().getMap().getConstHexByID(npc.hexID);

    std::vector<float> results;
    for (auto goal : goals)
        if (goal.available) {
            auto goalHex = GameManager::getInstance().getMap().getConstHexByID(goal.hexID);
            results.push_back(PathHelper::DistanceBetween(npcHex.position, goalHex.position));
        }
        else {
            results.push_back(1000);
        }

        auto result = std::min_element(begin(results), end(results));
        auto index = std::distance(begin(results), result);
        auto& goal = goals.at(index);

        assert(goal.hexID != npc.hexID && "No goal available... again?");

        goal.available = false;
        return goal.hexID;
}