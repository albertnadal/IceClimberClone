#include <entities/condor.h>
#include <chrono>

Condor::Condor() :
        IEntity(EntityIdentificator::CONDOR, EntityType::ENEMY, SurfaceType::SIMPLE, CondorStateIdentificator::CONDOR_MAX_STATES, false, true) {
}

void Condor::PrintName() {
    std::cout << "Condor." << std::endl;
}

inline bool Condor::ReachedScreenEdge() {
    return (position.GetRealX() < 0.0f) || (position.GetRealX() >= MOUNTAIN_WIDTH_FLOAT - (Width() >> 1));
}

bool Condor::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;
 
    if (isFlying) {
        // TODO
        needRedraw = true;
    }

    if (!animationLoaded) {
        return false;
    }

    if (animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
        return false;
    }

    if (chrono::system_clock::now() >= nextSpriteTime) {
        // Load next sprite of the current animation
        LoadNextSprite();
        return true;
    }

    return needRedraw;
}

void Condor::Hit(bool hitFromLeft) {
    // TODO
}

void Condor::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    LoadAnimationWithId(CondorAnimation::CONDOR_FLY_TO_RIGHT);
}

IEntity *Condor::Create() {
    return new Condor();
}

Condor::~Condor() = default;
