#ifndef INFLUENCE_ZONE_H
#define INFLUENCE_ZONE_H

#include "../MapStructure/Hex.h"

#include <set>
#include <vector>
#include <ostream>

struct InfluenceHex {
    unsigned int hexID;
    unsigned int score;

    InfluenceHex(const unsigned int hexID, const unsigned int score)
        :hexID{ hexID }, score{ score }
    {}

    InfluenceHex() = default;
};

using InfluenceData = std::vector<InfluenceHex>;
//using InfluenceDataInTime = std::vector<InfluenceData>;

class InfluenceZone {
private:
    unsigned int radius;

public:
    static const unsigned int GOAL_SCORE = 100;
    InfluenceData dataInTime;
    InfluenceHex currentHighest;

    InfluenceZone() = default;
    InfluenceZone(const unsigned int);

    void Update(const std::set<unsigned int>&);
    InfluenceHex consumeBestLatestHexID();
    bool hasDiscoveredBetterInfluence();

private:
    void createZone(const std::set<unsigned int>&);
};
#endif // !INFLUENCE_ZONE_H