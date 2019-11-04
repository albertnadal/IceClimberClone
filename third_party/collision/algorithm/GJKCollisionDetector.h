#pragma once

#include <memory>
#include <vector>
#include <collision/algorithm/Penetration.h>
#include <collision/structures/vec2.hpp>
#include <collision/geometry/Rectangle.h>

using namespace std;

namespace collision
{

class GJKCollisionDetector
{
  public:
    GJKCollisionDetector();
    bool detect(Rectangle &rectangle1, Rectangle &rectangle2, Penetration &penetration, vec2<int16_t> &vectorDirection);
};

} // namespace collision
