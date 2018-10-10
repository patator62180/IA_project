#include "Map.h"
#include "ObjectInfo.h"
#include "..\Utils\PathHelper.h"
#include "..\Utils\DebugHelper.h"

#include <sstream>
#include <cassert>
#include <algorithm>

void Map::InitLayout(const int& rowCount_, const int& colCount_) noexcept {
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
    connectHexes();
    updateObjects(ti);

    std::stringstream ss;
    ss << "-----MAP-----" << std::endl << *this;
    DebugHelper::getInstance().Log(ss.str());
}

void Map::updateHexes(const TurnInfo& ti) noexcept
{
    for (auto tile : ti.tiles)
        getHexByID(tile.second.tileID).type = tile.second.tileType;
}

void Map::connectHexes() noexcept
{
    for (auto& hex : layout)
        for (auto i = 0; i < Hex::EDGES_COUNT; ++i)
        {
            auto direction = static_cast<HexDirection>(i);
            auto result = isWorthAdding(hex, direction);
            if (result.first && !hex.edges[direction].isBlocked) {
                hex.edges[direction] = Edge{result.second, direction, !isHexAvailable(result.second) };
            }
            else {
                hex.edges[direction].isBlocked = true;
            }
        }
}

void Map::updateObjects(const TurnInfo& li) noexcept {
    for_each(begin(li.objects), end(li.objects), [&](std::pair<unsigned int, ObjectInfo> pair) {
        auto objectInfo = pair.second;
        if (!objectInfo.objectTypes.count(Object::ObjectType_PressurePlate))
        {
            bool isWindow = objectInfo.objectTypes.count(Object::ObjectType_Window);
            auto& hex = getHexByID(pair.second.tileID);

            hex.edges[objectInfo.position].setVisionProperty(isWindow);

            auto adjacentHexPosOff = PathHelper::CalculatePosOffset(hex.position, objectInfo.position);
            if (isDefinedInLayout(adjacentHexPosOff)) {
                auto& adjacentHex = getHexByID(hexPosOffToID(adjacentHexPosOff));
                adjacentHex.edges[PathHelper::getReverseDirection(objectInfo.position)].setVisionProperty(isWindow);
            }
            
        }
    });
}

const CoordAxial Map::hexIDToAxial(const unsigned int& ID) noexcept {
    int x = ID % colCount;
    int z = ID / colCount;

    return { x - (z - (z & 1)) / 2, z };
}

const unsigned int Map::hexPosOffToID(const CoordAxial& c) noexcept {
    return c.z * colCount + c.x;
}

const std::pair<bool, unsigned int> Map::isWorthAdding(const Hex& hex, const HexDirection& direction) {
    auto neighborPosOff = PathHelper::CalculatePosOffset(hex.position, direction);
    auto hexIDToAdd = hexPosOffToID(neighborPosOff);

    return { isDefinedInLayout(neighborPosOff), hexIDToAdd };
};

const bool Map::isDefinedInLayout(const CoordAxial& p) {
    return !(p.x < 0 || p.x > colCount - 1 || p.z < 0 || p.z > rowCount - 1);
}

Hex& Map::getHexByID(const unsigned int& ID) {
    assert(layout.size() > ID && "hex ID doesn't exist");
    return layout.at(ID);
}

const Hex& Map::getConstHexByID(const unsigned int& ID) const {
    assert(layout.size() > ID && "hex ID doesn't exist");
    return layout.at(ID);
}

const bool Map::isHexAvailable(const unsigned int& ID) noexcept {
    return !getConstHexByID(ID).isForbidden();
}

const Array2D& Map::getLayout() const noexcept {
    return layout;
}