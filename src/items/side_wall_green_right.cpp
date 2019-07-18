#include "side_wall_green_right.h"
#include <chrono>

SideWallGreenRight::SideWallGreenRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_RIGHT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
