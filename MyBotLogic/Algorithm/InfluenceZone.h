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

class InfluenceZone {

public:
    InfluenceData data;

    InfluenceZone() = default;

    void Update(const std::set<unsigned int>&);
    InfluenceHex consumeBestInfluence();

private:
    void findHighInfluence(const std::set<unsigned int>&);
};
#endif // !INFLUENCE_ZONE_H