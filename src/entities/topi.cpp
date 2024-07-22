#include <entities/topi.h>
#include <chrono>

Topi::Topi() :
        IEntity(EntityIdentificator::TOPI, EntityType::ENEMY, SurfaceType::SIMPLE, TopiStateIdentificator::TOPI_MAX_STATES, false, true) {
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    objectToCarryId = std::nullopt;
}

void Topi::PrintName() {
    std::cout << "Topi." << std::endl;
}

bool Topi::IsTopi() {
  return true;
}

inline bool Topi::ReachedScreenEdge() {
    return (position.GetRealX() < 0.0f) || (position.GetRealX() >= LEVEL_WIDTH_FLOAT - 8.0f);
}

void Topi::SetRandomWalkStartPosition() {
    direction = (rand() % 2 == 0) ? Direction::RIGHT : Direction::LEFT; // Random initial direction.
    if (direction == Direction::RIGHT) {
        PositionSetX(0.0f);
        ExternalEvent(TopiStateIdentificator::STATE_WALK_RIGHT, nullptr);
    } else {
        PositionSetX(LEVEL_WIDTH_FLOAT - 9.0f);
        ExternalEvent(TopiStateIdentificator::STATE_WALK_LEFT, nullptr);
    }
}

bool Topi::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;
 
    if (isWalking) {
        MoveTo(direction, 0.5f);
        if (ReachedScreenEdge()) {
            SetRandomWalkStartPosition();
        }
        needRedraw = true;
    }
    else if (isGoingToPickUpIce) {
        MoveTo(direction, 1.5f);
        if (ReachedScreenEdge()) {
            SetRandomWalkStartPosition(); // TODO: Use the proper function here
        }
        needRedraw = true;
    }
    else if (isFalling) {
        PositionAddY(3.5f);  // Simple linear fall instead of parabolic
        UpdatePositionInSpacePartitionTree();
        needRedraw = true;
    }
    else if (isGoingToRecover) {
        MoveTo(direction, 1.5f);
        if (ReachedScreenEdge()) {
            // Place Topi at its original position
            position.recoverInitialPosition();
            SetRandomWalkStartPosition();
        }
        needRedraw = true;
    }

    // Check for collisions
    UpdateCollisions();

    if (!animationLoaded) {
        return false;
    }

    if (animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
        return false;
    }

    if (chrono::system_clock::now() >= nextSpriteTime) {
        // Load next sprite of the current animation
        LoadNextSprite();

        // Check for possible collisions with the solid areas of the currrent sprite
        UpdateCollisions();

        return true;
    }

    return needRedraw;
}

void Topi::GetSolidCollisions(std::vector<ObjectCollision> &collisions, bool& topiIsSuspendedInTheAir, bool& topiFoundAHoleOnTheFloor) {
    // Check for collisions with other objects present in the scene.
    std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(GetLowerBound(), GetUpperBound());
    topiIsSuspendedInTheAir = false;
    topiFoundAHoleOnTheFloor = false;
    IEntity* underlyingObjectCandidate = nullptr;
    currentUnderlyingObject = nullptr;
    int minBottomIntersectionYUnderlyingObjectCandidate = 9999;
    int minIntersectionXDiffUnderlyingObjectCandidate = 9999;
    int numPixelsUnderlyingObjectsSurface = 0;

    for (auto intersection : objectIntersections) {
        if ((intersection.particle == this) || intersection.particle->isTraversable || (std::find(objectsToIgnoreDuringFall.begin(), objectsToIgnoreDuringFall.end(), intersection.particle) != objectsToIgnoreDuringFall.end())) {
            continue;
        }

        int horizontalCorrection = 0, verticalCorrection = 0;

        //std::cout << " ==== intersection.topIntersectionY: " << intersection.topIntersectionY << "\n";

        //std::cout << " [ START ] intersection.topIntersectionY: " << intersection.topIntersectionY << " | intersection.bottomIntersectionY: " << intersection.bottomIntersectionY << " | intersection.rightIntersectionX: " << intersection.rightIntersectionX << " | intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
        // Compute position correction when Topi collides when moving to the right
        if ((vectorDirection.x > 0) && (vectorDirection.y == 0) && (intersection.rightIntersectionX < 0) && (intersection.bottomIntersectionY != 0)) {
            horizontalCorrection = intersection.rightIntersectionX;
        }
        // Compute position correction when Topi collides when moving to the left
        else if ((vectorDirection.x < 0) && (vectorDirection.y == 0) && (intersection.leftIntersectionX > 0) && (intersection.bottomIntersectionY != 0)) {
            horizontalCorrection = intersection.leftIntersectionX;
        }
        // Compute position correction when Topi collides with the ground during a fall
        else if ((vectorDirection.y < 0) && (vectorDirection.x == 0) && (intersection.bottomIntersectionY < 0)) {
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset;
        }
        else {
            continue;
        }

        /*
        std::cout << " ---- vectorDirection.x: " << vectorDirection.x << "\n";
        std::cout << " ---- vectorDirection.y: " << vectorDirection.y << "\n";
        std::cout << " ---- intersection.rightIntersectionX: " << intersection.rightIntersectionX << "\n";
        std::cout << " ---- intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
        std::cout << " >>>> horizontalCorrection: " << horizontalCorrection << "\n";
        std::cout << " >>>> verticalCorrection: " << verticalCorrection << "\n";
        */
        collisions.push_back({intersection.particle, horizontalCorrection, verticalCorrection});

        if (intersection.bottomIntersectionY < minBottomIntersectionYUnderlyingObjectCandidate) {
            minBottomIntersectionYUnderlyingObjectCandidate = intersection.bottomIntersectionY;
            underlyingObjectCandidate = intersection.particle;
        }
    }

    std::cout << " > TOPI COLLIDES WITH " << objectIntersections.size() << " OBJECTS. " << collisions.size() << " CORRECTIONS NEEDED.\n";

    // Check if Topi is suspended in the air
    for (auto intersection : objectIntersections) {
        if (intersection.particle == this) {
            continue;
        }

        // Check if the intersection object is the proper underlying surface
        if (intersection.bottomIntersectionY == 0) {
            int intersectionXDiff = std::abs(std::abs(intersection.rightIntersectionX) - std::abs(intersection.leftIntersectionX));

            if (intersectionXDiff < minIntersectionXDiffUnderlyingObjectCandidate) {
                minIntersectionXDiffUnderlyingObjectCandidate = intersectionXDiff;
                underlyingObjectCandidate = intersection.particle;
            }

            int leftIntersection = std::min(std::abs(intersection.rightIntersectionX), currentSprite.width);
            int rightIntersection = std::min(std::abs(intersection.leftIntersectionX), currentSprite.width);
            numPixelsUnderlyingObjectsSurface += leftIntersection + rightIntersection - currentSprite.width;
        }
    }

    // Topi is considered to be suspended in the air when there are no underlying object or the underlying object surface
    // covers less o equal the half width of the Topi.
    if ((underlyingObjectCandidate == nullptr) || ((underlyingObjectCandidate != nullptr) && (numPixelsUnderlyingObjectsSurface <= (currentSprite.width >> 1)))) {
        topiIsSuspendedInTheAir = true;
        std::cout << "\n\n >>>>>>>>>> TOPI is suspended in the air <<<<<<<<<<<\n\n";

        if (underlyingObjectCandidate != nullptr) {
            currentUnderlyingObject = underlyingObjectCandidate;
        }
    }

    cout << "\n >>>>>>> TOPI UNDERLYING SURFACE: " << numPixelsUnderlyingObjectsSurface << " currentSprite.width: " << currentSprite.width << "\n";
    // Check if a hole is present on the ground based on an heuristic way.
    // If the number of pixels of the underlying surface is 3 pixels (or more) lower than the width of
    // the Topi then there is a hole under Topi. Note that screen edges are not taken in consideration.
    if ((underlyingObjectCandidate != nullptr) && !((position.GetRealX() < 0.0f) || (position.GetRealX() >= LEVEL_WIDTH_FLOAT - currentSprite.width)) && (numPixelsUnderlyingObjectsSurface <= currentSprite.width - 3)) {
        topiFoundAHoleOnTheFloor = true;
        objectToCarryId = underlyingObjectCandidate->id;
        //cout << " >>>>>>> TOPI NEED TO CARRY AN OBJECT OF TYPE: ";
        //underlyingObjectCandidate->PrintName();
        //cout << "\n\n";
    }
}

void Topi::UpdateCollisions() {
    std::vector<ObjectCollision> collisions;
    bool topiIsSuspendedInTheAir = false;
    bool topiFoundAHoleOnTheFloor = false;

    // Search for collisions with solid objects
    this->GetSolidCollisions(collisions, topiIsSuspendedInTheAir, topiFoundAHoleOnTheFloor);

    // Change state when Topi is suspended in the air (almost no ground under his feet).
    if (topiIsSuspendedInTheAir && (isGoingToPickUpIce || isGoingToRecover)) {
        // Ignore collision with current underlying object during Topi fall.
        if (currentUnderlyingObject != nullptr && (std::find(objectsToIgnoreDuringFall.begin(), objectsToIgnoreDuringFall.end(), currentUnderlyingObject) == objectsToIgnoreDuringFall.end())) {
            objectsToIgnoreDuringFall.push_back(currentUnderlyingObject);
        }

        SuspendedInTheAir();
        return;
    }

    // Change state when Topi detected a hole on the floor (run to pick up ice)
    if (topiFoundAHoleOnTheFloor && isWalking) {
        HoleDetectedWhenWalking();
        return;
    }

    if (collisions.empty()) {
        return;
    }

    // Get the major position correction of all collisions. Do not allow corrections higher than 10.
    int minHorizontalCorrection = 0, maxHorizontalCorrection = 0, minVerticalCorrection = 0, maxVerticalCorrection = 0;
    for (auto collision : collisions) {
        if (std::abs(collision.horizontalCorrection) <= 10) {
            minHorizontalCorrection = std::min(minHorizontalCorrection, collision.horizontalCorrection);
            maxHorizontalCorrection = std::max(maxHorizontalCorrection, collision.horizontalCorrection);
        }

        if (std::abs(collision.verticalCorrection) <= 10) {
            minVerticalCorrection = std::min(minVerticalCorrection, collision.verticalCorrection);
            maxVerticalCorrection = std::max(maxVerticalCorrection, collision.verticalCorrection);
        }
    }

    if (isFalling && minVerticalCorrection < 0) {
        // Topi collided with the ground (during a fall)
        std::cout << " ))))) COLLIDING WITH THE GROUND DURING FALL minVerticalCorrection: " << minVerticalCorrection << "\n";
        PositionAddY(static_cast<float>(minVerticalCorrection));
        UpdatePositionInSpacePartitionTree();
        FinishFall();
    }

    // Check for horizontal collisions
    if (isWalking || isGoingToRecover || isGoingToPickUpIce) {
        if (minHorizontalCorrection < 0 || maxHorizontalCorrection > 0) {
            float correction = (minHorizontalCorrection < 0) ? static_cast<float>(minHorizontalCorrection) : static_cast<float>(maxHorizontalCorrection);
            PositionAddX(correction);
            ChangeDirection();
            UpdatePositionInSpacePartitionTree();
            return;
        }
    }
}

void Topi::MoveTo(Direction direction, float distance) {
    if (!isFalling) {
        PositionAddX(direction == Direction::RIGHT ? distance : -(distance));
        vectorDirection.x = (direction == Direction::RIGHT ? 1 : -1);
        UpdatePositionInSpacePartitionTree();
    }
}

void Topi::FinishFall() {
    isFalling = false;
    objectsToIgnoreDuringFall.clear();
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    FallLanding();
}

void Topi::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    SetRandomWalkStartPosition(); // Set random initial position, direction and state
}

void Topi::LoadNextSprite() {
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

IEntity *Topi::Create() {
    return new Topi();
}

Topi::~Topi() = default;

bool Topi::ShouldBeginAnimationLoopAgain() {
    // TODO
    return false;
}

void Topi::STATE_Walk_Right() {
    cout << "\n >>>>>> TOPI::STATE_Walk_Right <<<<<<\n";
    isWalking = true;
    isGoingToPickUpIce = false;
    isGoingToRecover = false;
    isFalling = false;
    isDazed = false;
    direction = Direction::RIGHT;
    LoadAnimationWithId(TopiAnimation::TOPI_WALK_TO_RIGHT);
}

void Topi::STATE_Walk_Left() {
    cout << "\n >>>>>> TOPI::STATE_Walk_Left <<<<<<\n";
    isWalking = true;
    isGoingToPickUpIce = false;
    isGoingToRecover = false;
    isFalling = false;
    isDazed = false;
    direction = Direction::LEFT;
    LoadAnimationWithId(TopiAnimation::TOPI_WALK_TO_LEFT);
}

void Topi::STATE_Run_To_Pick_Up_Ice_Right() {
    cout << "\n >>>>>> TOPI::STATE_Run_To_Pick_Up_Ice_Right <<<<<<\n";
    isWalking = false;
    isGoingToPickUpIce = true;
    isGoingToRecover = false;
    isFalling = false;
    isDazed = false;
    direction = Direction::RIGHT;
    LoadAnimationWithId(TopiAnimation::TOPI_RUN_TO_RIGHT);
}

void Topi::STATE_Run_To_Pick_Up_Ice_Left() {
    cout << "\n >>>>>> TOPI::STATE_Run_To_Pick_Up_Ice_Left <<<<<<\n";
    isWalking = false;
    isGoingToPickUpIce = true;
    isGoingToRecover = false;
    isFalling = false;
    isDazed = false;
    direction = Direction::LEFT;
    LoadAnimationWithId(TopiAnimation::TOPI_RUN_TO_LEFT);
}

void Topi::STATE_Fall_Dazed_Right() {
    cout << "\n >>>>>> TOPI::STATE_Fall_Dazed_Right <<<<<<\n";
    isWalking = false;
    isGoingToPickUpIce = false;
    isGoingToRecover = false;
    isFalling = true;
    isDazed = true;
    direction = Direction::LEFT;
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    LoadAnimationWithId(TopiAnimation::TOPI_FALL_DAZED_RIGHT);
}

void Topi::STATE_Fall_Dazed_Left() {
    cout << "\n >>>>>> TOPI::STATE_Fall_Dazed_Left <<<<<<\n";
    isWalking = false;
    isGoingToPickUpIce = false;
    isGoingToRecover = false;
    isFalling = true;
    isDazed = true;
    direction = Direction::RIGHT;
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    LoadAnimationWithId(TopiAnimation::TOPI_FALL_DAZED_LEFT);
}

void Topi::STATE_Run_Dazed_Right() {
    cout << "\n >>>>>> TOPI::STATE_Run_Dazed_Right <<<<<<\n";
    isWalking = false;
    isGoingToPickUpIce = false;
    isGoingToRecover = true;
    isFalling = false;
    isDazed = true;
    direction = Direction::RIGHT;
    LoadAnimationWithId(TopiAnimation::RUN_DAZED_RIGHT);
}

void Topi::STATE_Run_Dazed_Left() {
    cout << "\n >>>>>> TOPI::STATE_Run_Dazed_Left <<<<<<\n";
    isWalking = false;
    isGoingToPickUpIce = false;
    isGoingToRecover = true;
    isFalling = false;
    isDazed = true;
    direction = Direction::LEFT;
    LoadAnimationWithId(TopiAnimation::RUN_DAZED_LEFT);
}

void Topi::HoleDetectedWhenWalking() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_LEFT)   // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_RIGHT)  // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::SuspendedInTheAir() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_RIGHT)          // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_LEFT)           // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_RIGHT)          // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_LEFT)           // STATE_Run_Dazed_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::FallLanding() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_LEFT)            // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_RIGHT)           // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::ChangeDirection() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (STATE_WALK_LEFT)                 // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (STATE_WALK_RIGHT)                // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_LEFT)   // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_RIGHT)  // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_LEFT)            // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_RIGHT)           // STATE_Run_Dazed_Left
    END_TRANSITION_MAP(nullptr)
}