#include <entities/cloud.h>
#include <chrono>

Cloud::Cloud() :
        IEntity(EntityIdentificator::CLOUD_SMALL, EntityType::TERRAIN, SurfaceType::SIMPLE, CloudStateIdentificator::CLOUD_MAX_STATES, false, false) {
}

Cloud::Cloud(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void Cloud::PrintName() {
        std::cout << "Cloud." << std::endl;
}

bool Cloud::IsCloud() {
  return true;
}

void Cloud::UpdateFlight() {
        PositionAddX(flyToRight ? 1.0f : -1.0f);

        // Respawn the cloud when it disappears from the margins
        if (flyToRight && (position.GetIntX() >= LEVEL_WIDTH)) {
                PositionSetX(0.0f - static_cast<float>(Width()));
        }
        else if (!flyToRight && (position.GetIntX() <= 0.0f - static_cast<float>(Width()))) {
                PositionSetX(static_cast<float>(LEVEL_WIDTH));
        }

        // Update the position of the cloud in the space partition tree
        UpdatePositionInSpacePartitionTree();
}

bool Cloud::Update(uint8_t pressedKeys_) {
        if (isMarkedToDelete) {
                return false;
        }

        bool needRedraw = false;

        UpdateFlight();

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

void Cloud::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudSmallAnimation::CLOUD_SMALL_FLYING);
}

IEntity* Cloud::Create() {
        return new Cloud();
}

Cloud::~Cloud() {

}

/* CloudSmall */

CloudSmall::CloudSmall() :
        Cloud(EntityIdentificator::CLOUD_SMALL, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, CloudStateIdentificator::CLOUD_MAX_STATES, false, false) {
}

void CloudSmall::PrintName() {
        std::cout << "CloudSmall." << std::endl;
}

void CloudSmall::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudSmallAnimation::CLOUD_SMALL_FLYING);
}

IEntity* CloudSmall::Create() {
        return new CloudSmall();
}

/* CloudBig */

CloudBig::CloudBig() :
        Cloud(EntityIdentificator::CLOUD_BIG, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, CloudStateIdentificator::CLOUD_MAX_STATES, false, false) {
}

void CloudBig::PrintName() {
        std::cout << "CloudBig." << std::endl;
}

void CloudBig::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudBigAnimation::CLOUD_BIG_FLYING);
}

IEntity* CloudBig::Create() {
        return new CloudBig();
}

/* CloudTiny */

CloudTiny::CloudTiny() :
        Cloud(EntityIdentificator::CLOUD_TINY, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, CloudStateIdentificator::CLOUD_MAX_STATES, false, false) {
}

void CloudTiny::PrintName() {
        std::cout << "CloudTiny." << std::endl;
}

void CloudTiny::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudTinyAnimation::CLOUD_TINY_FLYING);
}

IEntity* CloudTiny::Create() {
        return new CloudTiny();
}