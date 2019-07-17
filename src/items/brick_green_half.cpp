#include "brick_green_half.h"
#include <chrono>

BrickGreenHalf::BrickGreenHalf() :
        Brick(SceneObjectIdentificator::BRICK_GREEN_HALF, BrickStateIdentificator::BRICK_MAX_STATES) {
}

void BrickGreenHalf::PrintName() {
        std::cout << "BrickGreenHalf." << std::endl;
}

void BrickGreenHalf::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickGreenHalfAnimation::BRICK_GREEN_HALF_STICKY);
}

ISceneObject* BrickGreenHalf::Create() {
        return new BrickGreenHalf();
}

BrickGreenHalf::~BrickGreenHalf() {

}
