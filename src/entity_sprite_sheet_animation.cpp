#include <iostream>
#include <entity_sprite_sheet_animation.h>

EntitySpriteSheetAnimation::EntitySpriteSheetAnimation(uint16_t _id)
        : Id(_id)
{

}

EntitySpriteSheetAnimation::~EntitySpriteSheetAnimation() {
  sprites.clear();
}

void EntitySpriteSheetAnimation::AddSprite(SpriteData sprite)
{
  sprites.push_back(sprite);
}

const std::vector<SpriteData>& EntitySpriteSheetAnimation::GetSprites() const
{
  return sprites;
}
