#include <entities/water.h>
#include <chrono>

Water::Water() :
        IEntity(EntityIdentificator::WATER, EntityType::TERRAIN, SurfaceType::SIMPLE, WaterStateIdentificator::WATER_MAX_STATES, false, true) {
}

Water::Water(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void Water::PrintName() const {
        std::cout << "Water." << std::endl;
}

bool Water::Update(uint8_t pressedKeys_) {
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

void Water::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(WaterAnimation::WATER_STICKY);
}

IEntity* Water::Create() {
          return new Water();
}

Water::~Water() {

}
