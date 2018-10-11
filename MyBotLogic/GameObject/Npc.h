#ifndef NPC_H
#define NPC_H

#include "Globals.h"

#include <vector>
#include <ostream>

struct Movement {
    HexDirection direction;
    unsigned int toHexID;
};

struct Npc
{
    unsigned int ID;
    unsigned int hexID;
    unsigned int visionRange;
    unsigned int movementRange;
    bool omniscient;

    Npc(const unsigned int& ID, const unsigned int& hID, const unsigned int& vr, const unsigned int& mr, const bool& omniscient)
        : ID{ ID }, hexID{ hID }, visionRange{ vr }, movementRange{ mr }, omniscient{ omniscient }
    {}
    ~Npc() = default;

    friend std::ostream& operator<<(std::ostream&, Npc);

};

inline std::ostream& operator<<(std::ostream &os, Npc n)
{
    os << "Id:" << n.ID << ' ';

    //for (auto movement : n.pathRecord)
    //    os << movement.direction << ':' << movement.toHexID << ' ';

    return os << std::endl;
}

#endif // #ifndef NPC_H