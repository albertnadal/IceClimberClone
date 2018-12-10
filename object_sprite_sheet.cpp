#include "object_sprite_sheet.h"

ObjectSpriteSheet::ObjectSpriteSheet(SceneObjectIdentificator _id)
        : Id(_id)
{

}

ObjectSpriteSheet::~ObjectSpriteSheet() {
  for (auto *objectAnimation : animations)
  {
    delete objectAnimation;
  }

  animations.clear();
}

void ObjectSpriteSheet::Print()
{
  printf("SceneObject type Id: %d\n", Id);
  printf("Total object actions: %lu\n", animations.size());
  for (auto *objectAnimation : animations)
  {
    objectAnimation->Print();
  }
}

void ObjectSpriteSheet::AddAnimation(ObjectSpriteSheetAnimation *animation) {
  animations.push_back(animation);
}
