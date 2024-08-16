#include <entities/condor.h>
#include <chrono>

Condor::Condor() :
        IEntity(EntityIdentificator::CONDOR, EntityType::ENEMY, SurfaceType::SIMPLE, CondorStateIdentificator::CONDOR_MAX_STATES, false, true) {
}

void Condor::PrintName() {
    std::cout << "Condor." << std::endl;
}

inline bool Condor::ReachedScreenEdge() {
    return ((direction == Direction::LEFT) && (position.GetRealX() <= -Width())) || ((direction == Direction::RIGHT) && (position.GetRealX() >= MOUNTAIN_WIDTH_FLOAT));
}

void Condor::StartFlight() {
    if (!direction.has_value()) {
        direction = (rand() % 2 == 0) ? Direction::RIGHT : Direction::LEFT; // Random initial direction.
    }

    if (direction == Direction::RIGHT) {
        PositionSetX(-Width());
        LoadAnimationWithId(CondorAnimation::CONDOR_FLY_TO_RIGHT);
    } else {
        PositionSetX(MOUNTAIN_WIDTH_FLOAT);
        LoadAnimationWithId(CondorAnimation::CONDOR_FLY_TO_LEFT);
    }
    isFlying = true;
}

bool Condor::Update(const uint8_t pressedKeys_) {
    if (isHaunted) {
        return false;
    }

    bool needRedraw = false;
 
    if (isFlying && direction.has_value()) {
        MoveTo(*direction, 2.5f);
        if (ReachedScreenEdge()) {
            StartFlight();
        }
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
    isFlying = false;
    isHaunted = true;
}

void Condor::MoveTo(Direction direction, float distance) {
    PositionAddX(direction == Direction::RIGHT ? distance : -(distance));
    UpdatePositionInSpacePartitionTree();
}

void Condor::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    StartFlight(); // Set random initial position and direction.
}

IEntity *Condor::Create() {
    return new Condor();
}

Condor::~Condor() = default;
