#ifndef PATH_HELPER_H
#define PATH_HELPER_H

#include "..\MapStructure\CoordAxial.h"
#include "Globals.h"

#include <vector>

class PathHelper {
    static const std::vector<CoordAxial> baseMovement;

public:
    static const size_t getDirectionCount() noexcept;
    static const HexDirection getReverseDirection(const HexDirection) noexcept;

    static const CoordAxial CalculateDirectionPos(const HexDirection) noexcept;
    static const CoordAxial CalculatePos(const CoordAxial&, const HexDirection) noexcept;
    static const CoordAxial CalculatePosOffset(const CoordAxial&, const HexDirection) noexcept;
    static const CoordAxial PathHelper::AxialToOffset(const CoordAxial&) noexcept;

    static float DistanceBetween(const CoordAxial&, const CoordAxial&) noexcept;
    static unsigned int HexCountBetween(const CoordAxial& cl, const CoordAxial& cr) noexcept;
};

#endif // PATH_HELPER_H