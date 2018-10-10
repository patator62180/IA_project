#ifndef PATH_HELPER_H
#define PATH_HELPER_H

#include "..\MapStructure\CoordAxial.h"
#include "Globals.h"

#include <vector>

class PathHelper {
    static const std::vector<CoordAxial> baseMovement;

public:
    static const size_t getDirectionCount() noexcept;
    static const HexDirection getReverseDirection(const HexDirection&) noexcept;

    static const CoordAxial CalculatePosOffset(const CoordAxial&, const HexDirection&) noexcept;
    static const CoordAxial PathHelper::AxialToOffset(const CoordAxial&) noexcept;

    static const float DistanceBetween(const CoordAxial&, const CoordAxial&) noexcept;
   // static const unsigned int MovementCountBetween(const CoordAxial&, const CoordAxial&) noexcept;
};

#endif // PATH_HELPER_H