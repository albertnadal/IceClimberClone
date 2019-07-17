#include "brick_brown.h"
#include <chrono>

BrickBrown::BrickBrown() :
        Brick(SceneObjectIdentificator::BRICK_BROWN, BrickStateIdentificator::BRICK_MAX_STATES) {
}

void BrickBrown::PrintName() {
        std::cout << "BrickBrown." << std::endl;
}

void BrickBrown::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickBrownAnimation::BRICK_BROWN_STICKY);
}

ISceneObject* BrickBrown::Create() {
        return new BrickBrown();
}

BrickBrown::~BrickBrown() {

}
