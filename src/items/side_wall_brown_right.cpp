#include "side_wall_brown_right.h"
#include <chrono>

SideWallBrownRight::SideWallBrownRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_RIGHT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
