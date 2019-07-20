#include "side_wall_brown_left.h"
#include <chrono>

SideWallBrownLeft::SideWallBrownLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BROWN_LEFT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
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
