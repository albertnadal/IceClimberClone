#include <entities/bonus_stage_text.h>
#include <chrono>

BonusStageText::BonusStageText() :
        IEntity(EntityIdentificator::BONUS_STAGE_TEXT, EntityType::TERRAIN, SurfaceType::SIMPLE, BonusStageTextStateIdentificator::BONUS_STAGE_TEXT_MAX_STATES, false, true) {
}

BonusStageText::BonusStageText(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void BonusStageText::PrintName() {
        std::cout << "BonusStageText." << std::endl;
}

bool BonusStageText::Update(uint8_t pressedKeys_) {
        bool needRedraw = false;

        if(!animationLoaded) {
                return false;
        }

        if(animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
                return false;
        }

        if(chrono::system_clock::now() >= nextSpriteTime) {
                // Load next sprite of the current animation
                LoadNextSprite();
                return true;
        }

        return needRedraw;
}

void BonusStageText::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BonusStageTextAnimation::BONUS_STAGE_TEXT_STICKY);
}

IEntity* BonusStageText::Create() {
          return new BonusStageText();
}

BonusStageText::~BonusStageText() {

}
