#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "../GameObject/Npc.h"
#include "../MapStructure/Hex.h"
#include "TurnInfo.h"

#include <vector>

using BlackBoardData = std::vector<int>;

using InfluenceHex = std::pair<unsigned int, int>;
using InfluenceMap = std::set<InfluenceHex>;

struct Goal {
    unsigned int hexID;
    bool available;

    Goal(const unsigned int& hexID)
        : hexID{ hexID }, available{ true }
    {}
};
using Goals = std::vector<Goal>;

class BlackBoard
{
private:
    const int UNINITIALIZED = -1;
    const int GOAL_SCORE = 100000;

    const int MIN_REAL_SCORE = 0;
    const int MAX_REAL_SCORE = 6;
    const int MIN_TEMP_SCORE = 10;
    const int MAX_TEMP_SCORE = GOAL_SCORE - 1;

    const int TURN_WEIGHT = 10;
   // const int TURN_WEIGHT = Hex::EDGES_COUNT;

    Goals goals;

public:
    BlackBoardData data;

    BlackBoard() = default;
    void Init(const size_t&) noexcept;

    void UpdateNpcsVision(const TurnInfo&) noexcept;
    void UpdateNpcTile(const unsigned int&) noexcept;
    void UpdateGoalTile(const unsigned int&) noexcept;

    const int getTempVisionScore(const Hex&, const unsigned int& turnNb) const noexcept;
    const int getRealVisionScore(const Hex&) const noexcept;

    const bool needUpdate(const int& score) noexcept;
    const unsigned int getBestHexIDToGo(const Npc&) noexcept;

    const bool isRealScore(const int&);

    ~BlackBoard() = default;

    friend std::ostream& operator<<(std::ostream& os, BlackBoard&);

private:
    unsigned int FindClosestGoal(const Npc&);
};

inline std::ostream& operator<<(std::ostream& os, BlackBoard& h)
{
    int cpt = 0;
    for (auto data : h.data) {
        if (data != h.UNINITIALIZED)
            os << "hexID:" << cpt++ << ' '
            << "Prob:" << data << std::endl;
        else
            ++cpt;
    }

    return os;
}

#endif // !BLACKBOARD_H