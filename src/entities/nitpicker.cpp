#include <entities/nitpicker.h>
#include <chrono>

Nitpicker::Nitpicker() :
        IEntity(EntityIdentificator::NITPICKER, EntityType::ENEMY, SurfaceType::SIMPLE, NitpickerStateIdentificator::NITPICKER_MAX_STATES, false, true) {
    numAttackAttempts = 0;
}

void Nitpicker::PrintName() {
    std::cout << "Nitpicker." << std::endl;
}

inline bool Nitpicker::ReachedScreenEdge() {
    return (position.GetRealX() < 0.0f) || (position.GetRealX() >= MOUNTAIN_WIDTH_FLOAT - (Width() >> 1));
}

void Nitpicker::UpdateFlight() {
    float horizontalSpeed = speedVector.first;

    speedVector.first += (flyingRouteIt->first < position.GetCoordinate().first) ? -NITPICKER_ACCELERATION : NITPICKER_ACCELERATION;
    speedVector.second += (flyingRouteIt->second < position.GetCoordinate().second) ? -NITPICKER_ACCELERATION : NITPICKER_ACCELERATION;
    position.ApplyDelta(speedVector.first, speedVector.second);

    UpdatePositionInSpacePartitionTree();

    speedVector.second = std::max(-NITPICKER_MAX_SPEED, std::min(NITPICKER_MAX_SPEED, speedVector.second));
    speedVector.first = std::max(-NITPICKER_MAX_SPEED, std::min(NITPICKER_MAX_SPEED, speedVector.first));

    // Change the animation when changing the horizontal speed direction
    if ((horizontalSpeed < 0.0f) && (speedVector.first >= 0.0f)) {
        LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_RIGHT);
    } else if ((horizontalSpeed >= 0.0f) && (speedVector.first < 0.0f)) {
        LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_LEFT);
    }

    // Go fly to the next waypoint of the flying route when reached current waypoint.
    if (calculateDistance(*flyingRouteIt, position.GetCoordinate()) <= 30.0f) {
        ++flyingRouteIt;
    }

    // Stop flying when arriving at the final route waypoint.
    if (flyingRouteIt == flyingRoute.end()) {
        if (isRetreating) {
            EndFlight();
            return;
        }

        if (++numAttackAttempts >= NITPICKER_MAX_CONSECUTIVE_ATTACK_ATTEMPS) {
            isRetreating = true;
        }

        CalculateNewFlyingRoute();
        flyingRouteIt = flyingRoute.begin();
    }
}

bool Nitpicker::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;
 
    if (isWaitingForRespawn && nextRespawnTime.has_value() && (chrono::system_clock::now() >= nextRespawnTime.value())) {
        StartFlight();
        needRedraw = true;
    }
    else if (isFlying) {
        UpdateFlight();
        needRedraw = true;
    }
    else if (isFalling) {
        PositionAddY(3.5f);  // Simple linear fall instead of parabolic

        if ((position.GetCellY() - position.GetInitialCellY()) >= NITPICKER_MAX_FALL_CELLS) {
            // Add Nitpicker into the partition objects tree again. When hit by the player then the Nitpicker is removed from the tree.
            if (!spacePartitionObjectsTree->particleExists(this)) {
                std::vector<int> lowerBound = GetLowerBound();
                std::vector<int> upperBound = GetUpperBound();
                spacePartitionObjectsTree->insertParticle(this, lowerBound, upperBound);
            }
            EndFall();
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

void Nitpicker::Hit(bool hitFromLeft) {
    if (isFlying) {
        RemoveFromSpacePartitionObjectsTree();
        StartFall();
    }
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
    std::pair<float, float> endCoord;

    if (isRetreating) {
        // Create a flight path to put the Nitpicker out of the screen
        endCoord = startCoord;
        endCoord.first = (rand() % 2 == 0) ? -Width() : MOUNTAIN_WIDTH_FLOAT;
    } else {
        // Create a flight path towards Popo position
        endCoord = playerPosition.value()->GetCoordinate();
    }
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
    isRetreating = false;
    isFalling = false;
    numAttackAttempts = 0;
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
    WaitUntilRespawnTime();
}

void Nitpicker::STATE_Flying() {
    isWaitingForRespawn = false;
    isFlying = true;
    isRetreating = false;
    isFalling = false;
    PositionSetY(entityManager->GetCurrentCameraVerticalPosition() + NITPICKER_RESPAWN_TOP_MARGIN); // Use the viewport vertical position as respawn position.

    direction = (rand() % 2 == 0) ? Direction::RIGHT : Direction::LEFT; // Random initial direction.
    if (direction == Direction::RIGHT) {
        PositionSetX(0.0f);
        LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_RIGHT);
    } else {
        PositionSetX(MOUNTAIN_WIDTH_FLOAT - (Width() >> 1));
        LoadAnimationWithId(NitpickerAnimation::NITPICKER_FLY_TO_LEFT);
    }

    CalculateNewFlyingRoute();
    flyingRouteIt = flyingRoute.begin();
}

void Nitpicker::STATE_Falling() {
    isWaitingForRespawn = false;
    isFlying = false;
    isRetreating = false;
    isFalling = true;
    LoadAnimationWithId(speedVector.first < 0.0f ? NitpickerAnimation::NITPICKER_FALL_LEFT : NitpickerAnimation::NITPICKER_FALL_RIGHT);
}
