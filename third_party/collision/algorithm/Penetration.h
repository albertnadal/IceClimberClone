#pragma once

#include <collision/structures/vec2.hpp>

namespace collision
{
struct Penetration
{
    collision::vec2<float> normal;
    float depth = 0.0f;
};
} // namespace collision
