#include <collision/algorithm/ExpandingSimplex.h>

using namespace std;

namespace collision
{

ExpandingSimplex::ExpandingSimplex(const vector<collision::vec2<float>> &simplex)
{
    this->winding = calcWinding(simplex);
    size_t size = simplex.size();
    for (uint32_t i = 0; i < size; i++)
    {
        int j = i + 1 == size ? 0 : i + 1;

        // create the edge
        collision::vec2<float> a = simplex[i];
        collision::vec2<float> b = simplex[j];
        edgeQueue.push(shared_ptr<ExpandingSimplexEdge>(new ExpandingSimplexEdge(a, b, this->winding)));
    }
}

shared_ptr<ExpandingSimplexEdge> ExpandingSimplex::findClosestEdge()
{
    return edgeQueue.top();
}

void ExpandingSimplex::expand(collision::vec2<float> point)
{
    // remove the edge we are splitting
    shared_ptr<ExpandingSimplexEdge> edge = edgeQueue.top();
    edgeQueue.pop();

    // create two new edges
    shared_ptr<ExpandingSimplexEdge> edge1(new ExpandingSimplexEdge(edge->getPoint1(), point, winding));
    edgeQueue.push(edge1);

    shared_ptr<ExpandingSimplexEdge> edge2(new ExpandingSimplexEdge(point, edge->getPoint2(), winding));
    edgeQueue.push(edge2);
}

int32_t ExpandingSimplex::calcWinding(const vector<collision::vec2<float>> &simplex)
{
    size_t size = simplex.size();
    for (uint32_t i = 0; i < size; i++)
    {
        int j = i + 1 == size ? 0 : i + 1;

        collision::vec2<float> a = simplex[i];
        collision::vec2<float> b = simplex[j];

        float aCrossB = Vector2Util::cross(a, b);
        if (aCrossB > 0)
        {
            return 1;
        }
        else if (aCrossB < 0)
        {
            return -1;
        }
    }

    return 0;
}

} // namespace collision
