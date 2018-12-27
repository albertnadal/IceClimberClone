#include <iostream>
#include "object_sprite_sheet_animation.h"

ObjectSpriteSheetAnimation::ObjectSpriteSheetAnimation(uint16 _id)
        : Id(_id)
{
  spriteIterator = std::begin(sprites);
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
  spriteIterator = sprites.insert(spriteIterator, sprite);
}

void ObjectSpriteSheetAnimation::MoveToFirstSprite()
{
  spriteIterator = std::begin(sprites);
}

SpriteData ObjectSpriteSheetAnimation::GetNextSpriteData()
{
  if(spriteIterator == std::end(sprites)) {
    spriteIterator = std::begin(sprites);
  }
  return *spriteIterator++;
}
