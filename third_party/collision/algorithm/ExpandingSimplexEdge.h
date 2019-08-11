#pragma once

#include <collision/structures/vec2.hpp>
#include <collision/math/Vector2Util.h>
#include <memory>

using namespace std;

namespace collision
{

class ExpandingSimplexEdge
{
  public:
    ExpandingSimplexEdge(const collision::vec2<float> point1, const collision::vec2<float> point2, const int32_t winding)
    {
        this->normal = collision::vec2<float>(point2.x - point1.x, point2.y - point1.y);

        if (winding < 0)
        {
            this->normal = Vector2Util::right(normal);
        }
        else
        {
            this->normal = Vector2Util::left(normal);
        }

        // normalize the vector
        this->normal = Vector2Util::normalize(normal);

        // project the first point onto the normal (it doesnt matter which
        // you project since the normal is perpendicular to the edge)
        this->distance = abs(point1.x * normal.x + point1.y * normal.y);
        this->point1 = point1;
        this->point2 = point2;
    }

    const collision::vec2<float> &getPoint1() const
    {
        return point1;
    }

    const collision::vec2<float> &getPoint2() const
    {
        return point2;
    }

    const float getDistance() const
    {
        return distance;
    }

    const collision::vec2<float> &getNormal() const
    {
        return normal;
    }

  private:
    collision::vec2<float> point1;
    collision::vec2<float> point2;
    collision::vec2<float> normal;
    float distance;
};

struct DistancePredicate
{
    bool operator()(const shared_ptr<ExpandingSimplexEdge> &lhs, const shared_ptr<ExpandingSimplexEdge> &rhs) const
    {
        return lhs->getDistance() > rhs->getDistance();
    }
};

} // namespace collision
