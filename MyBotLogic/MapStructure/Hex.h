#ifndef HEX_H
#define HEX_H

#include "CoordAxial.h"
#include "Globals.h"
#include "Edge.h"

#include <ostream>
#include <vector>
#include <algorithm>

using Edges = std::vector<Edge>;

struct Hex
{
public:
    static const unsigned int EDGES_COUNT = 6;
    unsigned int ID;
    CoordAxial position;
    HexType type = HexType::TileAttribute_Uninitialized;
    Edges edges;

    Hex(const unsigned int ID, const CoordAxial position)
        : ID{ID}, position{position}, edges{ EDGES_COUNT }
    {
        for (int i = 0; i < Hex::EDGES_COUNT; ++i)
            edges[i].direction = static_cast<HexDirection>(i);
    }

    const bool isType(const HexType& typeCompare) const noexcept {
        return type == typeCompare;
    }

    const unsigned int EdgeCountNotBlocked() const noexcept {
        return static_cast<unsigned int>(std::count_if(begin(edges), end(edges), [](const Edge& e) {
            return !e.isBlocked;
        }));
    }

    const bool areAllEdgesBlocked() const noexcept {
        return EdgeCountNotBlocked() == 0;
    }

    const unsigned int EdgeCountType(const EdgeType type) const noexcept {
        return static_cast<unsigned int>(std::count_if(begin(edges), end(edges), [&type](const Edge& e) {
            return e.type == type;
        }));
    }

    friend std::ostream& operator<<(std::ostream& os, Hex&);
};

inline std::ostream& operator<<(std::ostream& os, Hex& h)
{
    os << "Id:" << h.ID << ' '
        << "Pos:" << h.position << std::endl;
 
    for (auto edge : h.edges)
        if (!edge.isBlocked)
            os << edge << std::endl;

    return os;
}

#endif // HEX_H