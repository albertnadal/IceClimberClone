#include <collision/algorithm/GJKCollisionDetector.h>
#include <collision/math/Epsilon.h>
#include <collision/math/Vector2Util.h>
#include <iostream>

using namespace std;

namespace collision
{

GJKCollisionDetector::GJKCollisionDetector() {

}

bool GJKCollisionDetector::detect(Rectangle &rectangle_a, Rectangle &rectangle_b, Penetration &penetration, vec2<int16_t> &vectorDirection) {

  if((rectangle_a.vertices.size() < 4) || (rectangle_b.vertices.size() < 4)) {
    return false;
  }

//  std::cout << "CHECKING COLLISION BETWEEN ";
//  rectangle_a.Print();
//  std::cout << " AND ";
//  rectangle_b.Print();
//  std::cout << "\n";

  penetration.depth.x = 0;
  penetration.depth.y = 0;

  uint16_t a_x1 = rectangle_a.x1;
  uint16_t a_y1 = rectangle_a.y1;
  uint16_t a_x2 = rectangle_a.x2;
  uint16_t a_y2 = rectangle_a.y2;

  uint16_t b_x1 = rectangle_b.x1;
  uint16_t b_y1 = rectangle_b.y1;
  uint16_t b_x2 = rectangle_b.x2;
  uint16_t b_y2 = rectangle_b.y2;
/*
  std::cout << "aX1: " << a_x1 << " bX2: " << b_x2 << " aX2: " << a_x2 << " bX1: " << b_x1 << " aY1: " << a_y1 << " bY2: " << b_y2 << " aY2: " << a_y2 << " bY1: " << b_y1 << "\n";
  std::cout << "aX1 < bX2: " << (a_x1 < b_x2) << "\n";
  std::cout << "aX2 > bX1: " << (a_x2 > b_x1) << "\n";
  std::cout << "aY1 > bY2: " << (a_y1 > b_y2) << "\n";
  std::cout << "aY2 < bY1: " << (a_y2 < b_y1) << "\n";
*/
  bool collision = ((a_x1 < b_x2) && (a_x2 > b_x1) && (a_y1 > b_y2) && (a_y2 < b_y1));
  if(collision) {
//    std::cout << " ------------- COLISIO DETECTADA!\n";
//    std::cout << " ------------- VECTOR DIRECTION X: (" << vectorDirection.x << ")\n";
//    std::cout << " ------------- VECTOR DIRECTION Y: (" << vectorDirection.y << ")\n";

    if((vectorDirection.x == 1) && (b_x1 < a_x2)) {
      penetration.depth.x = a_x2 - b_x1;
    } else if((vectorDirection.x == -1) && (a_x1 < b_x2)) {
      penetration.depth.x = -(b_x2 - a_x1);
    }

    if((vectorDirection.y == 1) && (b_y2 < a_y1)) {
      penetration.depth.y = a_y1 - b_y2;
    } else if((vectorDirection.y == -1) && (a_y2 < b_y1)) {
      penetration.depth.y = -(b_y1 - a_y2);
    }

//    std::cout << " DEPTH X: " << penetration.depth.x << " Y: " << penetration.depth.y << "\n";
//    std::cout << " >>>>> COLLISIO <<<<<\n";
    return true;
  }

//  std::cout << " >>>>> NO COLLISIO <<<<<\n";
  return false;
}

} // namespace collision
