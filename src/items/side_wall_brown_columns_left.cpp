#include "side_wall_brown_columns_left.h"
#include <chrono>

SideWallBrownColumnsLeft::SideWallBrownColumnsLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_COLUMNS_LEFT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
