#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <defines.h>
#include <collision/geometry/Rectangle.h>

using namespace std;

struct Area { uint16_t id; collision::Rectangle rectangle; };

class Sprite
{

public:
  int width;
  int height;
  int xOffset;
  int yOffset;
  float u1, v1, u2, v2;
  Sprite();
  ~Sprite();
};

#endif
