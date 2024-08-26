#ifndef ENTITY_SPRITE_SHEET_ANIMATION_H
#define ENTITY_SPRITE_SHEET_ANIMATION_H

#include <vector>
#include <defines.h>
#include <sprite.h>

struct SpriteData { int width, height, xOffset, yOffset; float u1, v1, u2, v2; int duration; bool beginNewLoop; int lowerBoundX, lowerBoundY, upperBoundX, upperBoundY; bool hasAttack; int attackLowerBoundX, attackLowerBoundY, attackUpperBoundX, attackUpperBoundY; };

class EntitySpriteSheetAnimation
{
  std::vector<SpriteData> sprites;
public:
  uint16_t Id;
  EntitySpriteSheetAnimation(uint16_t);
  ~EntitySpriteSheetAnimation();
  void AddSprite(SpriteData);
  const std::vector<SpriteData>& GetSprites() const;
};
#endif
