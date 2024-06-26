#include "brick.h"
#include <chrono>

Brick::Brick() :
        ISceneObject(SceneObjectIdentificator::BRICK, SceneObjectType::TERRAIN, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

Brick::Brick(SceneObjectIdentificator scene_id, SceneObjectType scene_type, unsigned char max_states, bool is_breakable) :
        ISceneObject(scene_id, scene_type, max_states, is_breakable) {
}

uint16_t Brick::Width() {
        return currentSprite.width;
}

uint16_t Brick::Height() {
        return currentSprite.height;
}

void Brick::PrintName() {
        std::cout << "Brick." << std::endl;
}

void Brick::Propel(float vSpeed, float hSpeed) {
    vInitialPropelSpeed = vSpeed;
    hInitialPropelSpeed = hSpeed;
    vInitialPropelPosition = position.GetRealY();
    hInitialPropelPosition = position.GetRealX();
    tPropel = 0.0f;
    isPropelled = true;
}

bool Brick::Update(uint8_t pressedKeys_) {
        if (isMarkedToDelete) {
                return false;
        }

        bool needRedraw = false;

        if(!animationLoaded) {
                return false;
        }

        if (isPropelled) {
                UpdatePropel();
                needRedraw = true;
        }
        else if(animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
                return false;
        }

        if(chrono::system_clock::now() >= nextSpriteTime) {
                // Load next sprite of the current animation
                LoadNextSprite();
                return true;
        }

        return needRedraw;
}

void Brick::UpdatePropel() {
    tPropel += 0.2f;

    // Equation of vertical and horizontal displacement of a parabolic jump.
    float vOffset = -(vInitialPropelSpeed * tPropel - (0.5f) * GRAVITY * tPropel * tPropel);

    // Update vertical propel position
    PositionSetXY(hInitialPropelPosition + (hInitialPropelSpeed * tPropel), vInitialPropelPosition + vOffset);

    if (tPropel >= 30.0f) {
        FinishPropel();
    }
}

void Brick::FinishPropel() {
    isPropelled = false;
    isMarkedToDelete = true;
}

void Brick::Hit(bool propelToRight) {
        if (!isBreakable) {
                return;
        }

        RemoveFromSpacePartitionObjectsTree();
        Break();
        Propel(24.0f, propelToRight ? 10.0f : -10.0f);
}

void Brick::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickAnimation::BRICK_GREEN_STICKY);
}

void Brick::LoadAnimationWithId(uint16_t animationId) {
        ObjectSpriteSheetAnimation *currentAnimation = spriteSheet->GetAnimationWithId(animationId);
        currentAnimationSprites = currentAnimation->GetSprites();
        animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        animationLoaded = true;
        firstSpriteOfCurrentAnimationIsLoaded = false;
        nextSpriteTime = std::chrono::system_clock::now();
}

void Brick::LoadNextSprite()
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

SpriteData Brick::NextSpriteData()
{
        if(currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
                currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
                (*currentAnimationSpriteIterator).beginNewLoop = true;
        }

        return *currentAnimationSpriteIterator++;
}

ISceneObject* Brick::Create() {
        return new Brick();
}

Brick::~Brick() {

}

bool Brick::BeginAnimationLoopAgain()
{
        return false;
}

void Brick::Break()
{
        BEGIN_TRANSITION_MAP                  // - Current State -
                TRANSITION_MAP_ENTRY (STATE_FALLING)  // STATE_Sticky
                TRANSITION_MAP_ENTRY (EVENT_IGNORED)  // STATE_Falling
        END_TRANSITION_MAP(NULL)
}

void Brick::STATE_Sticky()
{

}

void Brick::STATE_Falling()
{

}

/* BrickBlue */

BrickBlue::BrickBlue() :
        Brick(SceneObjectIdentificator::BRICK_BLUE, SceneObjectType::TERRAIN, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBlue::PrintName() {
        std::cout << "BrickBlue." << std::endl;
}

void BrickBlue::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueAnimation::BRICK_BLUE_STICKY);
}

ISceneObject* BrickBlue::Create() {
        return new BrickBlue();
}

void BrickBlue::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueAnimation::BRICK_BLUE_STICKY);
}

void BrickBlue::STATE_Falling()
{
        LoadAnimationWithId(BrickBlueAnimation::BRICK_BLUE_FALLING);
}

BrickBlue::~BrickBlue() {

}

/* BrickBlueHalf */

BrickBlueHalf::BrickBlueHalf() :
        Brick(SceneObjectIdentificator::BRICK_BLUE_HALF, SceneObjectType::TERRAIN, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBlueHalf::PrintName() {
        std::cout << "BrickBlueHalf." << std::endl;
}

void BrickBlueHalf::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueHalfAnimation::BRICK_BLUE_HALF_STICKY);
}

ISceneObject* BrickBlueHalf::Create() {
        return new BrickBlueHalf();
}

void BrickBlueHalf::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueHalfAnimation::BRICK_BLUE_HALF_STICKY);
}

void BrickBlueHalf::STATE_Falling()
{
        LoadAnimationWithId(BrickBlueHalfAnimation::BRICK_BLUE_HALF_FALLING);
}

BrickBlueHalf::~BrickBlueHalf() {

}

/* BrickBrown */

BrickBrown::BrickBrown() :
        Brick(SceneObjectIdentificator::BRICK_BROWN, SceneObjectType::TERRAIN, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBrown::PrintName() {
        std::cout << "BrickBrown." << std::endl;
}

void BrickBrown::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownAnimation::BRICK_BROWN_STICKY);
}

ISceneObject* BrickBrown::Create() {
        return new BrickBrown();
}

void BrickBrown::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownAnimation::BRICK_BROWN_STICKY);
}

void BrickBrown::STATE_Falling()
{
        LoadAnimationWithId(BrickBrownAnimation::BRICK_BROWN_FALLING);
}

BrickBrown::~BrickBrown() {

}

/* BrickBrownHalf */

BrickBrownHalf::BrickBrownHalf() :
        Brick(SceneObjectIdentificator::BRICK_BROWN_HALF, SceneObjectType::TERRAIN, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBrownHalf::PrintName() {
        std::cout << "BrickBrownHalf." << std::endl;
}

void BrickBrownHalf::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownHalfAnimation::BRICK_BROWN_HALF_STICKY);
}

ISceneObject* BrickBrownHalf::Create() {
        return new BrickBrownHalf();
}

void BrickBrownHalf::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownHalfAnimation::BRICK_BROWN_HALF_STICKY);
}

void BrickBrownHalf::STATE_Falling()
{
        LoadAnimationWithId(BrickBrownHalfAnimation::BRICK_BROWN_HALF_FALLING);
}

BrickBrownHalf::~BrickBrownHalf() {

}

/* BrickGreenHalf */

BrickGreenHalf::BrickGreenHalf() :
        Brick(SceneObjectIdentificator::BRICK_GREEN_HALF, SceneObjectType::TERRAIN, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickGreenHalf::PrintName() {
        std::cout << "BrickGreenHalf." << std::endl;
}

void BrickGreenHalf::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenHalfAnimation::BRICK_GREEN_HALF_STICKY);
}

ISceneObject* BrickGreenHalf::Create() {
        return new BrickGreenHalf();
}

void BrickGreenHalf::STATE_Sticky()
{
        LoadAnimationWithId(BrickGreenHalfAnimation::BRICK_GREEN_HALF_STICKY);
}

void BrickGreenHalf::STATE_Falling()
{
        LoadAnimationWithId(BrickGreenHalfAnimation::BRICK_GREEN_HALF_FALLING);
}

BrickGreenHalf::~BrickGreenHalf() {

}