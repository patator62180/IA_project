#include "InfluenceZone.h"

#include "../MapStructure/CoordAxial.h"
#include "../Algorithm/BlackBoard.h"
#include "../GameManager.h"
#include "../GameObject/Map.h"
#include "../Utils/PathHelper.h"

#include <cassert>
#include <algorithm>

InfluenceZone::InfluenceZone(const unsigned int radius)
    :radius{ radius }
{
}

void InfluenceZone::createZone(const std::set<unsigned int>& seenHexesID)
{
    dataInTime.clear();
    auto map = GameManager::getInstance().getMap();
    unsigned int influence = 0;

    for (auto ID : seenHexesID) {
        auto hex = map.getHexByID(ID);

        switch (hex.type) {

        case HexType::TileAttribute_Default:
            influence = hex.EdgeCountType(EdgeType::ObjectType_Window);
            break;

        case HexType::TileAttribute_Goal:
            influence = GOAL_SCORE;
            GameManager::getInstance().getAIHelper().bb.UpdateGoal(hex.ID);
            break;

        default:
            break;
        }

        if (influence > 0)
            dataInTime.push_back({ ID, influence });   
    }
}

void InfluenceZone::Update(const std::set<unsigned int>& seenHexesID) {
    createZone(seenHexesID);
}


InfluenceHex InfluenceZone::consumeBestLatestHexID() {

    InfluenceHex result{ 0,0 };

    if (!dataInTime.empty())
    {
        auto maxIter = std::max_element(begin(dataInTime), end(dataInTime), [](const InfluenceHex& r, const InfluenceHex& l) {
            return r.score < l.score;
        });
        result = *maxIter;

        dataInTime.erase(std::remove_if(begin(dataInTime), end(dataInTime), [&maxIter](InfluenceHex& ih) {
            return ih.hexID == maxIter->hexID;
        }), end(dataInTime));

        if (GameManager::getInstance().getAIHelper().isHexIDOccupied(result.hexID))
            return consumeBestLatestHexID();
    }

    return result;
}

bool InfluenceZone::hasDiscoveredBetterInfluence() {
    if (dataInTime.empty())
        return false;

    auto maxIter = std::max_element(begin(dataInTime), end(dataInTime), [](const InfluenceHex& r, const InfluenceHex& l) {
        return r.score < l.score;
    });

    return maxIter->hexID != currentHighest.hexID && maxIter->score > currentHighest.score;
}