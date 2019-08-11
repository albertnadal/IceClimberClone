#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <collision/structures/vec2.hpp>
#include <collision/algorithm/ExpandingSimplexEdge.h>

using namespace std;

namespace collision
{

class ExpandingSimplex
{
  public:
    ExpandingSimplex(const vector<vec2<float>> &simplex);
    shared_ptr<ExpandingSimplexEdge> findClosestEdge();
    void expand(vec2<float> vec2);

  private:
    int32_t calcWinding(const vector<vec2<float>> &simplex);

  private:
    int32_t winding;
    priority_queue<shared_ptr<ExpandingSimplexEdge>, vector<shared_ptr<ExpandingSimplexEdge>>, DistancePredicate> edgeQueue;
};

} // namespace collision
