#include "AStar.h"

#include "../GameManager.h"
#include "../Utils/PathHelper.h"
#include "../MapStructure/Hex.h"
#include "../AIHelper.h"

#include <deque>
#include <algorithm>

void AStar::SetBestPath(NpcStateInfo& stateInfo) noexcept
{
    AStarRecord opened;
    AStarRecord closed;
    auto map = GameManager::getInstance().getMap();
    auto currentHex = map.getHexByID(stateInfo.npc.hexID);
    auto hexGoal = map.getHexByID(stateInfo.objectiveHexID);
    auto obstacleHexID = GameManager::getInstance().getAIHelper().blackBoard.npcsCurrentHexID;
    auto bb = GameManager::getInstance().getAIHelper().blackBoard;

    opened.insert(new Record(0, currentHex.ID, HexDirection::CENTER, nullptr, 0));
    do
    {
        auto currentRecord = *(opened.begin());
        currentHex = map.getHexByID(currentRecord->hexID);
        opened.erase(currentRecord);
        closed.insert(currentRecord);

        if (currentHex.ID == hexGoal.ID) {
            stateInfo.pathRecord = std::move(buildPath(currentRecord));
            return;
        }

        for (auto edge : currentHex.edges)
        {
            if (!edge.isBlocked)
            {
                auto adjacenHex = map.getHexByID(edge.leadsToHexID);

                if (! (adjacenHex.isType(HexType::TileAttribute_Uninitialized) || hasBeenVisited(adjacenHex.ID, closed) || isPathObstructed(obstacleHexID, adjacenHex.ID)) ){
                   auto score = Record::CalculateScore(PathHelper::DistanceBetween(hexGoal.position, adjacenHex.position), currentRecord->movementCount+1);
                 //  score += bb.data[adjacenHex.ID];
                   opened.insert(new Record{ score , adjacenHex.ID, edge.direction, currentRecord, currentRecord->movementCount + 1 });
                }
            }
        }
    } while (!opened.empty());

    stateInfo.pathRecord = { };
    return;
}

PathRecord AStar::buildPath(const Record* hr) noexcept
{
    PathRecord pr;

    for (auto path = hr; path->parent != nullptr; path = path->parent)
        pr.push_back({ path->direction, path->hexID });

    return pr;
}

inline bool AStar::hasBeenVisited(const unsigned int id, const AStarRecord& ahr) noexcept {
    return std::find_if(begin(ahr), end(ahr), [&id](Record* hr) {
        return hr->hexID == id;
    }) != end(ahr);
}

inline bool AStar::isPathObstructed(const std::set<unsigned int>& obstacleHexIDs, const unsigned int id) noexcept {
    return std::find(begin(obstacleHexIDs), end(obstacleHexIDs), id) != end(obstacleHexIDs);
}