#include "BlackBoard.h"

#include "../GameManager.h"
#include "../GameObject/Map.h"
#include "../Utils/PathHelper.h"
#include "AStar.h"

#include <assert.h>
#include <algorithm>
#include <deque>

void BlackBoard::Init(const size_t size) noexcept
{
    data.resize(size);
}

void BlackBoard::UpdateNpc(NpcStateInfo& stateInfo, const std::set<unsigned int>& seenHexesID) noexcept {
    auto npcHex = GameManager::getInstance().getMap().getHexByID(stateInfo.npc.hexID);
    data[stateInfo.npc.hexID] = npcHex.EdgeCountNotBlocked();

    stateInfo.influenceZone.Update(seenHexesID);

    // add high value to blackboard if better than current hexID
    for (auto highValueData : stateInfo.influenceZone.data) {
        if(!hasBeenVisited(data[highValueData.hexID]))
            data[highValueData.hexID] = BASE_INFLUENCE_SCORE + highValueData.score;
    }

    // add valid hexes neibourgh to blackboard if was never visited 
    for (auto edge : npcHex.edges) {
        if (! (edge.isBlocked || hasBeenVisited(data[edge.leadsToHexID]) )) {
            data[edge.leadsToHexID] = BASE_INFLUENCE_SCORE;
        }
    }
}

inline bool BlackBoard::hasBeenVisited(const unsigned int score) const {
    return !(score < MIN_VISITED_SCORE || score > MAX_VISITED_SCORE);
}

bool BlackBoard::isUnvisited(const unsigned int ID) const noexcept {
    return data[ID] >= BASE_INFLUENCE_SCORE;
}

bool BlackBoard::isHighValue(const unsigned int score) const noexcept {
    return score > BASE_INFLUENCE_SCORE;
}

void BlackBoard::UpdateGoal(const unsigned int goalHexID) noexcept {
    goals.push_back({ goalHexID });
}

void BlackBoard::setBestPath(NpcStateInfo& npcInfo) {
    // npcExploreInfo.influenceZone.currentHighest.hexID = GameManager::getInstance().getAIHelper().bb.getBestGoal();
    auto bestResult = (npcInfo.npc.omniscient) ? 
        getBestGoal(npcInfo.npc.ID) :
        npcInfo.influenceZone.consumeBestInfluence();

    //true if didnt find anything of interest
    if (bestResult.score == 0) {
        AStar::SetNearestUnvisited(npcInfo);
        npcInfo.objective = State::Explore;
    }
    else {
        npcInfo.currentHighest = bestResult;
        npcInfo.objective = State::ExploreOriented;
    }
       
    AStar::SetBestPath(npcInfo);
}

void BlackBoard::setBestPathToUnvisited(NpcStateInfo& npcInfo) {
    AStar::SetNearestUnvisited(npcInfo);
    AStar::SetBestPath(npcInfo);
    npcInfo.objective = State::Explore;
}

InfluenceHex BlackBoard::getBestGoal(const unsigned int npcID) {
    auto goalIter = std::find_if(begin(goals), end(goals), [&npcID](Goal& g) {
        return g.available.first || npcID == g.available.second;
    });

    goalIter->available = { false, npcID };
    return { goalIter->hexID, GOAL_SCORE };
}