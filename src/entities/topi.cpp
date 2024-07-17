#include <entities/topi.h>
#include <chrono>

Topi::Topi() :
        IEntity(EntityIdentificator::TOPI, EntityType::ENEMY, SurfaceType::SIMPLE, TopiStateIdentificator::TOPI_MAX_STATES, false, true) {
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    underlyingObjectSurfaceType = SurfaceType::SIMPLE;
    direction = (rand() % 2 == 0) ? Direction::RIGHT : Direction::LEFT; // Random initial direction.
}

uint16_t Topi::Width() {
    return currentSprite.width;
}

uint16_t Topi::Height() {
    return currentSprite.height;
}

void Topi::PrintName() {
    std::cout << "Topi." << std::endl;
}

bool Topi::IsCloud() {
  return false;
}

void Topi::DisplaceTopiIfUnderlyingSurfaceIsMobile() {
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

inline void Topi::CorrectTopiPositionOnReachScreenEdge() {
    // TODO
    /*
    if (position.GetRealX() < 0.0f) {
        PositionSetX(LEVEL_WIDTH_FLOAT - 9.0f);
    }
    else if (position.GetRealX() >= LEVEL_WIDTH_FLOAT - 8.0f) {
        PositionSetX(0.0f);
    }
    */
}

bool Topi::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;
 
    if (isWalking) {
        MoveTo(direction);
        needRedraw = true;
    } /* else {

        // Displace the player if the underlying surface is mobile
        DisplaceTopiIfUnderlyingSurfaceIsMobile();
    }
    */
    // Correct the player's position if they go beyond the screen edges
    CorrectTopiPositionOnReachScreenEdge();

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

void Topi::GetSolidCollisions(std::vector<ObjectCollision> &collisions, bool& topiIsSuspendedInTheAir) {
    // TODO
    /*
    // Check for collisions with other objects present in the scene.
    std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(GetSolidLowerBound(), GetSolidUpperBound());
    topiIsSuspendedInTheAir = true;
    IEntity* underlyingObjectCandidate = nullptr;
    prevUnderlyingCloud = currentUnderlyingCloud;
    currentUnderlyingCloud = nullptr;
    int minBottomIntersectionYUnderlyingObjectCandidate = 9999;
    int minIntersectionXDiffUnderlyingObjectCandidate = 9999;

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
        // Compute position correction when player head collides during the ascending of a 90 degree or parabolic jump
        else if ((vectorDirection.y > 0) && (intersection.topIntersectionY > 0) && (intersection.bottomIntersectionY < 0) && (intersection.rightIntersectionX != 0) && (intersection.leftIntersectionX != 0)) {
            std::cout << " [ CASE B ] intersection.topIntersectionY: " << intersection.topIntersectionY << " | intersection.bottomIntersectionY: " << intersection.bottomIntersectionY << " | intersection.rightIntersectionX: " << intersection.rightIntersectionX << " | intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            verticalCorrection = intersection.topIntersectionY; // - currentSprite.yOffset + 1;

            if ((vectorDirection.x > 0) && (intersection.rightIntersectionX < 0)) {
                horizontalCorrection = intersection.rightIntersectionX;
                std::cout << " [ CASE B-0 ] horizontalCorrection: " << horizontalCorrection << "\n";
            } else if ((vectorDirection.x < 0) && (intersection.leftIntersectionX > 0)) {
                horizontalCorrection = intersection.leftIntersectionX;
                std::cout << " [ CASE B-1 ] horizontalCorrection: " << horizontalCorrection << "\n";
            }
        }
        // Compute position correction when player collides horizontaly (on the right side of a brick) jumping to the left during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x < 0) && (intersection.leftIntersectionX >= 0) && (std::abs(intersection.rightIntersectionX) >= std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            std::cout << " [ CASE C1 ]\n";
            std::cout << " [ C1 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.leftIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the left side of a brick) jumping to the left during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x < 0) && (intersection.rightIntersectionX <= 0) && (std::abs(intersection.rightIntersectionX) < std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            std::cout << " [ CASE C2 ]\n";
            std::cout << " [ C2 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.rightIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the left side of a brick) jumping to the right during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x > 0) && (intersection.rightIntersectionX <= 0) && (std::abs(intersection.rightIntersectionX) <= std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            std::cout << " [ CASE D1 ]\n";
            std::cout << " [ D1 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.rightIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the right side of a brick) jumping to the right during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x > 0) && (intersection.leftIntersectionX >= 0) && (std::abs(intersection.rightIntersectionX) > std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            std::cout << " [ CASE D2 ]\n";
            std::cout << " [ D2 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.leftIntersectionX;
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

    std::cout << " > PLAYER COLLIDES WITH " << objectIntersections.size() << " OBJECTS. " << collisions.size() << " CORRECTIONS NEEDED.\n";

    // Check if the player is suspended in the air or get the underlying surface type
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
        }
    }

    if (underlyingObjectCandidate != nullptr) {
        topiIsSuspendedInTheAir = false;

        if (underlyingObjectCandidate->IsCloud()) {
            currentUnderlyingCloud = underlyingObjectCandidate;
        }

        underlyingObjectSurfaceType = underlyingObjectCandidate->surfaceType;
        isOnMobileSurface = (underlyingObjectSurfaceType == SurfaceType::MOBILE_RIGHT) || (underlyingObjectSurfaceType == SurfaceType::MOBILE_LEFT);

        std::cout << " ------ Underlying object: ";
        underlyingObjectCandidate->PrintName();
    }
    else {
        underlyingObjectSurfaceType = std::nullopt;
        isOnMobileSurface = false;
    }
    */
}

void Topi::UpdateCollisions() {
    // TODO
    /*
    std::vector<ObjectCollision> collisions;
    bool topiIsSuspendedInTheAir = false;

    // Search for collisions with solid objects
    this->GetSolidCollisions(collisions, topiIsSuspendedInTheAir);

    // Check if the player is floating in the air (no ground under his feet)
    if (topiIsSuspendedInTheAir && !isJumping && !isFalling && !isHitting && !isSlipping) {
        FallDueToSuspendedInTheAir();
        return;
    }

    if (collisions.empty()) {
        return;
    }

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

void Topi::MoveTo(Direction direction) {
    if (!isFalling && !isDazed) {
        PositionAddX(direction == Direction::RIGHT ? 0.5f : -0.5f);
        vectorDirection.x = (direction == Direction::RIGHT ? 1 : -1);
    }
}

bool Topi::TopiIsQuiet() {
    return (vectorDirection.x == 0) && (vectorDirection.x == vectorDirection.y);
}

void Topi::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    LoadAnimationWithId((direction == Direction::RIGHT) ? TopiAnimation::TOPI_WALK_TO_RIGHT : TopiAnimation::TOPI_WALK_TO_LEFT);

    // Set the initial state according to the direction
    ExternalEvent((direction == Direction::RIGHT) ? TopiStateIdentificator::STATE_WALK_RIGHT : TopiStateIdentificator::STATE_WALK_LEFT, nullptr);
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
    direction = Direction::RIGHT;
    LoadAnimationWithId(TopiAnimation::TOPI_WALK_TO_RIGHT);
}

void Topi::STATE_Walk_Left() {
    isWalking = true;
    direction = Direction::LEFT;
    LoadAnimationWithId(TopiAnimation::TOPI_WALK_TO_LEFT);
}
