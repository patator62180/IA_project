#ifndef COOR_AXIAL_H
#define COOR_AXIAL_H

#include <iostream>

struct CoordAxial {
    int x, z;

    CoordAxial(const int x, const int z)
        : x{ x }, z{ z }
    {}

    friend std::ostream& operator<<(std::ostream& os, const CoordAxial&);

    const bool operator==(const CoordAxial& other) const noexcept {
        return x == other.x && z == other.z;
    }
    const CoordAxial operator+(const CoordAxial& other) const noexcept {
        return { x + other.x, z + other.z };
    }
    const CoordAxial operator*(const int& scale) const noexcept {
        return { x * scale, z * scale };
    }
};

inline std::ostream& operator<<(std::ostream& os, const CoordAxial& a)
{
    return os << a.x << ',' << a.z;;
}

#endif // COOR_AXIAL_H