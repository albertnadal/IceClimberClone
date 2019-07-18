#include "side_wall_green_left.h"
#include <chrono>

SideWallGreenLeft::SideWallGreenLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_GREEN_LEFT, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
}

void SideWallGreenLeft::PrintName() {
        std::cout << "SideWallGreenLeft." << std::endl;
}

void SideWallGreenLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallGreenLeftAnimation::SIDE_WALL_GREEN_LEFT_STICKY);
}

ISceneObject* SideWallGreenLeft::Create() {
        return new SideWallGreenLeft();
}

SideWallGreenLeft::~SideWallGreenLeft() {

}
