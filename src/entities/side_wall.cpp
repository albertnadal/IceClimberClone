#include <entities/side_wall.h>
#include <chrono>

SideWall::SideWall() :
        IEntity(EntityIdentificator::SIDE_WALL, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

SideWall::SideWall(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char max_states, bool is_breakable, bool is_traversable) :
        IEntity(_id, _type, surface_type, max_states, is_breakable, is_traversable) {
}

void SideWall::PrintName() {
        std::cout << "SideWall." << std::endl;
}

bool SideWall::IsCloud() {
  return false;
}

bool SideWall::Update(uint8_t pressedKeys_) {
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

void SideWall::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
}

IEntity* SideWall::Create() {
          return new SideWall();
}

SideWall::~SideWall() {

}

/* SideWallBlueLeft */

SideWallBlueLeft::SideWallBlueLeft() :
        SideWall(EntityIdentificator::SIDE_WALL_BLUE_LEFT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBlueLeft::PrintName() {
        std::cout << "SideWallBlueLeft." << std::endl;
}

void SideWallBlueLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueLeftAnimation::SIDE_WALL_BLUE_LEFT_STICKY);
}

IEntity* SideWallBlueLeft::Create() {
        return new SideWallBlueLeft();
}

/* SideWallBlueRight */

SideWallBlueRight::SideWallBlueRight() :
        SideWall(EntityIdentificator::SIDE_WALL_BLUE_RIGHT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBlueRight::PrintName() {
        std::cout << "SideWallBlueRight." << std::endl;
}

void SideWallBlueRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueRightAnimation::SIDE_WALL_BLUE_RIGHT_STICKY);
}

IEntity* SideWallBlueRight::Create() {
        return new SideWallBlueRight();
}

/* SideWallBlueColumnsLeft */

SideWallBlueColumnsLeft::SideWallBlueColumnsLeft() :
        SideWall(EntityIdentificator::SIDE_WALL_BLUE_COLUMNS_LEFT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBlueColumnsLeft::PrintName() {
        std::cout << "SideWallBlueColumnsLeft." << std::endl;
}

void SideWallBlueColumnsLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueColumnsLeftAnimation::SIDE_WALL_BLUE_COLUMNS_LEFT_STICKY);
}

IEntity* SideWallBlueColumnsLeft::Create() {
        return new SideWallBlueColumnsLeft();
}

/* SideWallBlueColumnsRight */

SideWallBlueColumnsRight::SideWallBlueColumnsRight() :
        SideWall(EntityIdentificator::SIDE_WALL_BLUE_COLUMNS_RIGHT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBlueColumnsRight::PrintName() {
        std::cout << "SideWallBlueColumnsRight." << std::endl;
}

void SideWallBlueColumnsRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueColumnsRightAnimation::SIDE_WALL_BLUE_COLUMNS_RIGHT_STICKY);
}

IEntity* SideWallBlueColumnsRight::Create() {
        return new SideWallBlueColumnsRight();
}

/* SideWallBrownLeft */

SideWallBrownLeft::SideWallBrownLeft() :
        SideWall(EntityIdentificator::SIDE_WALL_BROWN_LEFT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBrownLeft::PrintName() {
        std::cout << "SideWallBrownLeft." << std::endl;
}

void SideWallBrownLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownLeftAnimation::SIDE_WALL_BROWN_LEFT_STICKY);
}

IEntity* SideWallBrownLeft::Create() {
        return new SideWallBrownLeft();
}

/* SideWallBrownRight */

SideWallBrownRight::SideWallBrownRight() :
        SideWall(EntityIdentificator::SIDE_WALL_BROWN_RIGHT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBrownRight::PrintName() {
        std::cout << "SideWallBrownRight." << std::endl;
}

void SideWallBrownRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownRightAnimation::SIDE_WALL_BROWN_RIGHT_STICKY);
}

IEntity* SideWallBrownRight::Create() {
        return new SideWallBrownRight();
}

/* SideWallBrownColumnsLeft */

SideWallBrownColumnsLeft::SideWallBrownColumnsLeft() :
        SideWall(EntityIdentificator::SIDE_WALL_BROWN_COLUMNS_LEFT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBrownColumnsLeft::PrintName() {
        std::cout << "SideWallBrownColumnsLeft." << std::endl;
}

void SideWallBrownColumnsLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownColumnsLeftAnimation::SIDE_WALL_BROWN_COLUMNS_LEFT_STICKY);
}

IEntity* SideWallBrownColumnsLeft::Create() {
        return new SideWallBrownColumnsLeft();
}

/* SideWallBrownColumnsRight */

SideWallBrownColumnsRight::SideWallBrownColumnsRight() :
        SideWall(EntityIdentificator::SIDE_WALL_BROWN_COLUMNS_RIGHT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallBrownColumnsRight::PrintName() {
        std::cout << "SideWallBrownColumnsRight." << std::endl;
}

void SideWallBrownColumnsRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownColumnsRightAnimation::SIDE_WALL_BROWN_COLUMNS_RIGHT_STICKY);
}

IEntity* SideWallBrownColumnsRight::Create() {
        return new SideWallBrownColumnsRight();
}

/* SideWallGreenLeft */

SideWallGreenLeft::SideWallGreenLeft() :
        SideWall(EntityIdentificator::SIDE_WALL_GREEN_LEFT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallGreenLeft::PrintName() {
        std::cout << "SideWallGreenLeft." << std::endl;
}

void SideWallGreenLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenLeftAnimation::SIDE_WALL_GREEN_LEFT_STICKY);
}

IEntity* SideWallGreenLeft::Create() {
        return new SideWallGreenLeft();
}

/* SideWallGreenRight */

SideWallGreenRight::SideWallGreenRight() :
        SideWall(EntityIdentificator::SIDE_WALL_GREEN_RIGHT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallGreenRight::PrintName() {
        std::cout << "SideWallGreenRight." << std::endl;
}

void SideWallGreenRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenRightAnimation::SIDE_WALL_GREEN_RIGHT_STICKY);
}

IEntity* SideWallGreenRight::Create() {
        return new SideWallGreenRight();
}

/* SideWallGreenColumnsLeft */

SideWallGreenColumnsLeft::SideWallGreenColumnsLeft() :
        SideWall(EntityIdentificator::SIDE_WALL_GREEN_COLUMNS_LEFT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallGreenColumnsLeft::PrintName() {
        std::cout << "SideWallGreenColumnsLeft." << std::endl;
}

void SideWallGreenColumnsLeft::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenColumnsLeftAnimation::SIDE_WALL_GREEN_COLUMNS_LEFT_STICKY);
}

IEntity* SideWallGreenColumnsLeft::Create() {
        return new SideWallGreenColumnsLeft();
}

/* SideWallGreenColumnsRight */

SideWallGreenColumnsRight::SideWallGreenColumnsRight() :
        SideWall(EntityIdentificator::SIDE_WALL_GREEN_COLUMNS_RIGHT, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, true) {
}

void SideWallGreenColumnsRight::PrintName() {
        std::cout << "SideWallGreenColumnsRight." << std::endl;
}

void SideWallGreenColumnsRight::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenColumnsRightAnimation::SIDE_WALL_GREEN_COLUMNS_RIGHT_STICKY);
}

IEntity* SideWallGreenColumnsRight::Create() {
        return new SideWallGreenColumnsRight();
}

/* SideWallIceModelAUnbreakable */

SideWallIceModelAUnbreakable::SideWallIceModelAUnbreakable() :
        SideWall(EntityIdentificator::SIDE_WALL_ICE_MODEL_A_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, false) {
}

void SideWallIceModelAUnbreakable::PrintName() {
        std::cout << "SideWallIceModelAUnbreakable." << std::endl;
}

void SideWallIceModelAUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallIceModelAUnbreakableAnimation::SIDE_WALL_ICE_MODEL_A_UNBREAKABLE_STICKY);
}

IEntity* SideWallIceModelAUnbreakable::Create() {
        return new SideWallIceModelAUnbreakable();
}

/* SideWallIceModelBUnbreakable */

SideWallIceModelBUnbreakable::SideWallIceModelBUnbreakable() :
        SideWall(EntityIdentificator::SIDE_WALL_ICE_MODEL_B_UNBREAKABLE, EntityType::TERRAIN, SurfaceType::SIMPLE, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false, false) {
}

void SideWallIceModelBUnbreakable::PrintName() {
        std::cout << "SideWallIceModelBUnbreakable." << std::endl;
}

void SideWallIceModelBUnbreakable::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallIceModelBUnbreakableAnimation::SIDE_WALL_ICE_MODEL_B_UNBREAKABLE_STICKY);
}

IEntity* SideWallIceModelBUnbreakable::Create() {
        return new SideWallIceModelBUnbreakable();
}
