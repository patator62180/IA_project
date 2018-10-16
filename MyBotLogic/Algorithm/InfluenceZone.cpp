#include "InfluenceZone.h"

#include "../MapStructure/CoordAxial.h"
#include "../Algorithm/BlackBoard.h"
#include "../GameManager.h"
#include "../GameObject/Map.h"
#include "../Utils/PathHelper.h"

#include <cassert>
#include <algorithm>

void InfluenceZone::createZone(const std::set<unsigned int>& seenHexesID)
{
    data.clear();
    auto map = GameManager::getInstance().getMap();
    unsigned int influence = 0;

    for (auto ID : seenHexesID) {
        auto hex = map.getHexByID(ID);

        switch (hex.type) {

        case HexType::TileAttribute_Default:
            influence = hex.EdgeCountType(EdgeType::ObjectType_Window);
            break;

        case HexType::TileAttribute_Goal:
            influence = BlackBoard::GOAL_SCORE;
            GameManager::getInstance().getAIHelper().bb.UpdateGoal(hex.ID);
            break;

        default:
            break;
        }

        if (influence > 0)
            data.push_back({ ID, influence });   
    }
}

void InfluenceZone::Update(const std::set<unsigned int>& seenHexesID) {
    data.clear();
    createZone(seenHexesID);
}


InfluenceHex InfluenceZone::consumeBestInfluence() {
    InfluenceHex result{ 0,0 };

    if (!data.empty())
    {
        auto maxIter = std::max_element(begin(data), end(data), [](const InfluenceHex& r, const InfluenceHex& l) {
            return r.score < l.score;
        });
        result = *maxIter;

        //if there is an npc on that hex or hex has been visited, find the next best hex
        if (GameManager::getInstance().getAIHelper().isHexIDOccupied(result.hexID) || !GameManager::getInstance().getAIHelper().bb.isUnvisited(result.hexID)) {
            data.erase(maxIter);
            return consumeBestInfluence();
        }
    }

    return result;
}