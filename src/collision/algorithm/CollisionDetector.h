#pragma once

#include <memory>
#include <vector>
#include <collision/algorithm/Penetration.h>
#include <collision/structures/vec2.hpp>
#include <collision/geometry/Rectangle.h>
#include <position.h>

namespace collision {

    class CollisionDetector
    {
        public:
        CollisionDetector();
        bool detect(Rectangle &rectangle1, Rectangle &rectangle2, Penetration &penetration, vec2<int16_t> &vectorDirection);
        void calculateNonCollidingPosition(std::vector<collision::Rectangle> &targetRectangles,
                                           std::vector<collision::Rectangle> &movingRectangles, Position &position);
    };

}
