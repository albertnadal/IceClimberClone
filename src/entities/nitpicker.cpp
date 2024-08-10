#include <entities/nitpicker.h>
#include <chrono>

Nitpicker::Nitpicker() :
        IEntity(EntityIdentificator::NITPICKER, EntityType::ENEMY, SurfaceType::SIMPLE, NitpickerStateIdentificator::NITPICKER_MAX_STATES, false, true) {
    vectorDirection.x = 0;
    vectorDirection.y = 0;
}

void Nitpicker::PrintName() {
    std::cout << "Nitpicker." << std::endl;
}

inline bool Nitpicker::ReachedScreenEdge() {
    return (position.GetRealX() < 0.0f) || (position.GetRealX() >= MOUNTAIN_WIDTH_FLOAT - (Width() >> 1));
}

void Nitpicker::SetRandomFlyStartPosition() {
    direction = (rand() % 2 == 0) ? Direction::RIGHT : Direction::LEFT; // Random initial direction.
    if (direction == Direction::RIGHT) {
        PositionSetX(0.0f);
        ExternalEvent(NitpickerStateIdentificator::STATE_FLY_RIGHT, nullptr);
    } else {
        PositionSetX(MOUNTAIN_WIDTH_FLOAT - (Width() >> 1));
        ExternalEvent(NitpickerStateIdentificator::STATE_FLY_LEFT, nullptr);
    }
}

bool Nitpicker::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;
 
    if (isFlying) {
        MoveTo(direction, 0.5f);
        if (ReachedScreenEdge()) {
            SetRandomFlyStartPosition();
        }
        needRedraw = true;
    }
    else if (isFalling) {
        PositionAddY(3.5f);  // Simple linear fall instead of parabolic
        UpdatePositionInSpacePartitionTree();
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

void Nitpicker::MoveTo(Direction direction, float distance) {
    if (!isFalling) {
        PositionAddX(direction == Direction::RIGHT ? distance : -(distance));
        vectorDirection.x = (direction == Direction::RIGHT ? 1 : -1);
        UpdatePositionInSpacePartitionTree();
    }
}

void Nitpicker::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    SetRandomFlyStartPosition(); // Set random initial position, direction and state
}

void Nitpicker::LoadNextSprite() {
    SpriteData spriteData = NextSpriteData();

    if (spriteData.beginNewLoop) {
        if (ShouldBeginAnimationLoopAgain()) {
            spriteData = NextSpriteData();
        }
    }

    nextSpriteTime = (chrono::system_clock::now() + chrono::milliseconds(spriteData.duration));

    currentSprite.width = spriteData.width;
    currentSprite.height = spriteData.height;
    currentSprite.xOffset = spriteData.xOffset;
    currentSprite.yOffset = spriteData.yOffset;
    currentSprite.u1 = spriteData.u1;
    currentSprite.v1 = spriteData.v1;
    currentSprite.u2 = spriteData.u2;
    currentSprite.v2 = spriteData.v2;

    // Adjusts object position according to the sprite offset
    PositionSetOffset(spriteData.xOffset, spriteData.yOffset);

    recalculateAreasDataIsNeeded = true; // Is necessary because the current sprite may have different areas
    boundingBox = {spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY};
    solidBoundingBox = {spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY};
    firstSpriteOfCurrentAnimationIsLoaded = true;
}

IEntity *Nitpicker::Create() {
    return new Nitpicker();
}

Nitpicker::~Nitpicker() = default;

bool Nitpicker::ShouldBeginAnimationLoopAgain() {
    return false;
}

void Nitpicker::STATE_Fly_Right() {
    isFlying = true;
    isFalling = false;
    direction = Direction::RIGHT;
    LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_RIGHT);
}

void Nitpicker::STATE_Fly_Left() {
    isFlying = true;
    isFalling = false;
    direction = Direction::LEFT;
    LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_LEFT);
}
