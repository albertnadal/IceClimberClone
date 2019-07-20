#include "side_wall_green_columns_right.h"
#include <chrono>

SideWallGreenColumnsRight::SideWallGreenColumnsRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_COLUMNS_RIGHT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
