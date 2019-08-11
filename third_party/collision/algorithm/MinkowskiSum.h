#pragma once

#include <collision/structures/vec2.hpp>
#include <collision/geometry/Polygon.h>
#include <collision/math/Transform2.h>

namespace collision
{

class MinkowskiSum
{
  public:
    MinkowskiSum(const Polygon &convex1, const Transform2 &transform1, const Polygon &convex2, const Transform2 &transform2) : convex1(convex1), transform1(transform1), convex2(convex2), transform2(transform2)
    {
    }

    //! Returns the farthest point in the Minkowski sum given the direction.
    //!
    //! @param direction the search direction
    //! @return the point farthest in the Minkowski sum in the given direction
    collision::vec2<float> getSupportPoint(collision::vec2<float> direction) const
    {
        // get the farthest point in the given direction in convex1
        collision::vec2<float> point1 = convex1.getFarthestPoint(direction, transform1);

        // get the farthest point in the opposite direction in convex2
        direction *= -1.0;
        collision::vec2<float> point2 = convex2.getFarthestPoint(direction, transform2);

        // return the Minkowski sum point
        return point1 - point2;
    }

  private:
    const Polygon &convex1;
    const Transform2 &transform1;
    const Polygon &convex2;
    const Transform2 &transform2;
};

} // namespace collision
