#include "brick_blue_half.h"
#include <chrono>

BrickBlueHalf::BrickBlueHalf() :
        Brick(SceneObjectIdentificator::BRICK_BLUE_HALF, BrickStateIdentificator::BRICK_MAX_STATES) {
}

void BrickBlueHalf::PrintName() {
        std::cout << "BrickBlueHalf." << std::endl;
}

void BrickBlueHalf::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBlueHalfAnimation::BRICK_BLUE_HALF_STICKY);
}

ISceneObject* BrickBlueHalf::Create() {
        return new BrickBlueHalf();
}

BrickBlueHalf::~BrickBlueHalf() {

}
