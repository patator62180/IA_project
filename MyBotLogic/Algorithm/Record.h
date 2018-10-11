#ifndef RECORD_H
#define RECORD_H

#include "globals.h"
#include "../MapStructure/CoordAxial.h"
#include "../Utils/PathHelper.h"

#include <set>

//TODO remove hexID and try having a set<unsigned int, Record*> with compare

struct Record {
    //unsigned int score;
    int score;
    unsigned int hexID;
    HexDirection direction;
    Record* parent;
    const unsigned int movementCount;

    Record::Record(const int score, const unsigned int hexID, const HexDirection direction, Record* parent, const unsigned int movementCount)
        :score{ score }, hexID{ hexID }, direction(direction), parent{ parent }, movementCount{ movementCount }
    {}

    inline static const /*unsigned*/ int CalculateScore(const float& distance, const unsigned int& movementCount) noexcept {
        return static_cast<unsigned int>(distance + movementCount);
    }

    ~Record() {
        parent = nullptr;
        delete parent;
    }
};

struct AStarCompare {
    bool operator()(Record* l, Record* r) const noexcept {
        if (l->hexID == r->hexID)
            return l->score < r->score;

        return l->hexID > r->hexID;
    }
};

//struct AStarCompare {
//    bool operator()(Record* l, Record* r) const noexcept {
//        if (l->hexID == r->hexID)
//            return l->score > r->score;
//
//        return l->score < r->score;
//    }
//};

using AStarRecord = std::set<Record*, AStarCompare>;

//struct AStarCompare {
//    bool operator()(Record* l, Record* r) const noexcept {
//        if (l->hexID == r->hexID)
//            return l->score < r->score;
//
//        return l->hexID > r->hexID;
//    }
//};

#endif // RECORD_H