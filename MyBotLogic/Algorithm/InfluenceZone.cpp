#include "InfluenceZone.h"

#include "../MapStructure/CoordAxial.h"
#include "../Algorithm/BlackBoard.h"
#include "../GameManager.h"
#include "../GameObject/Map.h"
#include "../Utils/PathHelper.h"

#include <cassert>
#include <algorithm>

void InfluenceZone::findHighInfluence(const std::set<unsigned int>& seenHexesID)
{
    data.clear();
    auto map = GameManager::getInstance().getMap();
    auto bb = GameManager::getInstance().getAIHelper().blackBoard;

    for (auto ID : seenHexesID) {
        auto hex = map.getHexByID(ID);

        if (bb.isUninitialized(DataType::Score, hex.ID)) {
            switch (hex.type) {

            case HexType::TileAttribute_Default:
                data.push_back({ ID, hex.EdgeCountType(EdgeType::ObjectType_Window) });
                    break;

            case HexType::TileAttribute_Goal:
                data.push_back({ ID, BlackBoard::GOAL_SCORE });
                    break;

            default:
                break;
            }
        }
    }
}

void InfluenceZone::Update(const std::set<unsigned int>& seenHexesID) {
    data.clear();
    findHighInfluence(seenHexesID);
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
        if (GameManager::getInstance().getAIHelper().isHexIDOccupied(result.hexID)) {
            data.erase(maxIter);
            return consumeBestInfluence();
        }
    }

    return result;
}