#include <iostream>
#include <entity_sprite_sheet_animation.h>

EntitySpriteSheetAnimation::EntitySpriteSheetAnimation(uint16_t _id)
        : Id(_id)
{

}

EntitySpriteSheetAnimation::~EntitySpriteSheetAnimation() {
  sprites.clear();
}

void EntitySpriteSheetAnimation::Print()
{
  printf("Sprite sheet animation Id: %d\n", Id);
  printf("Total animation frames: %lu\n", sprites.size());
  for (auto sprite : sprites)
  {
    printf("W: %d H: %d x: %d y: %d u1: %f v1: %f u2: %f v2: %f\n", sprite.width, sprite.height, sprite.xOffset, sprite.yOffset, sprite.u1, sprite.v1, sprite.u2, sprite.v2);
  }
}

void EntitySpriteSheetAnimation::AddSprite(SpriteData sprite)
{
  sprites.push_back(sprite);
}

const std::vector<SpriteData>& EntitySpriteSheetAnimation::GetSprites() const
{
  return sprites;
}
