 #include "PathHelper.h"

#include <vector>
#include <algorithm>

const std::vector<CoordAxial> generateBaseMovement() noexcept
{
    //enum Direction aka ETilePosition { NE, E, SE, SW, W, NW, CENTER };
    std::vector<CoordAxial> movementForDirection =
    {
        { 1,-1 },
        { 1, 0 },
        { 0, 1 },
        { -1, 1 },
        { -1, 0 },
        { 0, -1 }
    };

    return movementForDirection;
}

const std::vector<CoordAxial> PathHelper::baseMovement = ::generateBaseMovement();
inline const size_t PathHelper::getDirectionCount() noexcept
{
    return baseMovement.size();
}

const HexDirection PathHelper::getReverseDirection(const HexDirection& d) noexcept {
    return static_cast<HexDirection>((d + getDirectionCount()/2) % getDirectionCount());
}

const CoordAxial PathHelper::CalculatePosOffset(const CoordAxial& p, const HexDirection& d) noexcept
{
    return AxialToOffset({ p + baseMovement[d] });
}

const float PathHelper::DistanceBetween(const CoordAxial& cl, const CoordAxial& cr) noexcept {
    auto clY = -cl.x - cl.z;
    auto crY = -cr.x - cr.z;
    return static_cast<float>(sqrt( (cl.x - cr.x) * (cl.x - cr.x) + (clY - crY) * (clY - crY) + (cl.z - cr.z) * (cl.z - cr.z) ));
}

const CoordAxial PathHelper::AxialToOffset(const CoordAxial& p) noexcept {
    auto x = p.x + (p.z - (p.z & 1)) / 2;
    return { x, p.z };
}

//const unsigned int PathHelper::MovementCountBetween(const CoordAxial& cl, const CoordAxial& cr) noexcept {
//    return std::max(abs(cl.x - cr.x), abs(cl.z - cr.z));
//}