#include <iostream>
#include "object_sprite_sheet_animation.h"

ObjectSpriteSheetAnimation::ObjectSpriteSheetAnimation(uint16 _id)
        : Id(_id)
{

}

ObjectSpriteSheetAnimation::~ObjectSpriteSheetAnimation() {
  sprites.clear();
}

void ObjectSpriteSheetAnimation::Print()
{
  printf("Sprite sheet animation Id: %d\n", Id);
  printf("Total animation frames: %lu\n", sprites.size());
  for (auto sprite : sprites)
  {
    printf("W: %d H: %d u1: %f v1: %f u2: %f v2: %f\n", sprite.width, sprite.height, sprite.u1, sprite.v1, sprite.u2, sprite.v2);
  }
}

void ObjectSpriteSheetAnimation::AddSprite(SpriteData sprite)
{
  sprites.push_back(sprite);
}

const std::vector<SpriteData>& ObjectSpriteSheetAnimation::GetSprites() const
{
  return sprites;
}
