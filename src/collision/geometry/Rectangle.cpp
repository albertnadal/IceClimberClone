#include <collision/geometry/Rectangle.h>
#include <cmath>
#include <iostream>

using namespace std;

namespace collision {

    Rectangle::Rectangle(vector<collision::vec2<float>> points) {
        vertices.insert(vertices.end(), points.begin(), points.end());
        SearchTopLeftAndBottomRightVertices();
    }

    void Rectangle::SearchTopLeftAndBottomRightVertices() {
        if (vertices.empty()) {
            return;
        }

        x1 = 99999;
        x2 = 0;
        y1 = 0;
        y2 = 99999;
        size_t size = vertices.size();
        for (size_t i = 0; i < size; i++) {
            collision::vec2<float> v = vertices[i];
            if (v.x < x1) { x1 = v.x; }
            if (v.x > x2) { x2 = v.x; }
            if (v.y > y1) { y1 = v.y; }
            if (v.y < y2) { y2 = v.y; }
        }
    }

} // namespace collision
