#include "side_wall_blue_left.h"
#include <chrono>

SideWallBlueLeft::SideWallBlueLeft() :
        SideWall(SceneObjectIdentificator::SIDE_WALL_BLUE_LEFT, SceneObjectType::TERRAIN, SideWallStateIdentificator::SIDE_WALL_MAX_STATES) {
}

void SideWallBlueLeft::PrintName() {
        std::cout << "SideWallBlueLeft." << std::endl;
}

void SideWallBlueLeft::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(SideWallBlueLeftAnimation::SIDE_WALL_BLUE_LEFT_STICKY);
}

ISceneObject* SideWallBlueLeft::Create() {
        return new SideWallBlueLeft();
}

SideWallBlueLeft::~SideWallBlueLeft() {

}
