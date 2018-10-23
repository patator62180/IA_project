#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "StateMachine.h"

#include <vector>
#include <set>
#include <ostream>

using BlackBoardData = std::vector<std::vector<unsigned int>>;

enum DataType { Score, Flood, COUNT };

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
private:
    static const unsigned int UNINITIALIZED = 0;
    static const unsigned int BASE_INFLUENCE_SCORE = 100;

    Goals goals;
public:

    static const unsigned int GOAL_SCORE = 1000;
    BlackBoardData data;
    std::set<unsigned int> npcsCurrentHexID;

    BlackBoard() = default;
    void Init(const size_t, const size_t) noexcept;

    void UpdateNpc(NpcStateInfo&, const std::set<unsigned int>&) noexcept;
    void AddGoal(const unsigned int) noexcept;

    bool isUninitialized(const DataType, const unsigned int) const noexcept;
    bool isGoalReachable(const unsigned int , const unsigned int);
    void setNpcObjectiveHexID(NpcStateInfo&);
    bool isNextMovementValid(NpcStateInfo&);
    ~BlackBoard() = default;
    friend std::ostream& operator<<(std::ostream& os, BlackBoard&);

private:
    //bool hasBeenVisited(const DataType, const unsigned int) const;
    void updateScoreData(const std::set<unsigned int>&);
    void updateFloodData(NpcStateInfo&);

    bool findClosestGoal(NpcStateInfo&);
    bool isFloodedByAnotherNpc(const unsigned int, const unsigned int) const noexcept;
};

inline std::ostream& operator<<(std::ostream& os, BlackBoard& h)
{
    //int cpt = 0;
    //for (auto score : h.data) {
    //    if(score != 0)
    //        os  << " HexID:" << cpt << ' '
    //            << " Score:" << score << std::endl;

    //    ++cpt;
    //}

    return os;
}

#endif // !BLACKBOARD_H