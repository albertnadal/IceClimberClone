#include <collision/algorithm/CollisionDetector.h>
#include <collision/math/Vector2Util.h>
#include <iostream>

namespace collision {

    CollisionDetector::CollisionDetector() {

    }

    bool CollisionDetector::checkCollision(Rectangle &rectangle_a, Rectangle &rectangle_b, Penetration &penetration,
                                           vec2<int16_t> &vectorDirection) {

        if ((rectangle_a.vertices.size() < 4) || (rectangle_b.vertices.size() < 4)) {
            return false;
        }

        //std::cout << "CHECKING COLLISION BETWEEN ";
        //rectangle_a.Print();
        //std::cout << " AND ";
        //rectangle_b.Print();
        //std::cout << "\n";

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
        if (collision) {
//    std::cout << " ------------- COLISIO DETECTADA!\n";
//    std::cout << " ------------- VECTOR DIRECTION X: (" << vectorDirection.x << ")\n";
//    std::cout << " ------------- VECTOR DIRECTION Y: (" << vectorDirection.y << ")\n";

            if ((vectorDirection.x == 1) && (b_x1 < a_x2)) {
                penetration.depth.x = a_x2 - b_x1;
            } else if ((vectorDirection.x == -1) && (a_x1 < b_x2)) {
                penetration.depth.x = -(b_x2 - a_x1);
            }

            if ((vectorDirection.y == 1) && (b_y2 < a_y1)) {
                penetration.depth.y = a_y1 - b_y2;
            } else if ((vectorDirection.y == -1) && (a_y2 < b_y1)) {
                penetration.depth.y = -(b_y1 - a_y2);
            }

            std::cout << " DIRECTION X: " << vectorDirection.x << " Y: " << vectorDirection.y << "\n";
            std::cout << " DEPTH X: " << penetration.depth.x << " Y: " << penetration.depth.y << "\n";
//    std::cout << " >>>>> COLLISIO <<<<<\n";
            return true;
        }

//  std::cout << " >>>>> NO COLLISIO <<<<<\n";
        return false;
    }

    void CollisionDetector::updateWithNonCollidingPosition(std::vector<collision::Rectangle> &targetRectangles,
                                                           std::vector<collision::Rectangle> &movingRectangles,
                                                           Position &position,
                                                           vec2<int16_t> &vectorDirection) {
        uint16_t targetRectanglesCount = targetRectangles.size();
        uint16_t movingRectanglesCount = movingRectangles.size();
        float tangent = position.getTrajectoryTangent();
        float initial_x = position.GetPrevX();
        float initial_y = position.GetPrevY();
        float y_intercept = initial_y - (tangent * initial_x);
        float start_x = initial_x + (vectorDirection.x >= 0 ? -5.0f : 5.0f); //initial_x;
        float start_y = (tangent * start_x) + y_intercept; //initial_y;
        float final_x = position.GetRealX();
        float final_y = position.GetRealY();
        float end_x = final_x;
        float end_y = final_y;
        float current_x, current_y, offset_x, offset_y;
        float prev_start_x = start_x, prev_start_y = start_y;
        bool current_position_collides;
        collision::Penetration penetration;

        bool position_of_no_collision_found = false;
        while(!position_of_no_collision_found) {
            current_position_collides = false;
            current_x = start_x + ((end_x - start_x) / 2.0f);
            current_y = (tangent * current_x) + y_intercept;
            std::cout << "START => X: " << start_x << " Y: " << start_y << "\n";
            std::cout << "END => X: " << end_x << " Y: " << end_y << "\n";
            std::cout << "HALF => X: " << current_x << " Y: " << current_y << "\n";

            offset_x = current_x - final_x;
            offset_y = current_y - final_y;

            std::cout << "OFFSET => X: " << offset_x << " Y: " << offset_y << "\n";

            for (uint16_t i = 0; (i < movingRectanglesCount) && !current_position_collides; i++) {
                Rectangle &movingRectangle = movingRectangles[i];
                std::cout << "RECTANGLE NO   OFFSET: ";
                movingRectangle.Print();
                Rectangle *tempRectangle = movingRectangle.cloneWithOffset(offset_x, offset_y);
                std::cout << "\nRECTANGLE WITH OFFSET: ";
                tempRectangle->Print();

                for (uint16_t e = 0; (e < targetRectanglesCount) && !current_position_collides; e++) {
                    Rectangle &targetRectangle = targetRectangles[e];
                    if(checkCollision(*tempRectangle, targetRectangle, penetration, vectorDirection)) {
                        current_position_collides = true;
                    }
                }

                tempRectangle->Print();
                delete tempRectangle;
            }

            if(current_position_collides) {
                std::cout << " >>> current position collides <<<\n";
                start_x = prev_start_x;
                start_y = prev_start_y;
                end_x = current_x;
                end_y = current_y;
            } else {
                std::cout << " >>> current position NOT collides <<<\n";
                prev_start_x = start_x;
                prev_start_y = start_y;
                start_x = current_x;
                start_y = current_y;
            }

            if(abs(start_x - end_x) <= 0.0001f) {
                std::cout << " >>> DONE <<<\n";
                position_of_no_collision_found = true;
            }
        }

        std::cout << " >>> RESULT CURRENT_X: " << current_x << " CURRENT_Y: " << current_y << " \n";
        position.setXY(current_x, current_y);
    }
} // namespace collision
