#include "Map.h"
#include "..\Utils\PathHelper.h"
#include "..\Utils\DebugHelper.h"

#include <sstream>
#include <cassert>
#include <algorithm>

class HexTypeNotHandle {};

void Map::InitLayout(const int rowCount_, const int colCount_) noexcept {
    rowCount = rowCount_;
    colCount = colCount_;
    layout.reserve(rowCount*colCount);

    int offset = 1;
    for (auto i = 0; i < rowCount; ++i)
    {
        offset -= !(i & 1);
        unsigned int indexI = i * colCount;

        for (auto j = 0; j < colCount; ++j)
        {
            layout.emplace_back( Hex{
                     indexI + j,
                    { offset + j,  i },
                } );
        }
    }
}

void Map::Update(const TurnInfo& ti) {
    updateHexes(ti);
    updateObjects(ti);

    //std::stringstream ss;
    //ss << "-----MAP-----" << std::endl << *this;
    //DebugHelper::getInstance().Log(ss.str());
}

void Map::updateHexes(const TurnInfo& ti)
{
    connectHex(ti);

    for (auto tile : ti.tiles) {
        auto& hex = getHexByID(tile.second.tileID);

        if (hex.isType(HexType::TileAttribute_Uninitialized)) {
            hex.type = tile.second.tileType;

            if (!hex.areAllEdgesBlocked()) {
                switch (hex.type) {
                case HexType::TileAttribute_Goal:
                    GameManager::getInstance().getAIHelper().blackBoard.AddGoal(hex.ID);
                    break;

                case HexType::TileAttribute_Forbidden:
                    for (auto edge : hex.edges)
                        getHexByID(edge.leadsToHexID).edges[PathHelper::getReverseDirection(edge.direction)].isBlocked = true;
                    break;

                default:
                    break;
                }
            }
        }
    }
}

void Map::connectHex(const TurnInfo& ti) noexcept
{
    for (auto tile : ti.tiles) {
        auto& hex = getHexByID(tile.second.tileID);

        for (auto edge : hex.edges)
            if (!edge.isBlocked) {
                auto result = isWorthAdding(hex, edge.direction);

                if (result.first)
                    hex.edges[edge.direction] = Edge{ result.second, edge.direction, false };
                else
                    hex.edges[edge.direction].isBlocked = true;
            }
    }
}

void Map::updateObjects(const TurnInfo& li) noexcept {
    for_each(begin(li.objects), end(li.objects), [&](std::pair<unsigned int, ObjectInfo> pair) {
        auto& hex = getHexByID(pair.second.tileID);
        for (auto edgeType : pair.second.objectTypes) {
            hex.edges[pair.second.position].setPropertyByType(edgeType);
        }
              
        auto adjacentHexPosOff = PathHelper::CalculatePosOffset(hex.position, pair.second.position);
        for (auto edgeType : pair.second.objectTypes) {  
            if (isDefinedInLayout(adjacentHexPosOff)) {
                auto& adjacentHex = getHexByID(hexPosOffToID(adjacentHexPosOff));
                adjacentHex.edges[PathHelper::getReverseDirection(pair.second.position)].setPropertyByType(edgeType);
            }
        }
    });
}

unsigned int Map::hexPosOffToID(const CoordAxial& c) const noexcept {
    return c.z * colCount + c.x;
}

const std::pair<bool, unsigned int> Map::isWorthAdding(const Hex& hex, const HexDirection direction) {
    auto neighborPosOff = PathHelper::CalculatePosOffset(hex.position, direction);
    auto hexIDToAdd = hexPosOffToID(neighborPosOff);

    return { isDefinedInLayout(neighborPosOff), hexIDToAdd };
};

bool Map::isDefinedInLayout(const CoordAxial& p) {
    return !(p.x < 0 || p.x > colCount - 1 || p.z < 0 || p.z > rowCount - 1);
}

Hex& Map::getHexByID(const unsigned int ID) {
    return layout[ID];
}

const Hex& Map::getHexByID(const unsigned int ID) const {
    return layout[ID];
}

const Hex& Map::getHexByPosOff(const CoordAxial& p) const {
    return getHexByID(hexPosOffToID(p));
}

const bool Map::isHexAvailable(const unsigned int ID) noexcept {
    return !getHexByID(ID).isType(HexType::TileAttribute_Forbidden);;
}

const Array2D& Map::getLayout() const noexcept {
    return layout;
}


Array2D& Map::getLayout() noexcept {
    return layout;
}