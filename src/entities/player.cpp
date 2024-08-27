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

void Player::PrintName() const {
    std::cout << "Popo." << std::endl;
}

void Player::AdjustPlayerPositionIfUnderlyingSurfaceIsMobile() {
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

inline void Player::AdjustPlayerPositionOnScreenEdge() {
    if (position.GetRealX() < 0.0f) {
        PositionSetX(MOUNTAIN_WIDTH_FLOAT - 9.0f);
    }
    else if (position.GetRealX() >= MOUNTAIN_WIDTH_FLOAT - 8.0f) {
        PositionSetX(0.0f);
    }
}

void Player::NotifyNewAltitudeHasBeenReached() {
    if (isJumping || isFalling) {
        return;
    }

    // Level floors are configured to be present in vertical positions multiple of six.
    int currentCellY = position.GetCellY();
    if((((currentCellY % 6 == 0) && (currentCellY > BONUS_STAGE_CELL_Y)) || (currentCellY == BONUS_STAGE_CELL_Y) || (currentCellY < BONUS_STAGE_CELL_Y)) && (currentCellY < lowestCellYReached)) {
        lowestCellYReached = currentCellY;

        // Notify the player reached a new altitude and get the updated bottom viewport of the mountain.
        auto bottomViewportOpt = entityManager->PlayerReachedNewAltitude(currentCellY);
        if (bottomViewportOpt.has_value()) {
            bottomViewport = bottomViewportOpt.value();
        }

        if (currentCellY == BONUS_STAGE_CELL_Y) {
            entityManager->PlayerEnteredBonusStage();
        }
    }
}

bool Player::Update(const uint8_t pressedKeys_) {
    if (isGameOver) {
        return false;
    }

    if (gameFinished) {
        // Make a pause before notifying the game has finished
        if(chrono::system_clock::now() >= notifyGameFinishedTime) {
            entityManager->PlayerCompletedMountain(condorHunted, vegetableCount, nitpickerCount, iceCount, brickCount);
        }
        return false;
    }

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
        AdjustPlayerPositionIfUnderlyingSurfaceIsMobile();

        if (pressedKeys != KeyboardKeyCode::IC_KEY_NONE) {
            ProcessPressedKeys();
        } else if (pressedKeys != prevPressedKeys) {
            ProcessReleasedKeys();
        }
    }

    // Correct the player's position if they go beyond the screen edges
    AdjustPlayerPositionOnScreenEdge();

    // Check for collisions
    UpdateCollisions();

    // Notify the current altitude achieved (to move the camera up if a new vertical level is reached)
    NotifyNewAltitudeHasBeenReached();

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

void Player::IncreaseAchievementCounterByEntity(IEntity* entity) {
    // Increase proper achievement counters
    if (entity->type == EntityType::ENEMY) {
        switch (entity->id) {
            case EntityIdentificator::NITPICKER:
                nitpickerCount++;
                break;

            case EntityIdentificator::ICE:
                iceCount++;
                break;

            case EntityIdentificator::CONDOR:
                condorHunted = true;
                gameFinished = true;
                notifyGameFinishedTime = (chrono::system_clock::now() + std::chrono::milliseconds(PAUSE_DURATION_BEFORE_GAME_FINISHED_NOTIFICATION));
                break;
            default:
                break;
        }
    } else if (entity->type == EntityType::VEGETABLE) {
        vegetableCount++;
    } else if ((entity->type == EntityType::TERRAIN) && entity->isBreakable) {
        brickCount++;
    }
}

void Player::CheckHitCollisionsWithEnemies() {
    // Check for attack collisions with enemies present in the scene.
    if (!GetAttackLowerBound().has_value() || !GetAttackUpperBound().has_value()) {
        return;
    }

    std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(GetAttackLowerBound().value(), GetAttackUpperBound().value());

    for (auto intersection : objectIntersections) {
        if (intersection.particle->type == EntityType::ENEMY) {
            intersection.particle->Hit(isHeadedToRight);
            IncreaseAchievementCounterByEntity(intersection.particle);
        }
    }
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
        if (intersection.particle->type == EntityType::VEGETABLE) {
            intersection.particle->Hit(isHeadedToRight);
            IncreaseAchievementCounterByEntity(intersection.particle);
        } else if ((intersection.particle->type == EntityType::ENEMY) && (intersection.particle->id != EntityIdentificator::CONDOR)) {
            collisions.clear();
            playerIsSuspendedInTheAir = false;
            Killed();
            return;
        }

        if ((intersection.particle == this) || intersection.particle->isTraversable || (std::find(objectsToIgnoreDuringFall.begin(), objectsToIgnoreDuringFall.end(), intersection.particle) != objectsToIgnoreDuringFall.end())) {
            continue;
        }

        int horizontalCorrection = 0, verticalCorrection = 0;

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
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player head collides during the ascending of a 90 degree or parabolic jump
        else if ((vectorDirection.y > 0) && (intersection.topIntersectionY > 0) && (intersection.bottomIntersectionY < 0) && (intersection.rightIntersectionX != 0) && (intersection.leftIntersectionX != 0)) {
            verticalCorrection = intersection.topIntersectionY; // - currentSprite.yOffset + 1;

            if ((vectorDirection.x > 0) && (intersection.rightIntersectionX < 0)) {
                horizontalCorrection = intersection.rightIntersectionX;
            } else if ((vectorDirection.x < 0) && (intersection.leftIntersectionX > 0)) {
                horizontalCorrection = intersection.leftIntersectionX;
            }
        }
        // Compute position correction when player collides horizontaly (on the right side of a brick) jumping to the left during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x < 0) && (intersection.leftIntersectionX >= 0) && (std::abs(intersection.rightIntersectionX) >= std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            horizontalCorrection = intersection.leftIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the left side of a brick) jumping to the left during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x < 0) && (intersection.rightIntersectionX <= 0) && (std::abs(intersection.rightIntersectionX) < std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            horizontalCorrection = intersection.rightIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the left side of a brick) jumping to the right during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x > 0) && (intersection.rightIntersectionX <= 0) && (std::abs(intersection.rightIntersectionX) <= std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            horizontalCorrection = intersection.rightIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        // Compute position correction when player collides horizontaly (on the right side of a brick) jumping to the right during falling
        else if ((vectorDirection.y < 0) && (vectorDirection.x > 0) && (intersection.leftIntersectionX >= 0) && (std::abs(intersection.rightIntersectionX) > std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY <= 0)) {
            horizontalCorrection = intersection.leftIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }
        else {
            continue;
        }

        collisions.push_back({intersection.particle, horizontalCorrection, verticalCorrection});

        if (intersection.bottomIntersectionY < minBottomIntersectionYUnderlyingObjectCandidate) {
            minBottomIntersectionYUnderlyingObjectCandidate = intersection.bottomIntersectionY;
            underlyingObjectCandidate = intersection.particle;
        }
    }

    // Check if the player is suspended in the air or get the underlying surface type
    for (auto intersection : objectIntersections) {
        if ((intersection.particle == this) || intersection.particle->isTraversable) {
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

    if ((underlyingObjectCandidate != nullptr) && !underlyingObjectCandidate->isTraversable) {
        playerIsSuspendedInTheAir = false;

        if (underlyingObjectCandidate->type == EntityType::CLOUD) {
            currentUnderlyingCloud = underlyingObjectCandidate;
        }

        underlyingObjectSurfaceType = underlyingObjectCandidate->surfaceType;
        isOnMobileSurface = (underlyingObjectSurfaceType == SurfaceType::MOBILE_RIGHT) || (underlyingObjectSurfaceType == SurfaceType::MOBILE_LEFT);
    }
    else {
        underlyingObjectSurfaceType = std::nullopt;
        isOnMobileSurface = false;
    }
}

void Player::UpdateCollisions() {
    if (isDead) {
        return;
    }

    std::vector<ObjectCollision> collisions;
    bool playerIsSuspendedInTheAir = false;

    // Check for hit collisions with nearby enemies
    this->CheckHitCollisionsWithEnemies();

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
            PositionAddX(int16_t(minHorizontalCorrection));
            isBlockedRight = true;
            return;
        } else if (maxHorizontalCorrection > 0) {
            // Player collided walking to left direction
            PositionAddX(int16_t(maxHorizontalCorrection));
            isBlockedLeft = true;
            return;
        }
    }

    if (isJumping && vectorDirection.y > 0 && vectorDirection.x > 0 && minHorizontalCorrection < 0 && std::abs(minHorizontalCorrection) <= 4) {
        // Player collided horizontally when during the ascension to the right side of a jump
        PositionAddX(int16_t(minHorizontalCorrection));
        FallDueToLateralCollisionJump();
    } else if (isJumping && vectorDirection.y > 0 && vectorDirection.x < 0 && maxHorizontalCorrection > 0 && std::abs(maxHorizontalCorrection) <= 4) {
        // Player collided horizontally when during the ascension to the left side of a jump
        PositionAddX(int16_t(maxHorizontalCorrection));
        FallDueToLateralCollisionJump();
    } else if (isJumping && vectorDirection.y > 0 && vectorDirection.x != 0 && maxVerticalCorrection > 0) {
        // Player head collided with an object (during a parabolic jump)
        PositionAddY(int16_t(maxVerticalCorrection));
        isJumping = false;
        isJumpApex = false;

        ObjectCollision collision = collisions.front();
        collision.object->Hit(isHeadedToRight);
        IncreaseAchievementCounterByEntity(collision.object);
        TopCollisionDuringJump();
    } else if (isJumping && vectorDirection.x != 0 && collisions.size() >= 1 && maxHorizontalCorrection >= 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the left during a jump
        if ((std::abs(maxHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Player collided vertically when jumping to left direction
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            // Player collided horizontally when jumping to left direction
            PositionAddX(int16_t(maxHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && vectorDirection.x != 0 && collisions.size() >= 1 && minHorizontalCorrection <= 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the right during a jump
        if ((std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Player collided vertically when jumping to right direction
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            // Player collided horizontally when jumping to right direction
            PositionAddX(int16_t(minHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isFalling && vectorDirection.x < 0 && collisions.size() >= 1 && maxHorizontalCorrection >= 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the left from the apex
        if ((std::abs(maxHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Player collided vertically when jumping to left direction
            PositionAddY(int16_t(minVerticalCorrection));
            FinishFall();
        } else {
            // Player collided horizontally when jumping to left direction
            PositionAddX(int16_t(maxHorizontalCorrection) + 1);
            FallDueToLateralCollisionJump();
        }
    } else if (isFalling && vectorDirection.x > 0 && collisions.size() >= 1 && minHorizontalCorrection <= 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the right from the apex
        if ((std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) && (collisions.size() == 1)) {
            // Player collided vertically when jumping to right direction
            PositionAddY(int16_t(minVerticalCorrection));
            FinishFall();
        } else {
            // Player collided horizontally when jumping to right direction
            PositionAddX(int16_t(minHorizontalCorrection) - 1);
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && vectorDirection.x == 0 && collisions.size() == 1 && maxHorizontalCorrection > 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision on the left side when player is falling during a 90 degree jump
        if (std::abs(maxHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) {
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            PositionAddX(int16_t(maxHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && vectorDirection.x == 0 && collisions.size() == 1 && minHorizontalCorrection < 0 && maxHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision on the right side when player is falling during a 90 degree jump
        if (std::abs(minHorizontalCorrection) > MIN_PIXELS_ON_UNDERLYING_SURFACE) {
            // Player collided vertically when jumping looking to right direction
            PositionAddY(int16_t(minVerticalCorrection));
            FinishJump();
        } else {
            PositionAddX(int16_t(minHorizontalCorrection));
            FallDueToLateralCollisionJump();
        }
    } else if (isJumping && minVerticalCorrection < 0) {
        // Player collided with the ground (during a jump landing)
        PositionAddY(int16_t(minVerticalCorrection));
        FinishJump();
    } else if (isJumping && maxVerticalCorrection > 0) {
        // Player collided on his head (during a jump)
        PositionAddY(int16_t(maxVerticalCorrection));
        isJumping = false;
        isJumpApex = false;

        ObjectCollision collision = collisions.front();
        collision.object->Hit(isHeadedToRight);
        IncreaseAchievementCounterByEntity(collision.object);
        TopCollisionDuringJump();
    } else if (isFalling && minVerticalCorrection < 0) {
        // Player collided with the ground (during a fall)
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

void Player::ProcessPressedKeys(bool checkPreviousPressedKeys) {
    // User pressed IC_KEY_RIGHT
    if ((pressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) == KeyboardKeyCode::IC_KEY_RIGHT) {
        // If is not IC_KEY_RIGHT repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) != KeyboardKeyCode::IC_KEY_RIGHT))) {
            isHeadedToRight = true;
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
            isHeadedToRight = false;
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

    if (!isJumping && !isFalling && !isSlipping && ((isOnMobileSurface && !isRunning && (isBlockedRight || isBlockedLeft)) || (isRunning && !isBlockedRight && !isBlockedLeft) || (isHeadedToRight && !isBlockedRight) || (!isHeadedToRight && !isBlockedLeft)) && ((pressedKeys & KeyboardKeyCode::IC_KEY_UP) == KeyboardKeyCode::IC_KEY_UP)) {
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

    boundingBox = {spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY};
    solidBoundingBox = {spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY};
    attackBoundingBox = spriteData.hasAttack
        ? std::make_optional<Boundaries>(Boundaries{spriteData.attackLowerBoundX, spriteData.attackLowerBoundY, spriteData.attackUpperBoundX, spriteData.attackUpperBoundY})
        : std::nullopt;
    firstSpriteOfCurrentAnimationIsLoaded = true;
}

IEntity *Player::Create() {
    return new Player();
}

Player::~Player() = default;

bool Player::ShouldBeginAnimationLoopAgain() {
    if (isDead || isRespawning) {
        return true;
    } else if (currentState == PlayerStateIdentificator::STATE_HIT_RIGHT) {
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
    float vJumpPosition = vInitialJumpPosition + vOffset;
    PositionSetXY(hInitialJumpPosition + (hInitialJumpSpeed * tJump), vJumpPosition);
    UpdatePreviousDirection();
    vectorDirection.x = (hInitialJumpSpeed > 0.0f) ? 1 : (hInitialJumpSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = (previous_vOffset < vOffset) ? -1 : 1;
    isJumpApex = ((prevVectorDirection.y == 1) && (vectorDirection.y == -1));

    if (isJumpApex) {
        // User is able to fall to the right or left side when pressing RIGHT or LEFT keys.
        ProcessPressedKeys(false);
    }

    previous_vOffset = vOffset;

    if (vJumpPosition > bottomViewport) {
        isGameOver = entityManager->PlayerHasLostALife(vegetableCount, nitpickerCount, iceCount, brickCount);
        if (!isGameOver) {
            StartRespawn();
        }
    }
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
    float vFallPosition = vInitialFallPosition + vOffset;
    PositionSetY(vFallPosition);
    UpdatePreviousDirection();
    vectorDirection.x = (hInitialFallSpeed > 0.0f) ? 1 : (hInitialFallSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = -1;

    if (vFallPosition > bottomViewport) {
        bool isGameOver = entityManager->PlayerHasLostALife(vegetableCount, nitpickerCount, iceCount, brickCount);
        if (!isGameOver) {
            StartRespawn();
        }
    }
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
    PositionAddX(isHeadedToRight ? 2.0f : -2.0f);
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

void Player::RespawnWithAnimation(PlayerAnimation animation) {
    isRespawning = true;
    isDead = false;
    isRunning = false;
    isJumping = false;
    isFalling = false;
    isJumpApex = false;

    if ((respawnX < 0.0f) || (respawnY < 0.0f)) {
        position.recoverInitialPosition();
    } else {
        position.setXY(respawnX, respawnY);
    }
    LoadAnimationWithId(animation);
}

void Player::MoveTo(Direction direction) {
    if (!isJumping && !isHitting) {
        // Store current "safe" player position as respawn position in case of death.
        respawnX = position.GetRealX();
        respawnY = position.GetRealY();

        // Update the player position.
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
    entityManager->PlaySoundById(isDead ? SoundIdentificator::LOSE_LIFE_SOUND : SoundIdentificator::JUMP_SOUND);
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
    vectorDirection.x = isHeadedToRight ? 1 : -1;
    vectorDirection.y = 0;
    hMomentum = 0;
    hInitialSlipPosition = position.GetRealX();
    isJumping = false;
    isJumpApex = false;
    isFalling = false;
    isSlipping = true;
}

void Player::STATE_Idle_Right() {
    isRespawning = false;
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
    isRespawning = false;
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
    isRespawning = false;
    isRunning = true;
    UpdatePreviousDirection();
    vectorDirection.x = 1;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::RUN_TO_RIGHT);
}

void Player::STATE_Run_Left() {
    isRespawning = false;
    isRunning = true;
    UpdatePreviousDirection();
    vectorDirection.x = -1;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::RUN_TO_LEFT);
}

void Player::STATE_Jump_Idle_Right() {
    isRespawning = false;
    Jump(47.0f, 0.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_RIGHT);
}

void Player::STATE_Jump_Idle_Left() {
    isRespawning = false;
    Jump(47.0f, 0.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_LEFT);
}

void Player::STATE_Jump_Run_Right() {
    isRespawning = false;
    isRunning = false;
    // More momentum produces a longer jump
    Jump(45.0f, hMomentum == maxMomentum ? 10.0f : 4.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_RIGHT);
}

void Player::STATE_Jump_Run_Left() {
    isRespawning = false;
    isRunning = false;
    // More momentum produces a longer jump
    Jump(45.0f, hMomentum == maxMomentum ? -10.0f : -4.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_LEFT);
}

void Player::STATE_Fall_Idle_Right() {
    isRespawning = false;
    isRunning = false;
    Fall(0.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
}

void Player::STATE_Fall_Idle_Left() {
    isRespawning = false;
    isRunning = false;
    Fall(0.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
}

void Player::STATE_Fall_Run_Right() {
    isRespawning = false;
    isRunning = false;
    Fall(8.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
}

void Player::STATE_Fall_Run_Left() {
    isRespawning = false;
    isRunning = false;
    Fall(-8.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
}

void Player::STATE_Hit_Right() {
    isRespawning = false;
    isRunning = false;
    LoadAnimationWithId(PlayerAnimation::HIT_RIGHT);
}

void Player::STATE_Hit_Left() {
    isRespawning = false;
    isRunning = false;
    LoadAnimationWithId(PlayerAnimation::HIT_LEFT);
}

void Player::STATE_Slip_Right() {
    isRunning = false;
    Slip();
    LoadAnimationWithId(PlayerAnimation::SLIP_TO_RIGHT);
}

void Player::STATE_Slip_Left() {
    isRunning = false;
    Slip();
    LoadAnimationWithId(PlayerAnimation::SLIP_TO_LEFT);
}

void Player::STATE_Killed() {
    isDead = true;
    Jump(47.0f, 0.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_DEAD);
}

void Player::STATE_Respawn_Right() {
    RespawnWithAnimation(PlayerAnimation::RESPAWN_RIGHT);
}

void Player::STATE_Respawn_Left() {
    RespawnWithAnimation(PlayerAnimation::RESPAWN_LEFT);
}