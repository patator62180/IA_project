#ifndef HEX_H
#define HEX_H

#include "CoordAxial.h"
#include "Globals.h"
#include "Edge.h"

#include <ostream>
#include <vector>
#include <algorithm>

struct Hex
{
    static const unsigned int EDGES_COUNT = 6;
    unsigned int ID;
    CoordAxial position;
    HexType type = HexType::TileAttribute_Forbidden;
    std::vector<Edge> edges;

    Hex(const unsigned int ID, const CoordAxial position)
        : ID{ID}, position{position}, edges{ EDGES_COUNT }
    {}

    const bool isForbidden() const noexcept {
        return type == HexType::TileAttribute_Forbidden;
    }

    const bool isGoal() const noexcept {
        return type == HexType::TileAttribute_Goal;
    }

    const float getBaseVisionScore() {
        auto edgesValidCount = std::count_if(begin(edges), end(edges), [](const Edge& e) {
            return e.canSeeThrough;
        });

        return edgesValidCount / static_cast<float>(EDGES_COUNT);
    }

    friend std::ostream& operator<<(std::ostream& os, Hex&);
};

inline std::ostream& operator<<(std::ostream& os, Hex& h)
{
    os << "Id:" << h.ID << ' '
        << "Pos:" << h.position << std::endl;
 
    for (auto edge : h.edges)
        if (!edge.isBlocked)
       // if (edge.canSeeThrough)
            os << edge << std::endl;

    return os;
}

#endif // HEX_H