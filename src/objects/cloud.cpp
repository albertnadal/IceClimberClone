#include "cloud.h"
#include <chrono>

Cloud::Cloud() :
        ISceneObject(SceneObjectIdentificator::CLOUD_SMALL, SceneObjectType::TERRAIN, SurfaceType::SIMPLE, CloudStateIdentificator::CLOUD_MAX_STATES, false) {
}

Cloud::Cloud(SceneObjectIdentificator _id, SceneObjectType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable) :
        ISceneObject(_id, _type, surface_type, max_states, is_breakable) {
}

uint16_t Cloud::Width() {
        return currentSprite.width;
}

uint16_t Cloud::Height() {
        return currentSprite.height;
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
        std::vector<int> lowerBound = GetLowerBound();
        std::vector<int> upperBound = GetUpperBound();
        spacePartitionObjectsTree->updateParticle(this, lowerBound, upperBound);
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

void Cloud::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudSmallAnimation::CLOUD_SMALL_FLYING);
}

void Cloud::LoadAnimationWithId(uint16_t animationId) {
        std::optional<ObjectSpriteSheetAnimation *> currentAnimation = spriteSheet->GetAnimationWithId(animationId);
        assert(currentAnimation != std::nullopt);
        currentAnimationSprites = (*currentAnimation)->GetSprites();
        animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        animationLoaded = true;
        firstSpriteOfCurrentAnimationIsLoaded = false;
        nextSpriteTime = std::chrono::system_clock::now();
}

void Cloud::LoadNextSprite()
{
  SpriteData spriteData = NextSpriteData();
  if(spriteData.beginNewLoop) {
          if(BeginAnimationLoopAgain()) {
            spriteData = NextSpriteData();
          }
  }

  nextSpriteTime = (chrono::system_clock::now() + std::chrono::milliseconds(spriteData.duration));

  currentSprite.width = spriteData.width;
  currentSprite.height = spriteData.height;
  currentSprite.xOffset = spriteData.xOffset;
  currentSprite.yOffset = spriteData.yOffset;
  currentSprite.u1 = spriteData.u1;
  currentSprite.v1 = spriteData.v1;
  currentSprite.u2 = spriteData.u2;
  currentSprite.v2 = spriteData.v2;
  currentSprite.areas = spriteData.areas;
  recalculateAreasDataIsNeeded = true; // Is necessary because the current sprite may have different areas
  boundingBox = { spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY };
  firstSpriteOfCurrentAnimationIsLoaded = true;
}

SpriteData Cloud::NextSpriteData()
{
        if(currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
                currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
                (*currentAnimationSpriteIterator).beginNewLoop = true;
        }

        return *currentAnimationSpriteIterator++;
}

ISceneObject* Cloud::Create() {
        return new Cloud();
}

Cloud::~Cloud() {

}

bool Cloud::BeginAnimationLoopAgain()
{
        return false;
}

/* CloudSmall */

CloudSmall::CloudSmall() :
        Cloud(SceneObjectIdentificator::CLOUD_SMALL, SceneObjectType::TERRAIN, SurfaceType::MOBILE_RIGHT, CloudStateIdentificator::CLOUD_MAX_STATES, false) {
}

void CloudSmall::PrintName() {
        std::cout << "CloudSmall." << std::endl;
}

void CloudSmall::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudSmallAnimation::CLOUD_SMALL_FLYING);
}

ISceneObject* CloudSmall::Create() {
        return new CloudSmall();
}

/* CloudBig */

CloudBig::CloudBig() :
        Cloud(SceneObjectIdentificator::CLOUD_BIG, SceneObjectType::TERRAIN, SurfaceType::MOBILE_RIGHT, CloudStateIdentificator::CLOUD_MAX_STATES, false) {
}

void CloudBig::PrintName() {
        std::cout << "CloudBig." << std::endl;
}

void CloudBig::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudBigAnimation::CLOUD_BIG_FLYING);
}

ISceneObject* CloudBig::Create() {
        return new CloudBig();
}

/* CloudTiny */

CloudTiny::CloudTiny() :
        Cloud(SceneObjectIdentificator::CLOUD_TINY, SceneObjectType::TERRAIN, SurfaceType::MOBILE_RIGHT, CloudStateIdentificator::CLOUD_MAX_STATES, false) {
}

void CloudTiny::PrintName() {
        std::cout << "CloudTiny." << std::endl;
}

void CloudTiny::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(CloudTinyAnimation::CLOUD_TINY_FLYING);
}

ISceneObject* CloudTiny::Create() {
        return new CloudTiny();
}