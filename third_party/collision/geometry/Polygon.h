#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <limits>
#include <collision/structures/vec2.hpp>
#include <collision/math/Transform2.h>

using namespace std;

namespace collision
{
class Polygon
{
  public:
    Polygon(initializer_list<collision::vec2<float>> points);
    Polygon(collision::vec2<float> center, initializer_list<collision::vec2<float>> points);
    const collision::vec2<float> &getCenter() const;
    const collision::vec2<float> getFarthestPoint(const collision::vec2<float> direction, const Transform2 &transform) const;

  private:
    float cross(const collision::vec2<float> &, const collision::vec2<float> &) const;
    collision::vec2<float> calcAreaWeightedCenter(const vector<collision::vec2<float>> &points);

  private:
    vector<collision::vec2<float>> vertices;
    collision::vec2<float> center;
};

} // namespace collision
