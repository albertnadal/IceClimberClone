#include "side_wall_green_columns_left.h"
#include <chrono>

SideWallGreenColumnsLeft::SideWallGreenColumnsLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_COLUMNS_LEFT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
