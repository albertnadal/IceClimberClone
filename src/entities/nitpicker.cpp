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

void Nitpicker::Respawn() {
    PositionSetY(entityManager->GetCurrentCameraVerticalPosition() + NITPICKER_RESPAWN_TOP_MARGIN); // Use the viewport vertical position as respawn position.

    direction = (rand() % 2 == 0) ? Direction::RIGHT : Direction::LEFT; // Random initial direction.
    if (direction == Direction::RIGHT) {
        PositionSetX(0.0f);
        ExternalEvent(NitpickerStateIdentificator::STATE_FLY_RIGHT, nullptr);
    } else {
        PositionSetX(MOUNTAIN_WIDTH_FLOAT - (Width() >> 1));
        ExternalEvent(NitpickerStateIdentificator::STATE_FLY_LEFT, nullptr);
    }

    CalculateNewFlyingRoute();
    flyingRouteIt = flyingRoute.begin();
    isWaitingForRespawn = false;
    isFlying = true;
}

bool Nitpicker::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;
 
    if (isWaitingForRespawn && nextRespawnTime.has_value() && (chrono::system_clock::now() >= nextRespawnTime.value())) {
        Respawn();
    }
    else if (isFlying) {
        speedVector.first += (flyingRouteIt->first < position.GetCoordinate().first) ? -0.01 : 0.01;
        speedVector.second += (flyingRouteIt->second < position.GetCoordinate().second) ? -0.01 : 0.01;
        position.ApplyDelta(speedVector.first, speedVector.second);
        UpdatePositionInSpacePartitionTree();

        speedVector.second = std::max(-NITPICKER_MAX_SPEED, std::min(NITPICKER_MAX_SPEED, speedVector.second));
        speedVector.first = std::max(-NITPICKER_MAX_SPEED, std::min(NITPICKER_MAX_SPEED, speedVector.first));

        // Go fly to the next waypoint of the flying route when reached current waypoint.
        if (calculateDistance(*flyingRouteIt, position.GetCoordinate()) <= 30.0f) {
            ++flyingRouteIt;
        }

        // Stop flying when arriving at the final route waypoint.
        if (flyingRouteIt == flyingRoute.end()) {
            CalculateNewFlyingRoute();
            flyingRouteIt = flyingRoute.begin();
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

float Nitpicker::calculateDistance(const std::pair<float, float>& a, const std::pair<float, float>& b) {
    return std::sqrt(std::pow(b.first - a.first, 2) + std::pow(b.second - a.second, 2));
}

void Nitpicker::CalculateNewFlyingRoute() {
    if (entityManager == nullptr) {
        return;
    }

    std::optional<Position *> playerPosition = entityManager->GetPlayerPosition();
    if (!playerPosition.has_value()) {
        return;
    }

    flyingRoute.clear();

    std::pair<float, float> startCoord = position.GetCoordinate();
    std::pair<float, float> endCoord = playerPosition.value()->GetCoordinate();
    std::pair<float, float> lastCoord = startCoord;

    for (int i = 0; i < NITPICKER_NUM_WAYPOINTS; ++i) {
        std::pair<float, float> nextCoord;
        float lastDistance = calculateDistance(lastCoord, endCoord);
        do {
            nextCoord.first = lastCoord.first + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (endCoord.first - lastCoord.first)));
            nextCoord.second = lastCoord.second + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (endCoord.second - lastCoord.second)));
        } while (calculateDistance(nextCoord, endCoord) >= lastDistance);

        flyingRoute.push_back(nextCoord);
        lastCoord = nextCoord;
    }

    flyingRoute.push_back(endCoord);
}

void Nitpicker::WaitUntilRespawnTime() {
    isWaitingForRespawn = true;
    isFlying = false;
    isFalling = false;
    nextRespawnTime = (chrono::system_clock::now() + std::chrono::milliseconds(NITPICKER_RESPAWN_WAIT_TIME_MILLISECONDS));
    PositionSetXY(-100.0f, -100.0f);  // Hide the Nitpicker out of the viewport.
}

void Nitpicker::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    WaitUntilRespawnTime();
}

IEntity *Nitpicker::Create() {
    return new Nitpicker();
}

Nitpicker::~Nitpicker() = default;

bool Nitpicker::ShouldBeginAnimationLoopAgain() {
    return false;
}

void Nitpicker::STATE_Waiting_Respawn() {
    isWaitingForRespawn = true;
    isFlying = false;
    isFalling = false;
}

void Nitpicker::STATE_Fly_Right() {
    isWaitingForRespawn = false;
    isFlying = true;
    isFalling = false;
    direction = Direction::RIGHT;
    LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_RIGHT);
}

void Nitpicker::STATE_Fly_Left() {
    isWaitingForRespawn = false;
    isFlying = true;
    isFalling = false;
    direction = Direction::LEFT;
    LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_LEFT);
}
