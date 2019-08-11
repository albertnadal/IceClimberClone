#pragma once

#include <collision/structures/vec2.hpp>
#include <cmath>

using namespace std;

namespace collision
{

class Vector2Util
{
  public:
    static float inversesqrt(float x)
    {
        return static_cast<float>(1) / sqrt(x);
    }

    static collision::vec2<float> normalize(collision::vec2<float> const &x)
    {
        return x * Vector2Util::inversesqrt(Vector2Util::dot(x, x));
    }

    static float dot(collision::vec2<float> const &x, collision::vec2<float> const &y)
    {
        collision::vec2<float> tmp(x * y);
        return tmp.x + tmp.y;
    }

    static float cross(const collision::vec2<float> &p1, const collision::vec2<float> &p2)
    {
        return p1.x * p2.y - p1.y * p2.x;
    }

    //!
    //! Computes:
    //! b * (a . c) - a * (b . c)
    //!
    static collision::vec2<float> tripleProduct(const collision::vec2<float> &a, const collision::vec2<float> &b, const collision::vec2<float> &c)
    {
        return (b * Vector2Util::dot(a, c)) - (a * Vector2Util::dot(b, c));
    }

    //!
    //! Magnitude squared:
    //! v.x * v.x + v.y * v.y
    //!
    static float magnitudeSquared(const collision::vec2<float> &v)
    {
        return v.x * v.x + v.y * v.y;
    }

    //! Returns the left-handed normal of this vector.
    static collision::vec2<float> left(const collision::vec2<float> &v)
    {
        return collision::vec2<float>(v.y, -v.x);
    }

    //! Returns the right-handed normal of this vector.
    static collision::vec2<float> right(const collision::vec2<float> &v)
    {
        return collision::vec2<float>(-v.y, v.x);
    }

    static bool isZero(const collision::vec2<float> &v)
    {
        return abs(v.x) <= numeric_limits<float>::epsilon() && abs(v.y) <= numeric_limits<float>::epsilon();
    }
};

} // namespace collision
