#include <entities/topi.h>
#include <chrono>

Topi::Topi() :
        IEntity(EntityIdentificator::TOPI, EntityType::ENEMY, SurfaceType::SIMPLE, TopiStateIdentificator::TOPI_MAX_STATES, false, true) {
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    objectToCarryId = std::nullopt;
}

int Topi::Width() {
    return currentSprite.width;
}

int Topi::Height() {
    return currentSprite.height;
}

void Topi::PrintName() {
    std::cout << "Topi." << std::endl;
}

bool Topi::IsCloud() {
  return false;
}

void Topi::DisplaceIfUnderlyingSurfaceIsMobile() {
    // TODO
    /*
    if (!underlyingObjectSurfaceType.has_value()) {
        return;
    }

    float displacement = 0.0f;
    if ((*underlyingObjectSurfaceType == SurfaceType::MOBILE_RIGHT) && !isBlockedRight) {
        displacement = 1.0f;
        vectorDirection.x = 1;
    }
    else if ((*underlyingObjectSurfaceType == SurfaceType::MOBILE_LEFT) && !isBlockedLeft) {
        displacement = -1.0f;
        vectorDirection.x = -1;
    }
    else return;

    PositionAddX(displacement);
    */
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
        needRedraw = true;
    }/* else {

        // Displace the player if the underlying surface is mobile
        DisplaceIfUnderlyingSurfaceIsMobile();
    }
    */

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
    std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(GetSolidLowerBound(), GetSolidUpperBound());
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

        std::cout << " ==== intersection.topIntersectionY: " << intersection.topIntersectionY << "\n";

        std::cout << " [ START ] intersection.topIntersectionY: " << intersection.topIntersectionY << " | intersection.bottomIntersectionY: " << intersection.bottomIntersectionY << " | intersection.rightIntersectionX: " << intersection.rightIntersectionX << " | intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
        // Compute position correction when player collides walking to the right
        if ((vectorDirection.x > 0) && (vectorDirection.y == 0) && (intersection.rightIntersectionX < 0) && (intersection.bottomIntersectionY != 0)) {
            horizontalCorrection = intersection.rightIntersectionX;
        }
        // Compute position correction when player collides walking to the left
        else if ((vectorDirection.x < 0) && (vectorDirection.y == 0) && (intersection.leftIntersectionX > 0) && (intersection.bottomIntersectionY != 0)) {
            horizontalCorrection = intersection.leftIntersectionX;
        }
        // Compute position correction when player collides with the ground during the descending of a 90 degrees jump
        else if ((vectorDirection.y < 0) && (vectorDirection.x == 0) && (intersection.bottomIntersectionY < 0)) {
            std::cout << " [ CASE A ] intersection.topIntersectionY: " << intersection.bottomIntersectionY + currentSprite.yOffset + 1 << "\n";
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        else {
            continue;
        }

        std::cout << " ---- vectorDirection.x: " << vectorDirection.x << "\n";
        std::cout << " ---- vectorDirection.y: " << vectorDirection.y << "\n";
        std::cout << " ---- intersection.rightIntersectionX: " << intersection.rightIntersectionX << "\n";
        std::cout << " ---- intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
        std::cout << " >>>> horizontalCorrection: " << horizontalCorrection << "\n";
        std::cout << " >>>> verticalCorrection: " << verticalCorrection << "\n";
        collisions.push_back({intersection.particle, horizontalCorrection, verticalCorrection});

        if (intersection.bottomIntersectionY < minBottomIntersectionYUnderlyingObjectCandidate) {
            minBottomIntersectionYUnderlyingObjectCandidate = intersection.bottomIntersectionY;
            underlyingObjectCandidate = intersection.particle;
        }
    }

    std::cout << " > TOPI COLLIDES WITH " << objectIntersections.size() << " OBJECTS. " << collisions.size() << " CORRECTIONS NEEDED.\n";

    // Check if Topi is suspended in the air or get the underlying surface type
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
        isOnMobileSurface = false;
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
        cout << " >>>>>>> TOPI NEED TO CARRY AN OBJECT OF TYPE: ";
        underlyingObjectCandidate->PrintName();
        cout << "\n\n";
    }
}

void Topi::UpdateCollisions() {
    std::vector<ObjectCollision> collisions;
    bool topiIsSuspendedInTheAir = false;
    bool topiFoundAHoleOnTheFloor = false;

    // Search for collisions with solid objects
    this->GetSolidCollisions(collisions, topiIsSuspendedInTheAir, topiFoundAHoleOnTheFloor);

    // Change state when Topi is suspended in the air (almost no ground under his feet).
    if (topiIsSuspendedInTheAir && isGoingToPickUpIce) {
        cout << "\n\n ===================> TOPI is suspended in the air <====================\n\n";

        // Ignore collision with current underlying object during Topi fall.
        if (currentUnderlyingObject != nullptr && (std::find(objectsToIgnoreDuringFall.begin(), objectsToIgnoreDuringFall.end(), currentUnderlyingObject) == objectsToIgnoreDuringFall.end())) {
            objectsToIgnoreDuringFall.push_back(currentUnderlyingObject);
        }
        SuspendedInTheAir();
        return;
    }

    // Change state when Topi detected a hole on the floor (run to pick up ice)
    if (topiFoundAHoleOnTheFloor && isWalking) {
        cout << "\n\n ===================> TOPI detected a hole on the floor <====================\n\n";
        HoleDetectedWhenWalking();
        return;
    }

    if (collisions.empty()) {
        return;
    }
    /*
    // Get the major position correction of all collisions
    int minHorizontalCorrection = 0, maxHorizontalCorrection = 0, minVerticalCorrection = 0, maxVerticalCorrection = 0;
    for (auto collision : collisions) {
        minHorizontalCorrection = std::min(minHorizontalCorrection, collision.horizontalCorrection);
        maxHorizontalCorrection = std::max(maxHorizontalCorrection, collision.horizontalCorrection);
        minVerticalCorrection = std::min(minVerticalCorrection, collision.verticalCorrection);
        maxVerticalCorrection = std::max(maxVerticalCorrection, collision.verticalCorrection);
    }

    // Check for horizontal collision when player is walking
    if (!isJumping && !isFalling) {
        if (minHorizontalCorrection < 0) {
            // Topi collided walking to right direction
            std::cout << " ))))) COLLISION WHEN MOVING TO RIGHT <<<<<<<\n";
            PositionAddX(int16_t(minHorizontalCorrection));
            isBlockedRight = true;
            return;
        } else if (maxHorizontalCorrection > 0) {
            // Topi collided walking to left direction
            std::cout << " ))))) COLLISION WHEN MOVING TO LEFT <<<<<<<\n";
            PositionAddX(int16_t(maxHorizontalCorrection));
            isBlockedLeft = true;
            return;
        }
    }

    if (isJumping && vectorDirection.y > 0 && vectorDirection.x > 0 && minHorizontalCorrection < 0 && std::abs(minHorizontalCorrection) <= 4) {
        // Topi collided horizontally when during the ascension to the right side of a jump
        std::cout << " ))))) COLLISION ON THE RIGHT SIDE DURING JUMP ASCENSION <<<<<<<\n";
        PositionAddX(int16_t(minHorizontalCorrection));
        FallDueToLateralCollisionJump();
    } else if (isJumping && vectorDirection.y > 0 && vectorDirection.x < 0 && maxHorizontalCorrection > 0 && std::abs(maxHorizontalCorrection) <= 4) {
        // Topi collided horizontally when during the ascension to the left side of a jump
        std::cout << " ))))) COLLISION ON THE LEFT SIDE DURING JUMP ASCENSION <<<<<<<\n";
        PositionAddX(int16_t(maxHorizontalCorrection));
        FallDueToLateralCollisionJump();
    } else if (isJumping && vectorDirection.y > 0 && vectorDirection.x != 0 && maxVerticalCorrection > 0) {
        // Topi head collided with an object (during a parabolic jump)
        std::cout << " ))))) COLLIDING ON TOP DURING PARABOLIC JUMP <<<<<<<\n";
        PositionAddY(int16_t(maxVerticalCorrection));
        isJumping = false;
        isJumpApex = false;

        ObjectCollision collision = collisions.front();
        collision.object->Hit(headedToRight);

        TopCollisionDuringJump();
    } else if (isJumping && vectorDirection.x != 0 && collisions.size() >= 1 && maxHorizontalCorrection >= 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the left during a jump
        if ((std::abs(maxHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Topi collided vertically when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            // Topi collided horizontally when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE LEFT SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddX(int16_t(maxHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && vectorDirection.x != 0 && collisions.size() >= 1 && minHorizontalCorrection <= 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the right during a jump
        if ((std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Topi collided vertically when jumping to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP RIGHT SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            // Topi collided horizontally when jumping to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE RIGHT SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddX(int16_t(minHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isFalling && vectorDirection.x < 0 && collisions.size() >= 1 && maxHorizontalCorrection >= 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the left from the apex
        if ((std::abs(maxHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Topi collided vertically when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP SIDE DURING FALL TO THE LEFT <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishFall();
        } else {
            // Topi collided horizontally when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE LEFT SIDE DURING FALL TO THE LEFT <<<<<<<\n";
            PositionAddX(int16_t(maxHorizontalCorrection) + 1);
            FallDueToLateralCollisionJump();
        }
    } else if (isFalling && vectorDirection.x > 0 && collisions.size() >= 1 && minHorizontalCorrection <= 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the right from the apex
        if ((std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Topi collided vertically when jumping to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP RIGHT SIDE DURING FALL TO THE RIGHT <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishFall();
        } else {
            // Topi collided horizontally when jumping to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE RIGHT SIDE DURING FALL TO THE RIGHT <<<<<<<\n";
            PositionAddX(int16_t(minHorizontalCorrection) - 1);
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && vectorDirection.x == 0 && collisions.size() == 1 && maxHorizontalCorrection > 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision on the left side when player is falling during a 90 degree jump
        if (std::abs(maxHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) {
            std::cout << " ))))) SINGLE COLLISION ON THE TOP SIDE DURING 90 DEGREE JUMP FALLING <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            std::cout << " ))))) SINGLE COLLISION ON THE LEFT SIDE DURING 90 DEGREE JUMP FALLING <<<<<<<\n";
            PositionAddX(int16_t(maxHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && vectorDirection.x == 0 && collisions.size() == 1 && minHorizontalCorrection < 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision on the right side when player is falling during a 90 degree jump
        if (std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) {
            // Topi collided vertically when jumping looking to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP SIDE DURING 90 DEGREE JUMP FALLING <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            std::cout << " ))))) SINGLE COLLISION ON THE RIGHT SIDE DURING 90 DEGREE JUMP FALLING <<<<<<<\n";
            PositionAddX(int16_t(minHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && minVerticalCorrection < 0) {
        // Topi collided with the ground (during a jump landing)
        std::cout << " ))))) COLLIDING WITH THE GROUND DURING JUMP <<<<<<<\n";
        PositionAddY(int16_t(minVerticalCorrection));
        FinishJump();
    } else if (isJumping && maxVerticalCorrection > 0) {
        // Topi collided on his head (during a jump)
        std::cout << " ))))) COLLIDING ON TOP DURING JUMP <<<<<<<\n";
        PositionAddY(int16_t(maxVerticalCorrection));
        isJumping = false;
        isJumpApex = false;

        ObjectCollision collision = collisions.front();
        collision.object->Hit(headedToRight);

        TopCollisionDuringJump();
    } else if (isFalling && minVerticalCorrection < 0) {
        // Topi collided with the ground (during a fall)
        std::cout << " ))))) COLLIDING WITH THE GROUND DURING FALL minVerticalCorrection: " << minVerticalCorrection << "\n";
        PositionAddY(int16_t(minVerticalCorrection));
        FinishFall();
    }
    */
}

void Topi::MoveTo(Direction direction, float distance) {
    if (!isFalling && !isDazed) {
        PositionAddX(direction == Direction::RIGHT ? distance : -(distance));
        vectorDirection.x = (direction == Direction::RIGHT ? 1 : -1);
    }
}

void Topi::FallDueToSuspendedInTheAir() {
    // TODO
    /*
    // Ignore collisions with the previous underlying cloud when player falls.
    if (prevUnderlyingObject != nullptr && (std::find(objectsToIgnoreDuringFall.begin(), objectsToIgnoreDuringFall.end(), prevUnderlyingObject) == objectsToIgnoreDuringFall.end())) {
        objectsToIgnoreDuringFall.push_back(prevUnderlyingObject);
    }

    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    SuspendedInTheAir();
    */
}

bool Topi::TopiIsQuiet() {
    return (vectorDirection.x == 0) && (vectorDirection.x == vectorDirection.y);
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
    isWalking = true;
    isGoingToPickUpIce = false;
    isFalling = false;
    isDazed = false;
    direction = Direction::RIGHT;
    LoadAnimationWithId(TopiAnimation::TOPI_WALK_TO_RIGHT);
}

void Topi::STATE_Walk_Left() {
    isWalking = true;
    isGoingToPickUpIce = false;
    isFalling = false;
    isDazed = false;
    direction = Direction::LEFT;
    LoadAnimationWithId(TopiAnimation::TOPI_WALK_TO_LEFT);
}

void Topi::STATE_Run_To_Pick_Up_Ice_Right() {
    isWalking = false;
    isGoingToPickUpIce = true;
    isFalling = false;
    isDazed = false;
    direction = Direction::RIGHT;
    LoadAnimationWithId(TopiAnimation::TOPI_RUN_TO_RIGHT);
}

void Topi::STATE_Run_To_Pick_Up_Ice_Left() {
    isWalking = false;
    isGoingToPickUpIce = true;
    isFalling = false;
    isDazed = false;
    direction = Direction::LEFT;
    LoadAnimationWithId(TopiAnimation::TOPI_RUN_TO_LEFT);
}

void Topi::STATE_Fall_Dazed_Right() {
    isWalking = false;
    isGoingToPickUpIce = false;
    isFalling = true;
    isDazed = true;
    direction = Direction::LEFT;
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    LoadAnimationWithId(TopiAnimation::TOPI_FALL_DAZED_RIGHT);
}

void Topi::STATE_Fall_Dazed_Left() {
    isWalking = false;
    isGoingToPickUpIce = false;
    isFalling = true;
    isDazed = true;
    direction = Direction::RIGHT;
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    LoadAnimationWithId(TopiAnimation::TOPI_FALL_DAZED_LEFT);
}

void Topi::STATE_Run_Dazed_Right() {

}

void Topi::STATE_Run_Dazed_Left() {

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
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Left
    END_TRANSITION_MAP(nullptr)
}