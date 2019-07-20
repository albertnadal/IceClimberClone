#include "side_wall_brown_columns_right.h"
#include <chrono>

SideWallBrownColumnsRight::SideWallBrownColumnsRight() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_COLUMNS_RIGHT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
