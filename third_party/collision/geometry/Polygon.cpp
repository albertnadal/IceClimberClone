#include <collision/geometry/Polygon.h>
#include <collision/math/Vector2Util.h>
#include <cmath>
#include <iostream>

using namespace std;

const float INV_3 = 1.0 / 3.0;

struct LinearEquation { bool infinite_slope; float slope; float y_intercept; float x; };

namespace collision
{

Polygon::Polygon(vector<collision::vec2<float>> points)
{
    vertices.insert(vertices.end(), points.begin(), points.end());
    center = calcAreaWeightedCenter(vertices);
    //applyPadding(0.5f);
}

Polygon::Polygon(initializer_list<collision::vec2<float>> points)
{
    vertices.insert(vertices.end(), points.begin(), points.end());
    center = calcAreaWeightedCenter(vertices);
    //applyPadding(0.5f);
}

Polygon::Polygon(collision::vec2<float> center, initializer_list<collision::vec2<float>> points)
{
    vertices.insert(vertices.end(), points.begin(), points.end());
    this->center = center;
    //applyPadding(0.5f);
}

const void Polygon::Print()
{
  size_t size = vertices.size();
  for (uint32_t i = 0; i < size; i++)
  {
      collision::vec2<float> v = vertices[i];
      std::cout << "(" << v.x << "," << v.y << ") ";
  }
  std::cout << endl;
}

const void Polygon::applyPadding(const float padding)
{
  // Get the lineal equation of all polygon edges
  vector<LinearEquation> edges;
  uint32_t total_vertices = vertices.size();
  for (uint32_t i = 0; i < total_vertices; i++)
  {
    collision::vec2<float>& current_vertice = vertices.at(i);
    collision::vec2<float> next_vertice;
    if(i == total_vertices-1) {
      next_vertice = vertices.at(0);
    } else {
      next_vertice = vertices.at(i+1);
    }

    LinearEquation current_edge = { false, 0.0f, 0.0f, 0.0f };
    float denominador = (current_vertice.x - next_vertice.x);
    if(denominador == 0.0f) {
      current_edge.infinite_slope = true;
      current_edge.x = current_vertice.x;
    } else {
      float numerador = (current_vertice.y - next_vertice.y);
      float slope = (current_vertice.y - next_vertice.y) / (current_vertice.x - next_vertice.x); // aka tangent
      slope = abs(slope) == 0.0f ? 0.0f : slope;
      float y_intercept = (slope * -current_vertice.x) + current_vertice.y;
      current_edge.slope = slope;
      current_edge.y_intercept = y_intercept;
    }
    edges.push_back(current_edge);
  }

  // Get the lineal equation of all polygon parallel edges with a certain distance (padding)
  vector<LinearEquation> parallel_edges;
  uint32_t total_edges = edges.size();
  for (uint32_t i = 0; i < total_edges; i++)
  {
    LinearEquation& current_edge = edges.at(i);
    LinearEquation parallel_edge = { false, 0.0f, 0.0f, 0.0f };

    if(current_edge.infinite_slope) {
      parallel_edge.infinite_slope = true;
      parallel_edge.x = (center.x >= current_edge.x) ? current_edge.x-padding : current_edge.x+padding;
    } else {
      LinearEquation parallel_center_equation = { false, current_edge.slope, 0.0f, 0.0f };
      float center_y_intercept = (parallel_center_equation.slope * -center.x) + center.y;
      float updated_padding = (center_y_intercept >= current_edge.y_intercept) ? -padding : padding;
      parallel_edge.y_intercept =  updated_padding * sqrt((current_edge.slope*current_edge.slope) + 1.0f);
      parallel_edge.y_intercept += current_edge.y_intercept;
      parallel_edge.slope = current_edge.slope;
    }
    parallel_edges.insert(parallel_edges.begin(), parallel_edge);
  }

  // Get the intersection points of calculated parallel edges and update polygon vertices
  vertices.clear();
  for (uint32_t i = 0; i < total_edges; i++)
  {
    LinearEquation& current_edge = parallel_edges.at(i);
    LinearEquation next_edge;
    if(i == total_edges-1) {
      next_edge = parallel_edges.at(0);
    } else {
      next_edge = parallel_edges.at(i+1);
    }

    float x = 0.0f, y = 0.0f;

    if(current_edge.infinite_slope) {
      x = current_edge.x;
      y = (next_edge.slope * x) + next_edge.y_intercept;
    } else if(next_edge.infinite_slope) {
      x = next_edge.x;
      y = (current_edge.slope * x) + current_edge.y_intercept;
    } else {
      x = (next_edge.y_intercept - current_edge.y_intercept) / (current_edge.slope - next_edge.slope);
      y = (current_edge.slope * x) + current_edge.y_intercept;
    }

    vertices.push_back(vec2<float>(x, y));
  }
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
