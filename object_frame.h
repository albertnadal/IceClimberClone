#pragma once
#include <vector>
#include <types.h>

class ObjectFrame
{
        uint16 width, height;
        float u1, v1, u2, v2;
public:
        ObjectFrame(uint16 _width, uint16 _height, float _u1, float _v1, float _u2, float _v2);
};
