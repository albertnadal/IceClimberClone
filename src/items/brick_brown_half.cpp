#include "brick_brown_half.h"
#include <chrono>

BrickBrownHalf::BrickBrownHalf() :
        Brick(SceneObjectIdentificator::BRICK_BROWN_HALF, SceneObjectType::TERRAIN, BrickStateIdentificator::BRICK_MAX_STATES) {
}

void BrickBrownHalf::PrintName() {
        std::cout << "BrickBrownHalf." << std::endl;
}

void BrickBrownHalf::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownHalfAnimation::BRICK_BROWN_HALF_STICKY);
}

ISceneObject* BrickBrownHalf::Create() {
        return new BrickBrownHalf();
}

BrickBrownHalf::~BrickBrownHalf() {

}
