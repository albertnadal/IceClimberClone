#ifndef SPRITE_H
#define SPRITE_H

#include <defines.h>
#include <sprite_texture.h>
#include <collision/collision.h>

using namespace std;

struct Area { uint16 id; collision::Polygon polygon; };
struct SpriteAreas { std::vector<Area> solidAreas; std::vector<Area> simpleAreas; };

class Sprite
{
  SpriteTexture texture;
public:
  uint16 width;
  uint16 height;
  float u1, v1, u2, v2;
  SpriteAreas *areas;
  Sprite();
  ~Sprite();
};

#endif
