#ifndef SPRITE_H
#define SPRITE_H

#include <types.h>
#include <sprite_texture.h>

using namespace std;

class Sprite
{
  SpriteTexture texture;
public:
  uint16 width;
  uint16 height;
  float u1, v1, u2, v2;
  Sprite();
  ~Sprite();
  void loadTexture();
};

#endif
