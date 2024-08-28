#include <entities/brick.h>
#include <chrono>

Brick::Brick() :
        IEntity(EntityIdentificator::BRICK, EntityType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

Brick::Brick(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void Brick::PrintName() const {
        std::cout << "Brick." << std::endl;
}

void Brick::Propel(float vSpeed, float hSpeed) {
    vInitialPropelSpeed = vSpeed;
    hInitialPropelSpeed = hSpeed;
    vInitialPropelPosition = position.GetRealY();
    hInitialPropelPosition = position.GetRealX();
    tPropel = 0.0f;
    isPropelled = true;
    gameManager->PlaySoundById(SoundIdentificator::BREAK_BRICK_SOUND);
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

void Brick::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickAnimation::BRICK_GREEN_STICKY);
}

IEntity* Brick::Create() {
        return new Brick();
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
        LoadAnimationWithId(BrickAnimation::BRICK_GREEN_STICKY);
}

void Brick::STATE_Falling()
{
        LoadAnimationWithId(BrickAnimation::BRICK_GREEN_FALLING);
}

/* BrickBlue */

BrickBlue::BrickBlue() :
        Brick(EntityIdentificator::BRICK_BLUE, EntityType::TERRAIN, SurfaceType::SLIDING, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBlue::PrintName() const {
        std::cout << "BrickBlue." << std::endl;
}

void BrickBlue::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueAnimation::BRICK_BLUE_STICKY);
}

IEntity* BrickBlue::Create() {
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
        Brick(EntityIdentificator::BRICK_BLUE_HALF, EntityType::TERRAIN, SurfaceType::SLIDING, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBlueHalf::PrintName() const {
        std::cout << "BrickBlueHalf." << std::endl;
}

void BrickBlueHalf::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueHalfAnimation::BRICK_BLUE_HALF_STICKY);
}

IEntity* BrickBlueHalf::Create() {
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
        Brick(EntityIdentificator::BRICK_BROWN, EntityType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBrown::PrintName() const {
        std::cout << "BrickBrown." << std::endl;
}

void BrickBrown::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownAnimation::BRICK_BROWN_STICKY);
}

IEntity* BrickBrown::Create() {
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
        Brick(EntityIdentificator::BRICK_BROWN_HALF, EntityType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBrownHalf::PrintName() const {
        std::cout << "BrickBrownHalf." << std::endl;
}

void BrickBrownHalf::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownHalfAnimation::BRICK_BROWN_HALF_STICKY);
}

IEntity* BrickBrownHalf::Create() {
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
        Brick(EntityIdentificator::BRICK_GREEN_HALF, EntityType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickGreenHalf::PrintName() const {
        std::cout << "BrickGreenHalf." << std::endl;
}

void BrickGreenHalf::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenHalfAnimation::BRICK_GREEN_HALF_STICKY);
}

IEntity* BrickGreenHalf::Create() {
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
        Brick(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_RIGHT, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBlueConveyorBeltRight::PrintName() const {
        std::cout << "BrickBlueConveyorBeltRight." << std::endl;
}

void BrickBlueConveyorBeltRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueConveyorBeltRightAnimation::BRICK_BLUE_CONVEYOR_BELT_RIGHT_STICKY);
}

IEntity* BrickBlueConveyorBeltRight::Create() {
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
        Brick(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_LEFT, EntityType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBlueConveyorBeltLeft::PrintName() const {
        std::cout << "BrickBlueConveyorBeltLeft." << std::endl;
}

void BrickBlueConveyorBeltLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueConveyorBeltLeftAnimation::BRICK_BLUE_CONVEYOR_BELT_LEFT_STICKY);
}

IEntity* BrickBlueConveyorBeltLeft::Create() {
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
        Brick(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_RIGHT, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBrownConveyorBeltRight::PrintName() const {
        std::cout << "BrickBrownConveyorBeltRight." << std::endl;
}

void BrickBrownConveyorBeltRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownConveyorBeltRightAnimation::BRICK_BROWN_CONVEYOR_BELT_RIGHT_STICKY);
}

IEntity* BrickBrownConveyorBeltRight::Create() {
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
        Brick(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_LEFT, EntityType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickBrownConveyorBeltLeft::PrintName() const {
        std::cout << "BrickBrownConveyorBeltLeft." << std::endl;
}

void BrickBrownConveyorBeltLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownConveyorBeltLeftAnimation::BRICK_BROWN_CONVEYOR_BELT_LEFT_STICKY);
}

IEntity* BrickBrownConveyorBeltLeft::Create() {
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
        Brick(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_RIGHT, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickGreenConveyorBeltRight::PrintName() const {
        std::cout << "BrickGreenConveyorBeltRight." << std::endl;
}

void BrickGreenConveyorBeltRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenConveyorBeltRightAnimation::BRICK_GREEN_CONVEYOR_BELT_RIGHT_STICKY);
}

IEntity* BrickGreenConveyorBeltRight::Create() {
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
        Brick(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_LEFT, EntityType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, true, false) {
}

void BrickGreenConveyorBeltLeft::PrintName() const {
        std::cout << "BrickGreenConveyorBeltLeft." << std::endl;
}

void BrickGreenConveyorBeltLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenConveyorBeltLeftAnimation::BRICK_GREEN_CONVEYOR_BELT_LEFT_STICKY);
}

IEntity* BrickGreenConveyorBeltLeft::Create() {
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
        Brick(EntityIdentificator::BRICK_GREEN_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickGreenUnbreakable::PrintName() const {
        std::cout << "BrickGreenUnbreakable." << std::endl;
}

void BrickGreenUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenUnbreakableAnimation::BRICK_GREEN_UNBREAKABLE_STICKY);
}

IEntity* BrickGreenUnbreakable::Create() {
        return new BrickGreenUnbreakable();
}

void BrickGreenUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickGreenUnbreakableAnimation::BRICK_GREEN_UNBREAKABLE_STICKY);
}

/* BrickBrownUnbreakable */

BrickBrownUnbreakable::BrickBrownUnbreakable() :
        Brick(EntityIdentificator::BRICK_BROWN_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::SIMPLE, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickBrownUnbreakable::PrintName() const {
        std::cout << "BrickBrownUnbreakable." << std::endl;
}

void BrickBrownUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownUnbreakableAnimation::BRICK_BROWN_UNBREAKABLE_STICKY);
}

IEntity* BrickBrownUnbreakable::Create() {
        return new BrickBrownUnbreakable();
}

void BrickBrownUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownUnbreakableAnimation::BRICK_BROWN_UNBREAKABLE_STICKY);
}

/* BrickBlueUnbreakable */

BrickBlueUnbreakable::BrickBlueUnbreakable() :
        Brick(EntityIdentificator::BRICK_BLUE_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::SLIDING, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickBlueUnbreakable::PrintName() const {
        std::cout << "BrickBlueUnbreakable." << std::endl;
}

void BrickBlueUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueUnbreakableAnimation::BRICK_BLUE_UNBREAKABLE_STICKY);
}

IEntity* BrickBlueUnbreakable::Create() {
        return new BrickBlueUnbreakable();
}

void BrickBlueUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueUnbreakableAnimation::BRICK_BLUE_UNBREAKABLE_STICKY);
}

/* BrickBlueConveyorBeltRightUnbreakable */

BrickBlueConveyorBeltRightUnbreakable::BrickBlueConveyorBeltRightUnbreakable() :
        Brick(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_RIGHT_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickBlueConveyorBeltRightUnbreakable::PrintName() const {
        std::cout << "BrickBlueConveyorBeltRightUnbreakable." << std::endl;
}

void BrickBlueConveyorBeltRightUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueConveyorBeltRightUnbreakableAnimation::BRICK_BLUE_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY);
}

IEntity* BrickBlueConveyorBeltRightUnbreakable::Create() {
        return new BrickBlueConveyorBeltRightUnbreakable();
}

void BrickBlueConveyorBeltRightUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueConveyorBeltRightUnbreakableAnimation::BRICK_BLUE_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY);
}

/* BrickBlueConveyorBeltLeftUnbreakable */

BrickBlueConveyorBeltLeftUnbreakable::BrickBlueConveyorBeltLeftUnbreakable() :
        Brick(EntityIdentificator::BRICK_BLUE_CONVEYOR_BELT_LEFT_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickBlueConveyorBeltLeftUnbreakable::PrintName() const {
        std::cout << "BrickBlueConveyorBeltLeftUnbreakable." << std::endl;
}

void BrickBlueConveyorBeltLeftUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueConveyorBeltLeftUnbreakableAnimation::BRICK_BLUE_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY);
}

IEntity* BrickBlueConveyorBeltLeftUnbreakable::Create() {
        return new BrickBlueConveyorBeltLeftUnbreakable();
}

void BrickBlueConveyorBeltLeftUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBlueConveyorBeltLeftUnbreakableAnimation::BRICK_BLUE_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY);
}

/* BrickGreenConveyorBeltRightUnbreakable */

BrickGreenConveyorBeltRightUnbreakable::BrickGreenConveyorBeltRightUnbreakable() :
        Brick(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_RIGHT_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickGreenConveyorBeltRightUnbreakable::PrintName() const {
        std::cout << "BrickGreenConveyorBeltRightUnbreakable." << std::endl;
}

void BrickGreenConveyorBeltRightUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenConveyorBeltRightUnbreakableAnimation::BRICK_GREEN_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY);
}

IEntity* BrickGreenConveyorBeltRightUnbreakable::Create() {
        return new BrickGreenConveyorBeltRightUnbreakable();
}

void BrickGreenConveyorBeltRightUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickGreenConveyorBeltRightUnbreakableAnimation::BRICK_GREEN_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY);
}

/* BrickGreenConveyorBeltLeftUnbreakable */

BrickGreenConveyorBeltLeftUnbreakable::BrickGreenConveyorBeltLeftUnbreakable() :
        Brick(EntityIdentificator::BRICK_GREEN_CONVEYOR_BELT_LEFT_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickGreenConveyorBeltLeftUnbreakable::PrintName() const {
        std::cout << "BrickGreenConveyorBeltLeftUnbreakable." << std::endl;
}

void BrickGreenConveyorBeltLeftUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenConveyorBeltLeftUnbreakableAnimation::BRICK_GREEN_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY);
}

IEntity* BrickGreenConveyorBeltLeftUnbreakable::Create() {
        return new BrickGreenConveyorBeltLeftUnbreakable();
}

void BrickGreenConveyorBeltLeftUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickGreenConveyorBeltLeftUnbreakableAnimation::BRICK_GREEN_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY);
}

/* BrickBrownConveyorBeltRightUnbreakable */

BrickBrownConveyorBeltRightUnbreakable::BrickBrownConveyorBeltRightUnbreakable() :
        Brick(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_RIGHT_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::MOBILE_RIGHT, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickBrownConveyorBeltRightUnbreakable::PrintName() const {
        std::cout << "BrickBrownConveyorBeltRightUnbreakable." << std::endl;
}

void BrickBrownConveyorBeltRightUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownConveyorBeltRightUnbreakableAnimation::BRICK_BROWN_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY);
}

IEntity* BrickBrownConveyorBeltRightUnbreakable::Create() {
        return new BrickBrownConveyorBeltRightUnbreakable();
}

void BrickBrownConveyorBeltRightUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownConveyorBeltRightUnbreakableAnimation::BRICK_BROWN_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY);
}

/* BrickBrownConveyorBeltLeftUnbreakable */

BrickBrownConveyorBeltLeftUnbreakable::BrickBrownConveyorBeltLeftUnbreakable() :
        Brick(EntityIdentificator::BRICK_BROWN_CONVEYOR_BELT_LEFT_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::MOBILE_LEFT, BrickStateIdentificator::BRICK_MAX_STATES, false, false) {
}

void BrickBrownConveyorBeltLeftUnbreakable::PrintName() const {
        std::cout << "BrickBrownConveyorBeltLeftUnbreakable." << std::endl;
}

void BrickBrownConveyorBeltLeftUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownConveyorBeltLeftUnbreakableAnimation::BRICK_BROWN_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY);
}

IEntity* BrickBrownConveyorBeltLeftUnbreakable::Create() {
        return new BrickBrownConveyorBeltLeftUnbreakable();
}

void BrickBrownConveyorBeltLeftUnbreakable::STATE_Sticky()
{
        LoadAnimationWithId(BrickBrownConveyorBeltLeftUnbreakableAnimation::BRICK_BROWN_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY);
}