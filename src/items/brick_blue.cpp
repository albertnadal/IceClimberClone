#include "brick_blue.h"
#include <chrono>

BrickBlue::BrickBlue() :
        Brick(SceneObjectIdentificator::BRICK_BLUE, BrickStateIdentificator::BRICK_MAX_STATES) {
}

void BrickBlue::PrintName() {
        std::cout << "BrickBlue." << std::endl;
}

void BrickBlue::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueAnimation::BRICK_BLUE_STICKY);
}

ISceneObject* BrickBlue::Create() {
        return new BrickBlue();
}

BrickBlue::~BrickBlue() {

}
