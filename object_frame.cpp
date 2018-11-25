#include "object_frame.h"

ObjectFrame::ObjectFrame(uint16 _width, uint16 _height, float _u1, float _v1, float _u2, float _v2)
        : width(_width), height(_height), u1(_u1), v1(_v1), u2(_u2), v2(_v2)
{
}

void ObjectFrame::Print()
{
  printf("W: %d H: %d u1: %f v1: %f u2: %f v2: %f\n", width, height, u1, v1, u2, v2);
}

ObjectFrame::~ObjectFrame() {
}
