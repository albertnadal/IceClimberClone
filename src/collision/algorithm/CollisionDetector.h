#pragma once

#include <memory>
#include <vector>
#include <collision/algorithm/Penetration.h>
#include <collision/structures/vec2.hpp>
#include <collision/geometry/Rectangle.h>
#include <position.h>

namespace collision {

    class CollisionDetector {
    public:
        CollisionDetector();

        bool
        checkCollision(Rectangle &rectangle_a, Rectangle &rectangle_b, Penetration &penetration, vec2<int16_t> &vectorDirection);

        void updateWithNonCollidingPosition(std::vector<collision::Rectangle> &targetRectangles,
                                            std::vector<collision::Rectangle> &movingRectangles,
                                            Position &position,
                                            vec2<int16_t> &vectorDirection);
    };

}
