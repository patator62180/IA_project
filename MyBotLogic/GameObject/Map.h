#ifndef MAP_H
#define MAP_H

#include "../MapStructure/CoordAxial.h"
#include "../MapStructure/Hex.h"
#include "LevelInfo.h"
#include "TurnInfo.h"

#include <vector>

using Array2D = std::vector<Hex>;

class Map
{
private:

private:
    int rowCount;
    int colCount;

    Array2D layout;
public:
    Map() = default;

    void InitLayout(const int&, const int&) noexcept;
    void Update(const TurnInfo&);
 
    Array2D& Map::getLayout() noexcept;
    const Array2D& getLayout() const noexcept;

    
    Hex& getHexByID(const unsigned int);
    const Hex& getHexByID(const unsigned int) const;
    const Hex& getHexByPosOff(const CoordAxial&) const;

    const std::pair<bool, unsigned int> isWorthAdding(const Hex&, const HexDirection);

    ~Map() = default;
    friend std::ostream& operator<<(std::ostream&, Map&);
private:
    void updateHexes(const TurnInfo&);
    void connectHex(const TurnInfo&) noexcept;
    void updateObjects(const TurnInfo&) noexcept;

    const CoordAxial hexIDToAxial(const unsigned int ID) noexcept;
    const unsigned int hexPosOffToID(const CoordAxial&) const noexcept;

    const bool isDefinedInLayout(const CoordAxial&);

    const bool isHexAvailable(const unsigned int) noexcept;
};

inline std::ostream& operator<<(std::ostream &os, Map &m)
{
    for (auto elem : m.layout)
        if(!elem.isType(HexType::TileAttribute_Uninitialized))
            os << elem << std::endl;

    return os;
}

#endif //MAP_H
