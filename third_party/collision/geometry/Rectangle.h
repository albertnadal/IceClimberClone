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
class Rectangle
{
  public:
    float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
    vector<collision::vec2<float>> vertices;
    Rectangle(vector<collision::vec2<float>> points);
    Rectangle(initializer_list<collision::vec2<float>> points);
    void Print();
  private:
    void SearchTopLeftAndBottomRightVertices();
};

} // namespace collision
