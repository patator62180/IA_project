#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "../GameObject/Map.h"
#include "../GameObject/Npc.h"
#include "NPCInfo.h"

#include <vector>

using BlackBoardData = std::vector<int>;

class BlackBoard
{
private:
    const int UNITIALIZED = -1;
    const int GOAL_SCORE = 1000;

    const int MIN_REAL_SCORE = 0;
    const int MAX_REAL_SCORE = 5;
    const int MIN_TEMP_SCORE = 10;
    const int MAX_TEMP_SCORE = GOAL_SCORE - 1;

public:
    BlackBoardData data;

    BlackBoard() = default;

    void Init(const size_t& size)
    {
        data = BlackBoardData(size, UNITIALIZED);
    }

    void Update(const Map& map, const TurnInfo& turnInfos) {
        for (auto npcInfo : turnInfos.npcs) {
            auto npcHex = map.getConstHexByID(npcInfo.second.tileID);
            data[npcHex.ID] = getRealVisionScore(npcHex);

            for (auto hexID : npcInfo.second.visibleTiles) {
                if (needUpdate(data[hexID])) {
                    auto hex = map.getConstHexByID(hexID);
                    auto score = hex.isGoal() ? GOAL_SCORE : getTempVisionScore(hex, turnInfos.turnNb);
                    data[hex.ID] = score;
                }
            }
        }
    }

    const int getTempVisionScore(const Hex& h, const unsigned int& turnNb) {
        return getRealVisionScore(h) + turnNb;
    }

    const int getRealVisionScore(const Hex& h) {
        return static_cast<int>(std::count_if(begin(h.edges), end(h.edges), [](const Edge& e) {
            return e.canSeeThrough;
        }));
    }

    const bool needUpdate(const int& score) {
        return score == UNITIALIZED;
    }

    unsigned int getBestHexIDToGo() {
        auto maxIter = std::max_element(begin(data), end(data));
        return static_cast<unsigned int>(std::distance(begin(data), maxIter));
    }

    ~BlackBoard() = default;

    friend std::ostream& operator<<(std::ostream& os, BlackBoard&);
};

inline std::ostream& operator<<(std::ostream& os, BlackBoard& h)
{
    int cpt = 0;
    for (auto data : h.data) {
        os << "hexID:" << cpt++ << ' '
            << "Prob:" << data << std::endl;
    }

    return os;
}

#endif // !BLACKBOARD_H