#include "side_wall_blue_columns_right.h"
#include <chrono>

SideWallBlueColumnsRight::SideWallBlueColumnsRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_COLUMNS_RIGHT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
