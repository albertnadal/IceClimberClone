#include "side_wall.h"
#include <chrono>

SideWall::SideWall() :
        ISceneObject(SceneObjectIdentificator::SIDE_WALL, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

SideWall::SideWall(SceneObjectIdentificator scene_id, SceneObjectType scene_type, unsigned char max_states, bool is_breakable) :
        ISceneObject(scene_id, scene_type, max_states, is_breakable) {
}

uint16_t SideWall::Width() {
        return currentSprite.width;
}

uint16_t SideWall::Height() {
        return currentSprite.height;
}

void SideWall::PrintName() {
        std::cout << "SideWall." << std::endl;
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

void SideWall::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
}

void SideWall::LoadAnimationWithId(uint16_t animationId) {
        ObjectSpriteSheetAnimation *currentAnimation = spriteSheet->GetAnimationWithId(animationId);
        currentAnimationSprites = currentAnimation->GetSprites();
        animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        animationLoaded = true;
        firstSpriteOfCurrentAnimationIsLoaded = false;
        nextSpriteTime = std::chrono::system_clock::now();
}

void SideWall::LoadNextSprite()
{
  SpriteData spriteData = NextSpriteData();
  if(spriteData.beginNewLoop) {
          cout << "*** BEGIN NEW LOOP ***" << endl;
          if(BeginAnimationLoopAgain()) {
            spriteData = NextSpriteData();
          }
  }

  nextSpriteTime = (chrono::system_clock::now() + std::chrono::milliseconds(spriteData.duration));

  currentSprite.width = spriteData.width;
  currentSprite.height = spriteData.height;
  currentSprite.u1 = spriteData.u1;
  currentSprite.v1 = spriteData.v1;
  currentSprite.u2 = spriteData.u2;
  currentSprite.v2 = spriteData.v2;
  boundingBox = { spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY };
  firstSpriteOfCurrentAnimationIsLoaded = true;
}

SpriteData SideWall::NextSpriteData()
{
        if(currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
                currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
                (*currentAnimationSpriteIterator).beginNewLoop = true;
        }

        return *currentAnimationSpriteIterator++;
}

ISceneObject* SideWall::Create() {
          return new SideWall();
}

SideWall::~SideWall() {

}

bool SideWall::BeginAnimationLoopAgain()
{
        return false;
}

/* SideWallBlueLeft */

SideWallBlueLeft::SideWallBlueLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_LEFT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBlueLeft::PrintName() {
        std::cout << "SideWallBlueLeft." << std::endl;
}

void SideWallBlueLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueLeftAnimation::SIDE_WALL_BLUE_LEFT_STICKY);
}

ISceneObject* SideWallBlueLeft::Create() {
        return new SideWallBlueLeft();
}

SideWallBlueLeft::~SideWallBlueLeft() {

}

/* SideWallBlueRight */

SideWallBlueRight::SideWallBlueRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBlueRight::PrintName() {
        std::cout << "SideWallBlueRight." << std::endl;
}

void SideWallBlueRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueRightAnimation::SIDE_WALL_BLUE_RIGHT_STICKY);
}

ISceneObject* SideWallBlueRight::Create() {
        return new SideWallBlueRight();
}

SideWallBlueRight::~SideWallBlueRight() {

}

/* SideWallBlueColumnsLeft */

SideWallBlueColumnsLeft::SideWallBlueColumnsLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_COLUMNS_LEFT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBlueColumnsLeft::PrintName() {
        std::cout << "SideWallBlueColumnsLeft." << std::endl;
}

void SideWallBlueColumnsLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueColumnsLeftAnimation::SIDE_WALL_BLUE_COLUMNS_LEFT_STICKY);
}

ISceneObject* SideWallBlueColumnsLeft::Create() {
        return new SideWallBlueColumnsLeft();
}

SideWallBlueColumnsLeft::~SideWallBlueColumnsLeft() {

}

/* SideWallBlueColumnsRight */

SideWallBlueColumnsRight::SideWallBlueColumnsRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_COLUMNS_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBlueColumnsRight::PrintName() {
        std::cout << "SideWallBlueColumnsRight." << std::endl;
}

void SideWallBlueColumnsRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueColumnsRightAnimation::SIDE_WALL_BLUE_COLUMNS_RIGHT_STICKY);
}

ISceneObject* SideWallBlueColumnsRight::Create() {
        return new SideWallBlueColumnsRight();
}

SideWallBlueColumnsRight::~SideWallBlueColumnsRight() {

}

/* SideWallBrownLeft */

SideWallBrownLeft::SideWallBrownLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_LEFT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBrownLeft::PrintName() {
        std::cout << "SideWallBrownLeft." << std::endl;
}

void SideWallBrownLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownLeftAnimation::SIDE_WALL_BROWN_LEFT_STICKY);
}

ISceneObject* SideWallBrownLeft::Create() {
        return new SideWallBrownLeft();
}

SideWallBrownLeft::~SideWallBrownLeft() {

}

/* SideWallBrownRight */

SideWallBrownRight::SideWallBrownRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBrownRight::PrintName() {
        std::cout << "SideWallBrownRight." << std::endl;
}

void SideWallBrownRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownRightAnimation::SIDE_WALL_BROWN_RIGHT_STICKY);
}

ISceneObject* SideWallBrownRight::Create() {
        return new SideWallBrownRight();
}

SideWallBrownRight::~SideWallBrownRight() {

}

/* SideWallBrownColumnsLeft */

SideWallBrownColumnsLeft::SideWallBrownColumnsLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_COLUMNS_LEFT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBrownColumnsLeft::PrintName() {
        std::cout << "SideWallBrownColumnsLeft." << std::endl;
}

void SideWallBrownColumnsLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownColumnsLeftAnimation::SIDE_WALL_BROWN_COLUMNS_LEFT_STICKY);
}

ISceneObject* SideWallBrownColumnsLeft::Create() {
        return new SideWallBrownColumnsLeft();
}

SideWallBrownColumnsLeft::~SideWallBrownColumnsLeft() {

}

/* SideWallBrownColumnsRight */

SideWallBrownColumnsRight::SideWallBrownColumnsRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_COLUMNS_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallBrownColumnsRight::PrintName() {
        std::cout << "SideWallBrownColumnsRight." << std::endl;
}

void SideWallBrownColumnsRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBrownColumnsRightAnimation::SIDE_WALL_BROWN_COLUMNS_RIGHT_STICKY);
}

ISceneObject* SideWallBrownColumnsRight::Create() {
        return new SideWallBrownColumnsRight();
}

SideWallBrownColumnsRight::~SideWallBrownColumnsRight() {

}

/* SideWallGreenLeft */

SideWallGreenLeft::SideWallGreenLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_LEFT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallGreenLeft::PrintName() {
        std::cout << "SideWallGreenLeft." << std::endl;
}

void SideWallGreenLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenLeftAnimation::SIDE_WALL_GREEN_LEFT_STICKY);
}

ISceneObject* SideWallGreenLeft::Create() {
        return new SideWallGreenLeft();
}

SideWallGreenLeft::~SideWallGreenLeft() {

}

/* SideWallGreenRight */

SideWallGreenRight::SideWallGreenRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallGreenRight::PrintName() {
        std::cout << "SideWallGreenRight." << std::endl;
}

void SideWallGreenRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenRightAnimation::SIDE_WALL_GREEN_RIGHT_STICKY);
}

ISceneObject* SideWallGreenRight::Create() {
        return new SideWallGreenRight();
}

SideWallGreenRight::~SideWallGreenRight() {

}

/* SideWallGreenColumnsLeft */

SideWallGreenColumnsLeft::SideWallGreenColumnsLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_COLUMNS_LEFT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallGreenColumnsLeft::PrintName() {
        std::cout << "SideWallGreenColumnsLeft." << std::endl;
}

void SideWallGreenColumnsLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenColumnsLeftAnimation::SIDE_WALL_GREEN_COLUMNS_LEFT_STICKY);
}

ISceneObject* SideWallGreenColumnsLeft::Create() {
        return new SideWallGreenColumnsLeft();
}

SideWallGreenColumnsLeft::~SideWallGreenColumnsLeft() {

}

/* SideWallGreenColumnsRight */

SideWallGreenColumnsRight::SideWallGreenColumnsRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_COLUMNS_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES, false) {
}

void SideWallGreenColumnsRight::PrintName() {
        std::cout << "SideWallGreenColumnsRight." << std::endl;
}

void SideWallGreenColumnsRight::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenColumnsRightAnimation::SIDE_WALL_GREEN_COLUMNS_RIGHT_STICKY);
}

ISceneObject* SideWallGreenColumnsRight::Create() {
        return new SideWallGreenColumnsRight();
}

SideWallGreenColumnsRight::~SideWallGreenColumnsRight() {

}