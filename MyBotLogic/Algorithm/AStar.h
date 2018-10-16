#ifndef A_STAR_H
#define A_STAR_H

#include "Record.h"
#include "../Algorithm/StateMachine.h"

#include <map>

using PathRecord = std::vector<Movement>;

class AStar
{
public:
    static void SetBestPath(NpcStateInfo&) noexcept;
    static void SetNearestUnvisited(NpcStateInfo&) noexcept;

private:
    static PathRecord buildPath(const Record*) noexcept;

    static const bool hasBeenVisited(const unsigned int, const AStarRecord&) noexcept;
    static const bool isPathObstructed(const std::set<unsigned int>&, const unsigned int) noexcept;
};

#endif // A_STAR_H
