#ifndef OBJECT_SPRITE_SHEET_ANIMATION_H
#define OBJECT_SPRITE_SHEET_ANIMATION_H

#include <vector>
#include <defines.h>
#include "sprite.h"

struct SpriteData { uint16_t width, height; int16_t xOffset, yOffset; float u1, v1, u2, v2; uint16_t duration; bool beginNewLoop; int lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; SpriteAreas *areas; };

class ObjectSpriteSheetAnimation
{
  std::vector<SpriteData> sprites;
public:
  uint16_t Id;
  ObjectSpriteSheetAnimation(uint16_t);
  ~ObjectSpriteSheetAnimation();
  void AddSprite(SpriteData);
  const std::vector<SpriteData>& GetSprites() const;
  void Print();
};
#endif
