#pragma once

#include <collision/structures/vec2.hpp>

namespace collision
{
  struct Penetration
  {
      collision::vec2<int16_t> depth;
  };
}
