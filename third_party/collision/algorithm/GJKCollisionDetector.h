#pragma once

#include <memory>
#include <vector>
#include <collision/algorithm/Penetration.h>
#include <collision/algorithm/EPAMinkowskiPenetrationSolver.h>
#include <collision/structures/vec2.hpp>
#include <collision/geometry/Polygon.h>
#include <collision/math/Transform2.h>

using namespace std;

namespace collision
{

class MinkowskiSum;

class GJKCollisionDetector
{
  public:
    GJKCollisionDetector();
    bool detect(const Polygon &convex1, const Polygon &convex2, Penetration& penetration);
    bool translationAndDetect(const Polygon &convex1, const Polygon &convex2);

  private:
    std::unique_ptr<EPAMinkowskiPenetrationSolver> penetrationSolver;
    collision::vec2<float> calcInitialDirection(const Polygon &convex1, const Transform2 &transform1, const Polygon &convex2, const Transform2 &transform2);
    bool detect(const MinkowskiSum &minkowskiSum, vector<collision::vec2<float>> &simplex, collision::vec2<float> &direction);
    bool checkSimplex(vector<collision::vec2<float>> &simplex, collision::vec2<float> &direction);
};

} // namespace collision
