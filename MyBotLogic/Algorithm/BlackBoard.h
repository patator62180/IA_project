#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "StateMachine.h"
#include "InfluenceZone.h"

#include <vector>
#include <ostream>

using BlackBoardData = std::vector<unsigned int>;

struct Goal {
    unsigned int hexID;
    std::pair<bool, unsigned int> available{ true, 0 };

    Goal(const unsigned int hexID)
        :hexID{ hexID }
    {};

    ~Goal() = default;
};
using Goals = std::vector<Goal>;

class BlackBoard
{
    friend class InfluenceZone;
private:
    static const unsigned int UNINITIALIZED = 0;
    static const unsigned int MIN_VISITED_SCORE = 1;
    static const unsigned int MAX_VISITED_SCORE = MIN_VISITED_SCORE + Hex::EDGES_COUNT;


    static const unsigned int BASE_INFLUENCE_SCORE = 900;
    static const unsigned int ASTAR_MAX_SCORE = BASE_INFLUENCE_SCORE - 1;

    Goals goals;
public:

    static const unsigned int GOAL_SCORE = 1000;
    BlackBoardData data;

    BlackBoard() = default;
    void Init(const size_t&) noexcept;

    void UpdateNpc(NpcStateInfo&, const std::set<unsigned int>&) noexcept;
    void UpdateGoal(const unsigned int) noexcept;
    void setBestPath(NpcStateInfo&);
    void setBestPathToUnvisited(NpcStateInfo&);

    bool isUnvisited(const unsigned int) const noexcept;
    bool isHighValue(const unsigned int) const noexcept;

    InfluenceHex getBestGoal(const unsigned int);

    ~BlackBoard() = default;
    friend std::ostream& operator<<(std::ostream& os, BlackBoard&);

private:
    bool hasBeenVisited(const unsigned int) const;
};

inline std::ostream& operator<<(std::ostream& os, BlackBoard& h)
{
    int cpt = 0;
    for (auto score : h.data) {
        if(score != 0)
            os  << " HexID:" << cpt << ' '
                << " Score:" << score << std::endl;

        ++cpt;
    }

    return os;
}

#endif // !BLACKBOARD_H