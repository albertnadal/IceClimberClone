#pragma once

#include <vector>
#include <collision/structures/vec2.hpp>

using namespace std;

namespace collision
{

class MinkowskiSum;
struct Penetration;

class EPAMinkowskiPenetrationSolver
{
public:
    EPAMinkowskiPenetrationSolver();
    void findPenetration(const vector<collision::vec2<float>>& simplex, const MinkowskiSum& minkowskiSum, Penetration& penetration) const;
};

} // namespace collision
