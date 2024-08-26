#include <entity_sprite_sheet.h>

EntitySpriteSheet::EntitySpriteSheet(EntityIdentificator _id)
        : Id(_id)
{

}

EntitySpriteSheet::~EntitySpriteSheet() {
  std::map<uint16_t, EntitySpriteSheetAnimation*>::iterator it = animations.begin();

	while (it != animations.end())
	{
		uint16_t entityAnimationId = it->first;
		EntitySpriteSheetAnimation* entityAnimation = it->second;
		delete entityAnimation;
		it++;
	}
}

void EntitySpriteSheet::AddAnimation(EntitySpriteSheetAnimation *animation) {
  animations.insert(std::pair<uint16_t,EntitySpriteSheetAnimation*>(animation->Id, animation));
}

std::optional<EntitySpriteSheetAnimation*> EntitySpriteSheet::GetAnimationWithId(uint16_t AnimationId) {
    auto it = animations.find(AnimationId);
    if (it != animations.end()) {
        return it->second;
    }

    return std::nullopt;
}
