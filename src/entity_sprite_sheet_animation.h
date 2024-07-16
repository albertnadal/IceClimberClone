#ifndef ENTITY_SPRITE_SHEET_ANIMATION_H
#define ENTITY_SPRITE_SHEET_ANIMATION_H

#include <vector>
#include <defines.h>
#include "sprite.h"

struct SpriteData { uint16_t width, height; int16_t xOffset, yOffset; float u1, v1, u2, v2; uint16_t duration; bool beginNewLoop; int lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; SpriteAreas *areas; };

class EntitySpriteSheetAnimation
{
  std::vector<SpriteData> sprites;
public:
  uint16_t Id;
  EntitySpriteSheetAnimation(uint16_t);
  ~EntitySpriteSheetAnimation();
  void AddSprite(SpriteData);
  const std::vector<SpriteData>& GetSprites() const;
  void Print();
};
#endif
