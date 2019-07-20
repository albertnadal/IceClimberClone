#include "side_wall_blue_columns_left.h"
#include <chrono>

SideWallBlueColumnsLeft::SideWallBlueColumnsLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_COLUMNS_LEFT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
