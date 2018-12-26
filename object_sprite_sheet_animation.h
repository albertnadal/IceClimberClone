#ifndef OBJECT_SPRITE_SHEET_ANIMATION_H
#define OBJECT_SPRITE_SHEET_ANIMATION_H

#include <vector>
#include <types.h>

struct SpriteData { uint16 width, height; float u1, v1, u2, v2; };

class ObjectSpriteSheetAnimation
{
public:
  uint16 Id;
  std::vector<SpriteData> sprites;
  ObjectSpriteSheetAnimation(uint16);
  ~ObjectSpriteSheetAnimation();
  void AddSprite(SpriteData);
  void Print();
};
#endif
