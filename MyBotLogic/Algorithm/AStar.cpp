#include "AStar.h"

#include "../GameManager.h"
#include "../Utils/PathHelper.h"
#include "../MapStructure/Hex.h"

#include <algorithm>

// TODO refacto getBestPath & getBestExplorePath into 1 function
PathRecord AStar::FindBestPath(const Npc& npc, const unsigned int& goalHexID) noexcept
{
    AStarRecord opened;
    AStarRecord closed;
    auto map = GameManager::getInstance().getMap();
    auto currentHex = map.getConstHexByID(npc.hexID);
    auto hexGoal = map.getConstHexByID(goalHexID);
    auto obstacleHexID = GameManager::getInstance().getAIHelper().getNpcsCurrentHexID();

    unsigned int movementCount = 0;
    opened.insert(new Record(0, currentHex.ID, HexDirection::CENTER, nullptr, movementCount));

    do
    {
        auto currentRecord = *(opened.begin());
        currentHex = map.getConstHexByID(currentRecord->hexID);
        opened.erase(currentRecord);
        closed.insert(currentRecord);

        if (currentHex.ID == goalHexID || movementCount > npc.movementRange)
            return buildPath(currentRecord);

        ++movementCount;

        for (auto edge : currentHex.edges)
        {
            if (!edge.isBlocked)
            {
                auto adjacenHex = map.getConstHexByID(edge.leadsToHexID);

                if (! (hasBeenVisited(adjacenHex.ID, closed) || isPathObstructed(obstacleHexID, adjacenHex.ID)) ){
                    auto score = Record::CalculateScore(PathHelper::DistanceBetween(hexGoal.position, adjacenHex.position), movementCount);

                    opened.insert(new Record{ score , adjacenHex.ID, edge.direction, currentRecord, movementCount });
                }
            }
        }
    } while (!opened.empty());

    return { { HexDirection::CENTER, npc.hexID } };
}

PathRecord AStar::FindBestExplorePath(const Npc& npc) noexcept
{
    //AStarRecord opened;
    //AStarRecord closed;
    //auto map = GameManager::getInstance().getMap();
    //auto currentHex = map.getCHexById(npc.hexID);
    //auto obstacleHexID = GameManager::getInstance().getAIHelper().getNpcsCurrentHexID();

    //unsigned int movementCount = 0;
    //opened.insert(new Record(0, currentHex.ID, HexDirection::CENTER, nullptr, movementCount));
    //
    //do
    //{
    //    auto currentRecord = *(opened.begin());
    //    currentHex = map.getCHexById(currentRecord->hexID);
    //    opened.erase(currentRecord);
    //    closed.insert(currentRecord);

    //    if (movementCount > npc.visionRange)
    //        return buildPath(currentRecord);

    //    ++movementCount;
    //    for (auto edge : currentHex.edges)
    //    {
    //        auto adjacenHex = map.getCHexById(edge.leadsToHexID);

    //        if (!(hasBeenVisited(adjacenHex.ID, closed) || isPathObstructed(obstacleHexID, adjacenHex.ID)))
    //        {               
    //            //need to calculate score from blackboard
    //            //auto score = Record::calculateScore(PathHelper::DistanceBetween(hexGoal.position, adjacenHex.position), movementCount);
    //            //opened.insert(new Record{ score , adjacenHex.ID, edge.direction, currentRecord, movementCount });
    //        }
    //    }
    //} while (!opened.empty());

    return { { HexDirection::CENTER, npc.hexID } };
}

PathRecord AStar::buildPath(const Record* hr) noexcept
{
    PathRecord pr;

    for (auto path = hr; path->parent != nullptr; path = path->parent)
        pr.push_back({ path->direction, path->hexID });

    return pr;
}

inline const bool AStar::hasBeenVisited(const unsigned int& id, const AStarRecord& ahr) noexcept {
    return std::find_if(begin(ahr), end(ahr), [&id](Record* hr) {
        return hr->hexID == id;
    }) != end(ahr);
}

inline const bool AStar::isPathObstructed(const std::set<unsigned int>& obstacleHexIDs, const unsigned int& id) noexcept {
    return std::find(begin(obstacleHexIDs), end(obstacleHexIDs), id) != end(obstacleHexIDs);
}