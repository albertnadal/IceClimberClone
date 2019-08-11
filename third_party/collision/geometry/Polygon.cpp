#include <collision/geometry/Polygon.h>
#include <collision/math/Vector2Util.h>
#include <cmath>

using namespace std;

const float INV_3 = 1.0 / 3.0;

namespace collision
{

Polygon::Polygon(initializer_list<collision::vec2<float>> points)
{
    vertices.insert(vertices.end(), points.begin(), points.end());
    center = calcAreaWeightedCenter(vertices);
}

Polygon::Polygon(collision::vec2<float> center, initializer_list<collision::vec2<float>> points)
{
    vertices.insert(vertices.end(), points.begin(), points.end());
    this->center = center;
}

const collision::vec2<float> &Polygon::getCenter() const
{
    return center;
}

const collision::vec2<float> Polygon::getFarthestPoint(const collision::vec2<float> direction, const Transform2 &transform) const
{
    // transform the normal into local space
    collision::vec2<float> localn = transform.getInverseTransformedR(direction);

    // set the farthest point to the first one
    collision::vec2<float> point = vertices[0];

    // prime the projection amount
    float max = (localn.x * vertices[0].x) + (localn.y * vertices[0].y);

    // loop through the rest of the vertices to find a further point along the axis
    size_t size = vertices.size();
    for (uint32_t i = 1; i < size; i++)
    {
        // get the current vertex
        collision::vec2<float> v = vertices[i];

        // project the vertex onto the axis
        float projection = Vector2Util::dot(localn, v);

        // check to see if the projection is greater than the last
        if (projection > max)
        {
            // otherwise this point is the farthest so far so clear the array and add it
            point = v;

            // set the new maximum
            max = projection;
        }
    }

    // transform the point into world space
    transform.transform(point);

    return point;
}

collision::vec2<float> Polygon::calcAreaWeightedCenter(const vector<collision::vec2<float>> &points)
{
    size_t size = points.size();

    // check for list of one point
    if (size == 1)
    {
        collision::vec2<float> p = points[0];
        return p;
    }

    // get the average center
    collision::vec2<float> ac;
    for (uint32_t i = 0; i < size; i++)
    {
        collision::vec2<float> p = points[i];
        ac += p;
    }

    ac *= (1.0 / size);

    // otherwise perform the computation
    collision::vec2<float> center;
    float area = 0.0;

    // loop through the vertices
    for (uint32_t i = 0; i < size; i++)
    {
        // get two vertices
        collision::vec2<float> p1 = points[i];
        collision::vec2<float> p2 = i + 1 < size ? points[i + 1] : points[0];
        p1 -= ac;
        p2 -= ac;

        // perform the cross product (yi * x(i+1) - y(i+1) * xi)
        float d = (p1.x * p2.y) - (p1.y * p2.x);

        // multiply by half
        float triangleArea = 0.5 * d;

        // add it to the total area
        area += triangleArea;

        // area weighted centroid
        // (p1 + p2) * (D / 3)
        // = (x1 + x2) * (yi * x(i+1) - y(i+1) * xi) / 3
        // we will divide by the total area later
        center += (p1 + p2) * INV_3 * triangleArea;
    }

    // check for zero area
    if (abs(area) <= numeric_limits<float>::epsilon())
    {
        // zero area can only happen if all the points are the same point
        // in which case just return a copy of the first
        return points[0];
    }

    // finish the centroid calculation by dividing by the total area
    center *= (1.0 / area);
    center += ac;

    // return the center
    return center;
}

float Polygon::cross(const collision::vec2<float> &p1, const collision::vec2<float> &p2) const
{
    return p1.x * p2.y - p1.y * p2.x;
}

} // namespace collision
