#ifndef SPRITE_H
#define SPRITE_H

#include <defines.h>
#include <sprite_texture.h>
#include <collision/collision.h>

using namespace std;

struct Area { uint16_t id; collision::Polygon polygon; };
struct SpriteAreas { std::vector<Area> solidAreas; std::vector<Area> simpleAreas; };

class Sprite
{
  SpriteTexture texture;
public:
  uint16_t width;
  uint16_t height;
  float u1, v1, u2, v2;
  SpriteAreas *areas;
  Sprite();
  ~Sprite();
};

#endif
