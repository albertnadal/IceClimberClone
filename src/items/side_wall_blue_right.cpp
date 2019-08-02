#include "side_wall_blue_right.h"
#include <chrono>

SideWallBlueRight::SideWallBlueRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
