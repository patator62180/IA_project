#ifndef RECORD_H
#define RECORD_H

#include "Globals.h"

#include <set>

struct Record {
    unsigned int score;
    unsigned int hexID;
    HexDirection direction;
    Record* parent;
    const unsigned int movementCount;

    Record::Record(const unsigned int score, const unsigned int hexID, const HexDirection direction, Record* parent, const unsigned int movementCount)
        :score{ score }, hexID{ hexID }, direction(direction), parent{ parent }, movementCount{ movementCount }
    {}

    inline static unsigned int CalculateScore(const float& distance, const unsigned int movementCount) noexcept {
        return static_cast<unsigned int>(distance + movementCount);
    }

    ~Record() {
        parent = nullptr;
        delete parent;
    }
};

// sort ascending
struct AStarCompare {
    bool operator()(Record* l, Record* r) const noexcept {
        if (l->hexID == r->hexID)
            return l->score < r->score;

        return l->hexID > r->hexID;
    }
};

// sort descending...?
//struct AStarCompare {
//    bool operator()(Record* l, Record* r) const noexcept {
//        if (l->hexID == r->hexID)
//            return l->score > r->score;
//
//        return l->score < r->score;
//    }
//};

using AStarRecord = std::set<Record*, AStarCompare>;

#endif // RECORD_H