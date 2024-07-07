#include "brick.h"
#include <chrono>

Brick::Brick() :
        ISceneObject(SceneObjectIdentificator::BRICK, SceneObjectType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

Brick::Brick(SceneObjectIdentificator _id, SceneObjectType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable) :
        ISceneObject(_id, _type, surface_type, max_states, is_breakable) {
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
        std::optional<ObjectSpriteSheetAnimation *> currentAnimation = spriteSheet->GetAnimationWithId(animationId);
        assert(currentAnimation != std::nullopt);
        currentAnimationSprites = (*currentAnimation)->GetSprites();
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
        Brick(SceneObjectIdentificator::BRICK_BLUE, SceneObjectType::TERRAIN, SurfaceType::SLIDING, BrickStateIdentificator::BRICK_MAX_STATES, true) {
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

/* BrickBlueHalf */

BrickBlueHalf::BrickBlueHalf() :
        Brick(SceneObjectIdentificator::BRICK_BLUE_HALF, SceneObjectType::TERRAIN, SurfaceType::SLIDING, BrickStateIdentificator::BRICK_MAX_STATES, true) {
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

/* BrickBrown */

BrickBrown::BrickBrown() :
        Brick(SceneObjectIdentificator::BRICK_BROWN, SceneObjectType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true) {
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

/* BrickBrownHalf */

BrickBrownHalf::BrickBrownHalf() :
        Brick(SceneObjectIdentificator::BRICK_BROWN_HALF, SceneObjectType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true) {
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

/* BrickGreenHalf */

BrickGreenHalf::BrickGreenHalf() :
        Brick(SceneObjectIdentificator::BRICK_GREEN_HALF, SceneObjectType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true) {
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

/* BrickBlueConveyorBeltRight */

BrickBlueConveyorBeltRight::BrickBlueConveyorBeltRight() :
        Brick(SceneObjectIdentificator::BRICK_BLUE_CONVEYOR_BELT_RIGHT, SceneObjectType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBlueConveyorBeltRight::PrintName() {
        std::cout << "BrickBlueConveyorBeltRight." << std::endl;
}

void BrickBlueConveyorBeltRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueConveyorBeltRightAnimation::BRICK_BLUE_CONVEYOR_BELT_RIGHT_STICKY);
}

ISceneObject* BrickBlueConveyorBeltRight::Create() {
        return new BrickBlueConveyorBeltRight();
}

void BrickBlueConveyorBeltRight::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueConveyorBeltRightAnimation::BRICK_BLUE_CONVEYOR_BELT_RIGHT_STICKY);
}

void BrickBlueConveyorBeltRight::STATE_Falling()
{
        LoadAnimationWithId(BrickBlueConveyorBeltRightAnimation::BRICK_BLUE_CONVEYOR_BELT_RIGHT_FALLING);
}

/* BrickBlueConveyorBeltLeft */

BrickBlueConveyorBeltLeft::BrickBlueConveyorBeltLeft() :
        Brick(SceneObjectIdentificator::BRICK_BLUE_CONVEYOR_BELT_LEFT, SceneObjectType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBlueConveyorBeltLeft::PrintName() {
        std::cout << "BrickBlueConveyorBeltLeft." << std::endl;
}

void BrickBlueConveyorBeltLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueConveyorBeltLeftAnimation::BRICK_BLUE_CONVEYOR_BELT_LEFT_STICKY);
}

ISceneObject* BrickBlueConveyorBeltLeft::Create() {
        return new BrickBlueConveyorBeltLeft();
}

void BrickBlueConveyorBeltLeft::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueConveyorBeltLeftAnimation::BRICK_BLUE_CONVEYOR_BELT_LEFT_STICKY);
}

void BrickBlueConveyorBeltLeft::STATE_Falling()
{
        LoadAnimationWithId(BrickBlueConveyorBeltLeftAnimation::BRICK_BLUE_CONVEYOR_BELT_LEFT_FALLING);
}

/* BrickBrownConveyorBeltRight */

BrickBrownConveyorBeltRight::BrickBrownConveyorBeltRight() :
        Brick(SceneObjectIdentificator::BRICK_BROWN_CONVEYOR_BELT_RIGHT, SceneObjectType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBrownConveyorBeltRight::PrintName() {
        std::cout << "BrickBrownConveyorBeltRight." << std::endl;
}

void BrickBrownConveyorBeltRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownConveyorBeltRightAnimation::BRICK_BROWN_CONVEYOR_BELT_RIGHT_STICKY);
}

ISceneObject* BrickBrownConveyorBeltRight::Create() {
        return new BrickBrownConveyorBeltRight();
}

void BrickBrownConveyorBeltRight::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownConveyorBeltRightAnimation::BRICK_BROWN_CONVEYOR_BELT_RIGHT_STICKY);
}

void BrickBrownConveyorBeltRight::STATE_Falling()
{
        LoadAnimationWithId(BrickBrownConveyorBeltRightAnimation::BRICK_BROWN_CONVEYOR_BELT_RIGHT_FALLING);
}

/* BrickBrownConveyorBeltLeft */

BrickBrownConveyorBeltLeft::BrickBrownConveyorBeltLeft() :
        Brick(SceneObjectIdentificator::BRICK_BROWN_CONVEYOR_BELT_LEFT, SceneObjectType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickBrownConveyorBeltLeft::PrintName() {
        std::cout << "BrickBrownConveyorBeltLeft." << std::endl;
}

void BrickBrownConveyorBeltLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownConveyorBeltLeftAnimation::BRICK_BROWN_CONVEYOR_BELT_LEFT_STICKY);
}

ISceneObject* BrickBrownConveyorBeltLeft::Create() {
        return new BrickBrownConveyorBeltLeft();
}

void BrickBrownConveyorBeltLeft::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownConveyorBeltLeftAnimation::BRICK_BROWN_CONVEYOR_BELT_LEFT_STICKY);
}

void BrickBrownConveyorBeltLeft::STATE_Falling()
{
        LoadAnimationWithId(BrickBrownConveyorBeltLeftAnimation::BRICK_BROWN_CONVEYOR_BELT_LEFT_FALLING);
}

/* BrickGreenConveyorBeltRight */

BrickGreenConveyorBeltRight::BrickGreenConveyorBeltRight() :
        Brick(SceneObjectIdentificator::BRICK_GREEN_CONVEYOR_BELT_RIGHT, SceneObjectType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickGreenConveyorBeltRight::PrintName() {
        std::cout << "BrickGreenConveyorBeltRight." << std::endl;
}

void BrickGreenConveyorBeltRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenConveyorBeltRightAnimation::BRICK_GREEN_CONVEYOR_BELT_RIGHT_STICKY);
}

ISceneObject* BrickGreenConveyorBeltRight::Create() {
        return new BrickGreenConveyorBeltRight();
}

void BrickGreenConveyorBeltRight::STATE_Sticky()
{
        LoadAnimationWithId(BrickGreenConveyorBeltRightAnimation::BRICK_GREEN_CONVEYOR_BELT_RIGHT_STICKY);
}

void BrickGreenConveyorBeltRight::STATE_Falling()
{
        LoadAnimationWithId(BrickGreenConveyorBeltRightAnimation::BRICK_GREEN_CONVEYOR_BELT_RIGHT_FALLING);
}

/* BrickGreenConveyorBeltLeft */

BrickGreenConveyorBeltLeft::BrickGreenConveyorBeltLeft() :
        Brick(SceneObjectIdentificator::BRICK_GREEN_CONVEYOR_BELT_LEFT, SceneObjectType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, true) {
}

void BrickGreenConveyorBeltLeft::PrintName() {
        std::cout << "BrickGreenConveyorBeltLeft." << std::endl;
}

void BrickGreenConveyorBeltLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenConveyorBeltLeftAnimation::BRICK_GREEN_CONVEYOR_BELT_LEFT_STICKY);
}

ISceneObject* BrickGreenConveyorBeltLeft::Create() {
        return new BrickGreenConveyorBeltLeft();
}

void BrickGreenConveyorBeltLeft::STATE_Sticky()
{
        LoadAnimationWithId(BrickGreenConveyorBeltLeftAnimation::BRICK_GREEN_CONVEYOR_BELT_LEFT_STICKY);
}

void BrickGreenConveyorBeltLeft::STATE_Falling()
{
        LoadAnimationWithId(BrickGreenConveyorBeltLeftAnimation::BRICK_GREEN_CONVEYOR_BELT_LEFT_FALLING);
}

/* BrickGreenUnbreakable */

BrickGreenUnbreakable::BrickGreenUnbreakable() :
        Brick(SceneObjectIdentificator::BRICK_GREEN_UNBREAKABLE, SceneObjectType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, false) {
}

void BrickGreenUnbreakable::PrintName() {
        std::cout << "BrickGreenUnbreakable." << std::endl;
}

void BrickGreenUnbreakable::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenUnbreakableAnimation::BRICK_GREEN_UNBREAKABLE_STICKY);
}

ISceneObject* BrickGreenUnbreakable::Create() {
        return new BrickGreenUnbreakable();
}

void BrickGreenUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickGreenUnbreakableAnimation::BRICK_GREEN_UNBREAKABLE_STICKY);
}

/* BrickBrownUnbreakable */

BrickBrownUnbreakable::BrickBrownUnbreakable() :
        Brick(SceneObjectIdentificator::BRICK_BROWN_UNBREAKABLE, SceneObjectType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, false) {
}

void BrickBrownUnbreakable::PrintName() {
        std::cout << "BrickBrownUnbreakable." << std::endl;
}

void BrickBrownUnbreakable::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownUnbreakableAnimation::BRICK_BROWN_UNBREAKABLE_STICKY);
}

ISceneObject* BrickBrownUnbreakable::Create() {
        return new BrickBrownUnbreakable();
}

void BrickBrownUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownUnbreakableAnimation::BRICK_BROWN_UNBREAKABLE_STICKY);
}

/* BrickBlueUnbreakable */

BrickBlueUnbreakable::BrickBlueUnbreakable() :
        Brick(SceneObjectIdentificator::BRICK_BLUE_UNBREAKABLE, SceneObjectType::TERRAIN, SurfaceType::SLIDING, BrickStateIdentificator::BRICK_MAX_STATES, false) {
}

void BrickBlueUnbreakable::PrintName() {
        std::cout << "BrickBlueUnbreakable." << std::endl;
}

void BrickBlueUnbreakable::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueUnbreakableAnimation::BRICK_BLUE_UNBREAKABLE_STICKY);
}

ISceneObject* BrickBlueUnbreakable::Create() {
        return new BrickBlueUnbreakable();
}

void BrickBlueUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueUnbreakableAnimation::BRICK_BLUE_UNBREAKABLE_STICKY);
}
