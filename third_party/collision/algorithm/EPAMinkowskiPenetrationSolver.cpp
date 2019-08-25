#include <collision/algorithm/EPAMinkowskiPenetrationSolver.h>
#include <collision/algorithm/ExpandingSimplex.h>
#include <collision/algorithm/MinkowskiSum.h>
#include <collision/algorithm/Penetration.h>
#include <collision/math/Vector2Util.h>

using std::unique_ptr;

namespace collision
{

const int MAX_ITERATIONS = 100;

EPAMinkowskiPenetrationSolver::EPAMinkowskiPenetrationSolver()
{
}

void EPAMinkowskiPenetrationSolver::findPenetration(const vector<collision::vec2<float>>& simplex, const MinkowskiSum& minkowskiSum, Penetration& penetration) const
{
    ExpandingSimplex expandingSimplex(simplex);
    shared_ptr<ExpandingSimplexEdge> edge;
    collision::vec2<float> point;

    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        edge = expandingSimplex.findClosestEdge();
        point = minkowskiSum.getSupportPoint(edge->getNormal());

        float projection = Vector2Util::dot(point, edge->getNormal());
        if ((projection - edge->getDistance()) < std::numeric_limits<float>::epsilon())
        {
            penetration.normal = edge->getNormal();
            penetration.depth = projection;
            return;
        }

        expandingSimplex.expand(point);
    }

    penetration.normal = edge->getNormal();
    penetration.depth = Vector2Util::dot(point, edge->getNormal());
}

} // namespace collision
