#include <entities/player.h>
#include <chrono>

Player::Player() :
        IEntity(EntityIdentificator::POPO, EntityType::PLAYER, SurfaceType::SIMPLE, PlayerStateIdentificator::POPO_MAX_STATES, false, true) {
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    prevVectorDirection.x = 0;
    prevVectorDirection.y = 0;
    underlyingObjectSurfaceType = SurfaceType::SIMPLE;
}

void Player::PrintName() {
    std::cout << "Main character." << std::endl;
}

void Player::DisplacePlayerIfUnderlyingSurfaceIsMobile() {
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
}

inline void Player::CorrectPlayerPositionOnReachScreenEdge() {
    if (position.GetRealX() < 0.0f) {
        PositionSetX(LEVEL_WIDTH_FLOAT - 9.0f);
    }
    else if (position.GetRealX() >= LEVEL_WIDTH_FLOAT - 8.0f) {
        PositionSetX(0.0f);
    }
}

bool Player::Update(const uint8_t pressedKeys_) {

    bool needRedraw = false;
    pressedKeys = pressedKeys_;

    if (isJumping) {
        UpdateJump();
        needRedraw = true;
    } else if (isFalling) {
        UpdateFall();
        needRedraw = true;
    } else if (isSlipping) {
        UpdateSlip();
        needRedraw = true;
    } else {

        // Displace the player if the underlying surface is mobile
        DisplacePlayerIfUnderlyingSurfaceIsMobile();

        if (pressedKeys != KeyboardKeyCode::IC_KEY_NONE) {
            ProcessPressedKeys();
        } else if (pressedKeys != prevPressedKeys) {
            ProcessReleasedKeys();
        }

    }

    // Correct the player's position if they go beyond the screen edges
    CorrectPlayerPositionOnReachScreenEdge();

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

void Player::GetSolidCollisions(std::vector<ObjectCollision> &collisions, bool& playerIsSuspendedInTheAir) {
    // Check for collisions with other objects present in the scene.
    std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(GetSolidLowerBound(), GetSolidUpperBound());
    playerIsSuspendedInTheAir = true;
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

        //std::cout << " ==== intersection.topIntersectionY: " << intersection.topIntersectionY << "\n";

        //std::cout << " [ START ] intersection.topIntersectionY: " << intersection.topIntersectionY << " | intersection.bottomIntersectionY: " << intersection.bottomIntersectionY << " | intersection.rightIntersectionX: " << intersection.rightIntersectionX << " | intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
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
            //std::cout << " [ CASE A ] intersection.topIntersectionY: " << intersection.bottomIntersectionY + currentSprite.yOffset + 1 << "\n";
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player head collides during the ascending of a 90 degree or parabolic jump
        else if ((vectorDirection.y > 0) && (intersection.topIntersectionY > 0) && (intersection.bottomIntersectionY < 0) && (intersection.rightIntersectionX != 0) && (intersection.leftIntersectionX != 0)) {
            //std::cout << " [ CASE B ] intersection.topIntersectionY: " << intersection.topIntersectionY << " | intersection.bottomIntersectionY: " << intersection.bottomIntersectionY << " | intersection.rightIntersectionX: " << intersection.rightIntersectionX << " | intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            verticalCorrection = intersection.topIntersectionY; // - currentSprite.yOffset + 1;

            if ((vectorDirection.x > 0) && (intersection.rightIntersectionX < 0)) {
                horizontalCorrection = intersection.rightIntersectionX;
                //std::cout << " [ CASE B-0 ] horizontalCorrection: " << horizontalCorrection << "\n";
            } else if ((vectorDirection.x < 0) && (intersection.leftIntersectionX > 0)) {
                horizontalCorrection = intersection.leftIntersectionX;
                //std::cout << " [ CASE B-1 ] horizontalCorrection: " << horizontalCorrection << "\n";
            }
        }
        // Compute position correction when player collides horizontaly (on the right side of a brick) jumping to the left during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x < 0) && (intersection.leftIntersectionX >= 0) && (std::abs(intersection.rightIntersectionX) >= std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            //std::cout << " [ CASE C1 ]\n";
            //std::cout << " [ C1 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.leftIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the left side of a brick) jumping to the left during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x < 0) && (intersection.rightIntersectionX <= 0) && (std::abs(intersection.rightIntersectionX) < std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            //std::cout << " [ CASE C2 ]\n";
            //std::cout << " [ C2 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.rightIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the left side of a brick) jumping to the right during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x > 0) && (intersection.rightIntersectionX <= 0) && (std::abs(intersection.rightIntersectionX) <= std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            //std::cout << " [ CASE D1 ]\n";
            //std::cout << " [ D1 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.rightIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the right side of a brick) jumping to the right during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x > 0) && (intersection.leftIntersectionX >= 0) && (std::abs(intersection.rightIntersectionX) > std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            //std::cout << " [ CASE D2 ]\n";
            //std::cout << " [ D2 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.leftIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
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

    //std::cout << " > PLAYER COLLIDES WITH " << objectIntersections.size() << " OBJECTS. " << collisions.size() << " CORRECTIONS NEEDED.\n";

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
        playerIsSuspendedInTheAir = false;

        if (underlyingObjectCandidate->IsCloud()) {
            currentUnderlyingCloud = underlyingObjectCandidate;
        }

        underlyingObjectSurfaceType = underlyingObjectCandidate->surfaceType;
        isOnMobileSurface = (underlyingObjectSurfaceType == SurfaceType::MOBILE_RIGHT) || (underlyingObjectSurfaceType == SurfaceType::MOBILE_LEFT);

        //std::cout << " ------ Underlying object: ";
        underlyingObjectCandidate->PrintName();
    }
    else {
        underlyingObjectSurfaceType = std::nullopt;
        isOnMobileSurface = false;
    }
}

void Player::UpdateCollisions() {
    std::vector<ObjectCollision> collisions;
    bool playerIsSuspendedInTheAir = false;

    // Search for collisions with solid objects
    this->GetSolidCollisions(collisions, playerIsSuspendedInTheAir);

    // Check if the player is floating in the air (no ground under his feet)
    if (playerIsSuspendedInTheAir && !isJumping && !isFalling && !isHitting && /*!isBlockedRight && !isBlockedLeft &&*/ !isSlipping) {
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
            // Player collided walking to right direction
            std::cout << " ))))) COLLISION WHEN MOVING TO RIGHT <<<<<<<\n";
            PositionAddX(int16_t(minHorizontalCorrection));
            isBlockedRight = true;
            return;
        } else if (maxHorizontalCorrection > 0) {
            // Player collided walking to left direction
            std::cout << " ))))) COLLISION WHEN MOVING TO LEFT <<<<<<<\n";
            PositionAddX(int16_t(maxHorizontalCorrection));
            isBlockedLeft = true;
            return;
        }
    }
/*
    std::cout << " ---- minVerticalCorrection: " << minVerticalCorrection << "\n";
    std::cout << " ---- maxVerticalCorrection: " << maxVerticalCorrection << "\n";
    std::cout << " ---- minHorizontalCorrection: " << minHorizontalCorrection << "\n";
    std::cout << " ---- maxHorizontalCorrection: " << maxHorizontalCorrection << "\n";
    std::cout << " ---- std::abs(minVerticalCorrection): " << std::abs(minVerticalCorrection) << "\n";
    std::cout << " ---- std::abs(minHorizontalCorrection): " << std::abs(minHorizontalCorrection) << "\n";
    std::cout << " ---- collisions.size(): " << collisions.size() << "\n";
    std::cout << " ---- vectorDirection.y: " << vectorDirection.y << "\n";
    std::cout << " ---- vectorDirection.x: " << vectorDirection.x << "\n";
    std::cout << " ---- isBlockedRight: " << isBlockedRight << "\n";
    std::cout << " ---- isBlockedLeft: " << isBlockedLeft << "\n";
    std::cout << " ---- isJumping: " << isJumping << "\n";
    std::cout << " ---- isFalling: " << isFalling << "\n";
    std::cout << " ---- isSlipping: " << isSlipping << "\n";
*/
    if (isJumping && vectorDirection.y > 0 && vectorDirection.x > 0 && minHorizontalCorrection < 0 && std::abs(minHorizontalCorrection) <= 4) {
        // Player collided horizontally when during the ascension to the right side of a jump
        std::cout << " ))))) COLLISION ON THE RIGHT SIDE DURING JUMP ASCENSION <<<<<<<\n";
        PositionAddX(int16_t(minHorizontalCorrection));
        FallDueToLateralCollisionJump();
    } else if (isJumping && vectorDirection.y > 0 && vectorDirection.x < 0 && maxHorizontalCorrection > 0 && std::abs(maxHorizontalCorrection) <= 4) {
        // Player collided horizontally when during the ascension to the left side of a jump
        std::cout << " ))))) COLLISION ON THE LEFT SIDE DURING JUMP ASCENSION <<<<<<<\n";
        PositionAddX(int16_t(maxHorizontalCorrection));
        FallDueToLateralCollisionJump();
    } else if (isJumping && vectorDirection.y > 0 && vectorDirection.x != 0 && maxVerticalCorrection > 0) {
        // Player head collided with an object (during a parabolic jump)
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
            // Player collided vertically when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            // Player collided horizontally when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE LEFT SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddX(int16_t(maxHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && vectorDirection.x != 0 && collisions.size() >= 1 && minHorizontalCorrection <= 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the right during a jump
        if ((std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Player collided vertically when jumping to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP RIGHT SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            // Player collided horizontally when jumping to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE RIGHT SIDE DURING JUMP FALLING <<<<<<<\n";
            PositionAddX(int16_t(minHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isFalling && vectorDirection.x < 0 && collisions.size() >= 1 && maxHorizontalCorrection >= 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the left from the apex
        if ((std::abs(maxHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Player collided vertically when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP SIDE DURING FALL TO THE LEFT <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishFall();
        } else {
            // Player collided horizontally when jumping to left direction
            std::cout << " ))))) SINGLE COLLISION ON THE LEFT SIDE DURING FALL TO THE LEFT <<<<<<<\n";
            PositionAddX(int16_t(maxHorizontalCorrection) + 1);
            FallDueToLateralCollisionJump();
        }
    } else if (isFalling && vectorDirection.x > 0 && collisions.size() >= 1 && minHorizontalCorrection <= 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the right from the apex
        if ((std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Player collided vertically when jumping to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP RIGHT SIDE DURING FALL TO THE RIGHT <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishFall();
        } else {
            // Player collided horizontally when jumping to right direction
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
            // Player collided vertically when jumping looking to right direction
            std::cout << " ))))) SINGLE COLLISION ON THE TOP SIDE DURING 90 DEGREE JUMP FALLING <<<<<<<\n";
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            std::cout << " ))))) SINGLE COLLISION ON THE RIGHT SIDE DURING 90 DEGREE JUMP FALLING <<<<<<<\n";
            PositionAddX(int16_t(minHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && minVerticalCorrection < 0) {
        // Player collided with the ground (during a jump landing)
        std::cout << " ))))) COLLIDING WITH THE GROUND DURING JUMP <<<<<<<\n";
        PositionAddY(int16_t(minVerticalCorrection));
        FinishJump();
    } else if (isJumping && maxVerticalCorrection > 0) {
        // Player collided on his head (during a jump)
        std::cout << " ))))) COLLIDING ON TOP DURING JUMP <<<<<<<\n";
        PositionAddY(int16_t(maxVerticalCorrection));
        isJumping = false;
        isJumpApex = false;

        ObjectCollision collision = collisions.front();
        collision.object->Hit(headedToRight);

        TopCollisionDuringJump();
    } else if (isFalling && minVerticalCorrection < 0) {
        // Player collided with the ground (during a fall)
        std::cout << " ))))) COLLIDING WITH THE GROUND DURING FALL minVerticalCorrection: " << minVerticalCorrection << "\n";
        PositionAddY(int16_t(minVerticalCorrection));
        FinishFall();
    }
}

void Player::UpdatePreviousDirection() {
    if ((vectorDirection.x != 0) || (vectorDirection.y != 0)) {
        prevVectorDirection.x = vectorDirection.x;
        prevVectorDirection.y = vectorDirection.y;
    }
}

bool Player::PlayerIsQuiet() {
    return (vectorDirection.x == 0) && (vectorDirection.x == vectorDirection.y);
}

void Player::ProcessPressedKeys(bool checkPreviousPressedKeys) {
    // User pressed IC_KEY_RIGHT
    if ((pressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) == KeyboardKeyCode::IC_KEY_RIGHT) {
        // If is not IC_KEY_RIGHT repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) != KeyboardKeyCode::IC_KEY_RIGHT))) {
            headedToRight = true;
            isBlockedLeft = false;

            if (isJumpApex) {
                // Player fall on the right side when pressing the RIGHT key in the apex of a 90 degree jump.
                hMomentum = 0;
                RightKeyPressedAtJumpApex();
                return;
            }

            RightKeyPressed();
        }
        MoveTo(Direction::RIGHT);
    } else if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) == KeyboardKeyCode::IC_KEY_RIGHT) {
        // Go back to stand-by state
        RightKeyReleased();
    }

    if ((pressedKeys & KeyboardKeyCode::IC_KEY_LEFT) == KeyboardKeyCode::IC_KEY_LEFT) {
        // If is not IC_KEY_LEFT repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::IC_KEY_LEFT) != KeyboardKeyCode::IC_KEY_LEFT))) {
            headedToRight = false;
            isBlockedRight = false;

            if (isJumpApex) {
                // Player fall on the right side when pressing the RIGHT key in the apex of a 90 degree jump.
                hMomentum = 0;
                LeftKeyPressedAtJumpApex();
                return;
            }

            LeftKeyPressed();
        }
        MoveTo(Direction::LEFT);
    } else if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_LEFT) == KeyboardKeyCode::IC_KEY_LEFT) {
        // Go back to stand-by state
        LeftKeyReleased();
    }

    if (!isJumping && !isFalling && !isSlipping && ((isOnMobileSurface && !isRunning && (isBlockedRight || isBlockedLeft)) || (isRunning && !isBlockedRight && !isBlockedLeft) || (headedToRight && !isBlockedRight) || (!headedToRight && !isBlockedLeft)) && ((pressedKeys & KeyboardKeyCode::IC_KEY_UP) == KeyboardKeyCode::IC_KEY_UP)) {
        // If is not IC_KEY_UP repeated press then change character state
        if ((!checkPreviousPressedKeys) ||
            ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::IC_KEY_UP) != KeyboardKeyCode::IC_KEY_UP))) {
            UpKeyPressed();
        }
    }

    if (!isJumping && !isHitting && !isFalling && !isSlipping && ((pressedKeys & KeyboardKeyCode::IC_KEY_SPACE) == KeyboardKeyCode::IC_KEY_SPACE)) {
        // If is not IC_KEY_SPACE repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) &&
                                            ((prevPressedKeys & KeyboardKeyCode::IC_KEY_SPACE) !=
                                             KeyboardKeyCode::IC_KEY_SPACE))) {
            isHitting = true;
            SpaceKeyPressed();
        }
    }

    if (!isJumping && ((pressedKeys & KeyboardKeyCode::IC_KEY_DOWN) == KeyboardKeyCode::IC_KEY_DOWN)) {
        // If is not IC_KEY_DOWN repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) &&
                                            ((prevPressedKeys & KeyboardKeyCode::IC_KEY_DOWN) !=
                                             KeyboardKeyCode::IC_KEY_DOWN))) {
            DownKeyPressed();
        }
    }

    prevPressedKeys = pressedKeys;
}

void Player::ProcessReleasedKeys() {
    if (((prevPressedKeys & (KeyboardKeyCode::IC_KEY_RIGHT | KeyboardKeyCode::IC_KEY_LEFT)) != 0) && (hMomentum == maxMomentum) && (underlyingObjectSurfaceType.has_value() && *underlyingObjectSurfaceType == SurfaceType::SLIDING)) {
        // User stopped running to the RIGHT or LEFT on a sliding surface
        StopRunningOnSlidingSurface();
    }
    else if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) == KeyboardKeyCode::IC_KEY_RIGHT) {
        // Go back to stand-by state
        RightKeyReleased();
    }
    else if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_LEFT) == KeyboardKeyCode::IC_KEY_LEFT) {
        // Go back to stand-by state
        LeftKeyReleased();
    }

    prevPressedKeys = KeyboardKeyCode::IC_KEY_NONE;
}

void Player::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    LoadAnimationWithId(PlayerAnimation::STAND_BY_RIGHT);
}

void Player::LoadNextSprite() {
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

IEntity *Player::Create() {
    return new Player();
}

Player::~Player() = default;

bool Player::ShouldBeginAnimationLoopAgain() {
    if (currentState == PlayerStateIdentificator::STATE_HIT_RIGHT) {
        isHitting = false;
        ExternalEvent(PlayerStateIdentificator::STATE_IDLE_RIGHT, nullptr);
        return true;
    } else if (currentState == PlayerStateIdentificator::STATE_HIT_LEFT) {
        isHitting = false;
        ExternalEvent(PlayerStateIdentificator::STATE_IDLE_LEFT, nullptr);
        return true;
    } else {
        return false;
    }
}

void Player::UpdateJump() {
    tJump += 0.2f;

    // Equation of vertical and horizontal displacement of a parabolic jump.
    float vOffset = -(vInitialJumpSpeed * tJump - (0.5f) * GRAVITY * tJump * tJump);

    // Update vertical jump position
    PositionSetXY(hInitialJumpPosition + (hInitialJumpSpeed * tJump), vInitialJumpPosition + vOffset);
    UpdatePreviousDirection();
    vectorDirection.x = (hInitialJumpSpeed > 0.0f) ? 1 : (hInitialJumpSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = (previous_vOffset < vOffset) ? -1 : 1;
    isJumpApex = ((prevVectorDirection.y == 1) && (vectorDirection.y == -1));

    if (isJumpApex) {
        // User is able to fall to the right or left side when pressing RIGHT or LEFT keys.
        ProcessPressedKeys(false);
    }

    previous_vOffset = vOffset;
}

void Player::FinishJump() {
    isJumping = false;
    isJumpApex = false;
    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    JumpLanding();
}

void Player::FallDueToLateralCollisionJump() {
    isJumping = false;
    isJumpApex = false;
    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    LateralCollisionDuringJump();
}

void Player::FallDueToSuspendedInTheAir() {
    // Ignore collisions with the previous underlying cloud when player falls.
    if (prevUnderlyingCloud != nullptr && (std::find(objectsToIgnoreDuringFall.begin(), objectsToIgnoreDuringFall.end(), prevUnderlyingCloud) == objectsToIgnoreDuringFall.end())) {
        objectsToIgnoreDuringFall.push_back(prevUnderlyingCloud);
    }

    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    SuspendedInTheAir();
}

void Player::UpdateFall() {
    tFall += 0.2f;
    PositionSetX(hInitialFallPosition + (hInitialFallSpeed * tFall));

    // Equation of vertical displacement in free fall.
    float vOffset = (0.5f) * GRAVITY * tFall * tFall;
    PositionSetY(vInitialFallPosition + vOffset);
    UpdatePreviousDirection();
    vectorDirection.x = (hInitialFallSpeed > 0.0f) ? 1 : (hInitialFallSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = -1;
}

void Player::FinishFall() {
    isFalling = false;
    objectsToIgnoreDuringFall.clear();
    UpdatePreviousDirection();
    previous_vOffset = 0.0f;
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    FallLanding();
}

void Player::UpdateSlip() {
    PositionAddX(headedToRight ? 2.0f : -2.0f);
    UpdatePreviousDirection();

    if ((std::abs(hInitialSlipPosition - position.GetRealX()) >= SLIPPING_DISTANCE) || isBlockedLeft || isBlockedRight) {
        FinishSlip();
    }
}

void Player::FinishSlip() {
    isSlipping = false;
    UpdatePreviousDirection();
    previous_vOffset = 0.0f;
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    StopSlipping();
}

void Player::MoveTo(Direction direction) {
    if (!isJumping && !isHitting) {
        PositionAddX(direction == Direction::RIGHT ? 4.0f : -4.0f);
        vectorDirection.x = (direction == Direction::RIGHT ? 1 : -1);
        if (hMomentum < maxMomentum) {
            hMomentum++;
        }
    }
}

void Player::Jump(float vSpeed, float hSpeed) {
    UpdatePreviousDirection();
    vectorDirection.x = (hSpeed > 0.0f) ? 1 : (hSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = (vSpeed > 0.0f) ? 1 : (vSpeed < 0.0f) ? -1 : 0;
    vInitialJumpSpeed = vSpeed;
    hInitialJumpSpeed = hSpeed;
    vInitialJumpPosition = position.GetRealY();
    hInitialJumpPosition = position.GetRealX();
    tJump = 0.0f;
    isJumping = true;
    isJumpApex = false;
    isFalling = false;
    isSlipping = false;
}

void Player::Fall(float hSpeed) {
    UpdatePreviousDirection();
    vectorDirection.x = (hSpeed > 0.0f) ? 1 : (hSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = -1;
    vInitialFallSpeed = 0.0f;
    hInitialFallSpeed = hSpeed;
    vInitialFallPosition = position.GetRealY();
    hInitialFallPosition = position.GetRealX();
    tFall = 0.0f;
    isJumping = false;
    isJumpApex = false;
    isFalling = true;
    isSlipping = false;
}

void Player::Slip() {
    UpdatePreviousDirection();
    vectorDirection.x = headedToRight ? 1 : -1;
    vectorDirection.y = 0;
    hMomentum = 0;
    hInitialSlipPosition = position.GetRealX();
    isJumping = false;
    isJumpApex = false;
    isFalling = false;
    isSlipping = true;
}

void Player::STATE_Idle_Right() {
    isRunning = false;
    isBlockedRight = false;
    isBlockedLeft = false;
    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::STAND_BY_RIGHT);
    ProcessPressedKeys(false);
}

void Player::STATE_Idle_Left() {
    isRunning = false;
    isBlockedRight = false;
    isBlockedLeft = false;
    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::STAND_BY_LEFT);
    ProcessPressedKeys(false);
}

void Player::STATE_Run_Right() {
    isRunning = true;
    UpdatePreviousDirection();
    vectorDirection.x = 1;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::RUN_TO_RIGHT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Run_Left() {
    isRunning = true;
    UpdatePreviousDirection();
    vectorDirection.x = -1;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::RUN_TO_LEFT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Jump_Idle_Right() {
    cout << ">>>>> STATE_Jump_Idle_Right\n";
    Jump(47.0f, 0.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_RIGHT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Jump_Idle_Left() {
    cout << ">>>>> STATE_Jump_Idle_Left\n";
    Jump(47.0f, 0.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_LEFT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Jump_Run_Right() {
    cout << ">>>>> STATE_Jump_Run_Right\n";
    isRunning = false;
    // More momentum produces a longer jump
    Jump(45.0f, hMomentum == maxMomentum ? 10.0f : 4.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_RIGHT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Jump_Run_Left() {
    cout << ">>>>> STATE_Jump_Run_Left\n";
    isRunning = false;
    // More momentum produces a longer jump
    Jump(45.0f, hMomentum == maxMomentum ? -10.0f : -4.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_LEFT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Fall_Idle_Right() {
    cout << ">>>>> STATE_Fall_Idle_Right\n";
    isRunning = false;
    Fall(0.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
}

void Player::STATE_Fall_Idle_Left() {
    cout << ">>>>> STATE_Fall_Idle_Left\n";
    isRunning = false;
    Fall(0.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
}

void Player::STATE_Fall_Run_Right() {
    cout << ">>>>> STATE_Fall_Run_Right\n";
    isRunning = false;
    Fall(8.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
}

void Player::STATE_Fall_Run_Left() {
    cout << ">>>>> STATE_Fall_Run_Left\n";
    isRunning = false;
    Fall(-8.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
}

void Player::STATE_Fall_Jump_Run_Right() {
    cout << ">>>>> STATE_Fall_Jump_Run_Right\n";
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Fall_Jump_Run_Left() {
    cout << ">>>>> STATE_Fall_Jump_Run_Left\n";
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Hit_Right() {
    isRunning = false;
    LoadAnimationWithId(PlayerAnimation::HIT_RIGHT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Hit_Left() {
    isRunning = false;
    LoadAnimationWithId(PlayerAnimation::HIT_LEFT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Slip_Right() {
    isRunning = false;
    Slip();
    LoadAnimationWithId(PlayerAnimation::SLIP_TO_RIGHT);
    //ProcessPressedKeys(false);
}

void Player::STATE_Slip_Left() {
    isRunning = false;
    Slip();
    LoadAnimationWithId(PlayerAnimation::SLIP_TO_LEFT);
    //ProcessPressedKeys(false);
}
