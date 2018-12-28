#ifndef OBJECT_SPRITE_SHEET_ANIMATION_H
#define OBJECT_SPRITE_SHEET_ANIMATION_H

#include <vector>
#include <types.h>

struct SpriteData { uint16 width, height; float u1, v1, u2, v2; uint16 duration; };

class ObjectSpriteSheetAnimation
{
  std::vector<SpriteData>::iterator spriteIterator;
  std::vector<SpriteData> sprites;
public:
  uint16 Id;
  bool hasOnlyOneSprite = false;
  ObjectSpriteSheetAnimation(uint16);
  ~ObjectSpriteSheetAnimation();
  void AddSprite(SpriteData);
  SpriteData NextSpriteData();
  void MoveToFirstSprite();
  void Print();
};
#endif
