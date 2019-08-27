#ifndef OBJECT_SPRITE_SHEET_H
#define OBJECT_SPRITE_SHEET_H

#include <map>
#include <defines.h>
#include <object_sprite_sheet_animation.h>

class SceneObject;

class ObjectSpriteSheet
{
        SceneObjectIdentificator Id;
        std::map<uint16_t, ObjectSpriteSheetAnimation*> animations;
public:
        ObjectSpriteSheet(SceneObjectIdentificator);
        ~ObjectSpriteSheet();
        void AddAnimation(ObjectSpriteSheetAnimation*);
        ObjectSpriteSheetAnimation* GetAnimationWithId(uint16_t);
        void Print();
};

#endif
