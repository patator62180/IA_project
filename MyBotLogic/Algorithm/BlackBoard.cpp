#include "BlackBoard.h"

#include "../GameManager.h"
#include "../GameObject/Map.h"
#include "../Utils/PathHelper.h"

#include <assert.h>
#include <algorithm>
#include <deque>

void BlackBoard::Init(const size_t& size) noexcept
{
    data.resize(size);
}

void BlackBoard::UpdateNpc(NpcStateInfo& stateInfo, const std::set<unsigned int>& seenHexesID) noexcept {
    auto npcHex = GameManager::getInstance().getMap().getHexByID(stateInfo.npc.hexID);
    data[stateInfo.npc.hexID] = MIN_VISITED_SCORE + npcHex.EdgeCountNotBlocked();

    stateInfo.influenceZone.Update(seenHexesID);
    for (auto highValueData : stateInfo.influenceZone.dataInTime) {
        if(data[highValueData.hexID] < highValueData.score)
            data[highValueData.hexID] = highValueData.score;
    }

    for (auto edge : npcHex.edges) {
        if (! (edge.isBlocked || hasBeenVisited(data[edge.leadsToHexID]) )) {
            data[edge.leadsToHexID] = ADJACENT_NOT_VISITED;
        }
    }
}

inline bool BlackBoard::hasBeenVisited(const unsigned int score) const {
    return score > ADJACENT_NOT_VISITED;
}

void BlackBoard::UpdateGoal(const unsigned int goalHexID) noexcept {
    goals.push_back({ goalHexID });
}

void BlackBoard::setBestInfluenceHex(NpcStateInfo& npcInfo) {

    auto bestResult = npcInfo.influenceZone.consumeBestLatestHexID();

    if (bestResult.score == 0) {
        std::deque<unsigned int> hexIDToDo;
        hexIDToDo.push_back(npcInfo.npc.hexID);
       
        auto map = GameManager::getInstance().getMap();

        bool found = false;
        while (!found) {
            auto hex = map.getHexByID(hexIDToDo.front());
            hexIDToDo.pop_front();

            for (auto edge : hex.edges) {
                if (!edge.isBlocked) {
                    if (data[edge.leadsToHexID] == ADJACENT_NOT_VISITED) {
                        bestResult.hexID = edge.leadsToHexID;
                        bestResult.score = data[edge.leadsToHexID];
                        found = true;
                        break;
                    }
                    hexIDToDo.emplace_back(edge.leadsToHexID);
                }
            }
        }
    }
    npcInfo.influenceZone.currentHighest = bestResult;
}

unsigned int BlackBoard::getBestGoal(const unsigned int npcID) {
    auto goalIter = std::find_if(begin(goals), end(goals), [&npcID](Goal& g) {
        return g.available.first || npcID == g.available.second;
    });

    goalIter->available = { false, npcID };
    return goalIter->hexID;
}