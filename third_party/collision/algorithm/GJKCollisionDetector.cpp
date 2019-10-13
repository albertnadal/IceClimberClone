#include <collision/algorithm/GJKCollisionDetector.h>
#include <collision/algorithm/MinkowskiSum.h>
#include <collision/math/Epsilon.h>
#include <collision/math/Vector2Util.h>
#include <iostream>

using namespace std;

namespace collision
{

GJKCollisionDetector::GJKCollisionDetector()
{
  penetrationSolver = unique_ptr<EPAMinkowskiPenetrationSolver>(new EPAMinkowskiPenetrationSolver);
}

bool GJKCollisionDetector::detect(Rectangle &rectangle_a, Rectangle &rectangle_b, Penetration& penetration)
{
  if((rectangle_a.vertices.size() < 4) || (rectangle_b.vertices.size() < 4)) {
    return false;
  }

  std::cout << "CHECKING COLLISION BETWEEN ";
  rectangle_a.Print();
  std::cout << " AND ";
  rectangle_b.Print();
  std::cout << "\n";

  uint16_t a_x1 = rectangle_a.x1;
  uint16_t a_y1 = rectangle_a.y1;
  uint16_t a_x2 = rectangle_a.x2;
  uint16_t a_y2 = rectangle_a.y2;

  uint16_t b_x1 = rectangle_b.x1;
  uint16_t b_y1 = rectangle_b.y1;
  uint16_t b_x2 = rectangle_b.x2;
  uint16_t b_y2 = rectangle_b.y2;

  return (a_x1 < b_x2 && a_x2 > b_x1 && a_y1 < b_y2 && a_y2 > b_y1);
}

bool GJKCollisionDetector::detect(const Polygon &convex1, const Polygon &convex2, Penetration& penetration)
{
    Transform2 transform1;
    Transform2 transform2;
    vector<collision::vec2<float>> simplex;
    MinkowskiSum minkowskiSum(convex1, transform1, convex2, transform2);
    collision::vec2<float> direction = calcInitialDirection(convex1, transform1, convex2, transform2);

    if(detect(minkowskiSum, simplex, direction)) {
      penetrationSolver->findPenetration(simplex, minkowskiSum, penetration);
      return true;
    }
    return false;
}

collision::vec2<float> GJKCollisionDetector::calcInitialDirection(
    const Polygon &convex1,
    const Transform2 &transform1,
    const Polygon &convex2,
    const Transform2 &transform2)
{
    collision::vec2<float> c1 = transform1.getTransformed(convex1.getCenter());
    collision::vec2<float> c2 = transform2.getTransformed(convex2.getCenter());

    return c2 - c1;
}

bool GJKCollisionDetector::detect(const MinkowskiSum &minkowskiSum, vector<collision::vec2<float>> &simplex, collision::vec2<float> &direction)
{
    // check for a zero direction vector
    if (Vector2Util::isZero(direction))
    {
        direction = collision::vec2<float>(1.0, 0.0);
    }

    // add the first point
    simplex.push_back(minkowskiSum.getSupportPoint(direction));

    // is the support point past the origin along d?
    if (Vector2Util::dot(simplex[0], direction) <= 0.0)
    {
        return false;
    }

    // negate the search direction
    direction *= -1.0;

    // start the loop
    while (true)
    {
        // always add another point to the simplex at the beginning of the loop
        simplex.push_back(minkowskiSum.getSupportPoint(direction));

        // make sure that the last point we added was past the origin
        collision::vec2<float> lastPoint = simplex.back();
        if (Vector2Util::dot(lastPoint, direction) <= 0.0)
        {
            // a is not past the origin so therefore the shapes do not intersect
            // here we treat the origin on the line as no intersection
            // immediately return with null indicating no penetration
            return false;
        }
        else
        {
            // if it is past the origin, then test whether the simplex contains the origin
            if (checkSimplex(simplex, direction))
            {
                // if the simplex contains the origin then we know that there is an intersection.
                // if we broke out of the loop then we know there was an intersection
                return true;
            }

            // if the simplex does not contain the origin then we need to loop using the new
            // search direction and simplex
        }
    }
}

bool GJKCollisionDetector::checkSimplex(vector<collision::vec2<float>> &simplex, collision::vec2<float> &direction)
{
    // get the last point added (a)
    collision::vec2<float> a = simplex.back();

    // this is the same as a.to(ORIGIN);
    collision::vec2<float> ao = -a;

    // check to see what type of simplex we have
    if (simplex.size() == 3)
    {
        // then we have a triangle
        collision::vec2<float> b = simplex[1];
        collision::vec2<float> c = simplex[0];

        // get the edges
        collision::vec2<float> ab = b - a;
        collision::vec2<float> ac = c - a;

        // get the edge normals
        collision::vec2<float> abPerp = Vector2Util::tripleProduct(ac, ab, ab);
        collision::vec2<float> acPerp = Vector2Util::tripleProduct(ab, ac, ac);

        // see where the origin is at
        float acLocation = Vector2Util::dot(acPerp, ao);

        if (acLocation >= 0.0)
        {
            // the origin lies on the right side of A->C
            // because of the condition for the gjk loop to continue the origin
            // must lie between A and C so remove B and set the
            // new search direction to A->C perpendicular vector
            simplex.erase(simplex.begin() + 1);

            // this used to be direction.set(Vector.tripleProduct(ac, ao, ac));
            // but was changed since the origin may lie on the segment created
            // by a -> c in which case would produce a zero vector normal
            // calculating ac's normal using b is more robust
            direction = acPerp;
        }
        else
        {
            float abLocation = Vector2Util::dot(abPerp, ao);

            // the origin lies on the left side of A->C
            if (abLocation < 0.0)
            {
                // the origin lies on the right side of A->B and therefore in the
                // triangle, we have an intersection
                return true;
            }
            else
            {
                // the origin lies between A and B so remove C and set the
                // search direction to A->B perpendicular vector
                simplex.erase(simplex.begin());

                // this used to be direction.set(Vector.tripleProduct(ab, ao, ab));
                // but was changed since the origin may lie on the segment created
                // by a -> b in which case would produce a zero vector normal
                // calculating ab's normal using c is more robust
                direction = abPerp;
            }
        }
    }
    else
    {
        // get the b point
        collision::vec2<float> b = simplex[0];
        collision::vec2<float> ab = b - a;

        // otherwise we have 2 points (line segment)
        // because of the condition for the gjk loop to continue the origin
        // must lie in between A and B, so keep both points in the simplex and
        // set the direction to the perp of the line segment towards the origin
        direction = Vector2Util::tripleProduct(ab, ao, ab);

        // check for degenerate cases where the origin lies on the segment
        // created by a -> b which will yield a zero edge normal
        if (Vector2Util::magnitudeSquared(direction) <= Epsilon::getEpsilon())
        {
            // in this case just choose either normal (left or right)
            direction = Vector2Util::left(ab);
        }
    }

    return false;
}

} // namespace collision
