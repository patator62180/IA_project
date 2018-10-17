#ifndef EDGE_H
#define EDGE_H

#include "Globals.h"

#include <ostream>

struct Edge
{
    unsigned int leadsToHexID;
    EdgeType type = EdgeType::ObjectType_None;
    HexDirection direction;
    bool isBlocked = false;

    Edge() = default;

    Edge(const unsigned int leadsToHexID, const HexDirection direction, const bool isBlocked)
        :leadsToHexID{ leadsToHexID }, direction{ direction }, isBlocked{ isBlocked }
    {}

    void setPropertyByType(const EdgeType typeToApply) {
        type = typeToApply;
        isBlocked = true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Edge&);
};

inline std::ostream& operator<<(std::ostream& os, const Edge& e)
{
    return os << "Direction:" << e.direction << ' '
              << "LeadsToId:" << e.leadsToHexID;;
}

#endif // EDGE_H