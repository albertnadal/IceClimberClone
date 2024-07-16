#ifndef ENTITY_SPRITE_SHEET_H
#define ENTITY_SPRITE_SHEET_H

#include <map>
#include <optional>
#include <defines.h>
#include <entity_sprite_sheet_animation.h>


class EntitySpriteSheet
{
        EntityIdentificator Id;
        std::map<uint16_t, EntitySpriteSheetAnimation*> animations;
public:
        EntitySpriteSheet(EntityIdentificator);
        ~EntitySpriteSheet();
        void AddAnimation(EntitySpriteSheetAnimation*);
        std::optional<EntitySpriteSheetAnimation*> GetAnimationWithId(uint16_t);
        void Print();
};

#endif
