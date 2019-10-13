#ifndef SPRITE_H
#define SPRITE_H

#include <defines.h>
#include <sprite_texture.h>
#include <collision/collision.h>

using namespace std;

struct Area { uint16_t id; collision::Rectangle rectangle; };
struct SpriteAreas { std::vector<Area> solidAreas; std::vector<Area> simpleAreas; };

class Sprite
{
  SpriteTexture texture;
public:
  uint16_t width;
  uint16_t height;
  int16_t xOffset;
  int16_t yOffset;
  float u1, v1, u2, v2;
  SpriteAreas *areas;
  Sprite();
  ~Sprite();
};

#endif
