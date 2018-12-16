#ifndef OBJECT_SPRITE_SHEET_H
#define OBJECT_SPRITE_SHEET_H

#include <vector>
#include <types.h>
#include <object_sprite_sheet_animation.h>

class SceneObject;

class ObjectSpriteSheet
{
        SceneObjectIdentificator Id;
        std::vector<ObjectSpriteSheetAnimation*> animations;
public:
        ObjectSpriteSheet(SceneObjectIdentificator);
        ~ObjectSpriteSheet();
        void AddAnimation(ObjectSpriteSheetAnimation*);
        void Print();
};

#endif
