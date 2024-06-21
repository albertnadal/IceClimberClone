#include "objects/player.h"
#include <chrono>

Player::Player() :
        ISceneObject(SceneObjectIdentificator::MAIN_CHARACTER, SceneObjectType::PLAYER,
                     PlayerStateIdentificator::MAIN_CHARACTER_MAX_STATES) {
    //Initially the object is quiet
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    prevVectorDirection.x = 0;
    prevVectorDirection.y = 0;
}

uint16_t Player::Width() {
    return currentSprite.width;
}

uint16_t Player::Height() {
    return currentSprite.height;
}

void Player::PrintName() {
    std::cout << "Main character." << std::endl;
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
    } else {

        if (pressedKeys != KeyboardKeyCode::IC_KEY_NONE) {
            ProcessPressedKeys();
        } else if (pressedKeys != prevPressedKeys) {
            ProcessReleasedKeys();
        }

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

// Search for solid collisions with objects
void Player::GetSolidCollisions(std::vector<ObjectCollision> &collisions) {
    // Check for collisions with other objects in the scene
    std::vector<aabb::AABBIntersection<ISceneObject*>> objectIntersections = spacePartitionObjectsTree->query(GetSolidLowerBound(), GetSolidUpperBound());

    for (auto intersection : objectIntersections) {
        if (intersection.particle == this) {
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
        // Compute position correction when player collides horizontaly jumping to the right during the ascent
        /*else if ((vectorDirection.y > 0) && (vectorDirection.x > 0) && (intersection.rightIntersectionX < 0) && (std::abs(intersection.rightIntersectionX) <= std::abs(intersection.leftIntersectionX)) && (intersection.bottomIntersectionY < 0)) {
            std::cout << " [ CASE E1 ]\n";
            std::cout << " [ E1 ] &&&&&& intersection.rightIntersectionX: " << intersection.rightIntersectionX << " intersection.leftIntersectionX: " << intersection.leftIntersectionX << "\n";
            horizontalCorrection = intersection.rightIntersectionX;
            verticalCorrection = intersection.bottomIntersectionY + currentSprite.yOffset + 1;
        }*/
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
    }

    std::cout << " > PLAYER COLLIDES WITH " << objectIntersections.size() << " OBJECTS. " << collisions.size() << " CORRECTIONS NEEDED.\n";
        /*
        // Iterate all potential collision candidates and check for real collisions
        for (uint16_t i = 0; i < intersectionsCount; i++) {
            ISceneObject *collisionCandidateObject = potentialCollisionCandidatesObjects[i];
            if (collisionCandidateObject != this) {

                // Check precise collision of every solid area of the collision candidate object with every solid area of the main character
                std::vector<Area> &collisionCandidateObjectSolidAreas = collisionCandidateObject->GetSolidAreas();
                for (auto & collisionCandidateObjectSolidArea : collisionCandidateObjectSolidAreas) {
                    collision::Rectangle candidateSolidAreaRectangle = collisionCandidateObjectSolidArea.rectangle;

                    // Check collision with all main character solid areas
                    std::vector<Area> &mainCharacterSolidAreas = GetSolidAreas();

                    collision::Penetration penetration;
                    for (auto & mainCharacterSolidArea : mainCharacterSolidAreas) {
                        collision::Rectangle mainCharacterSolidAreaRectangle = mainCharacterSolidArea.rectangle;
                        bool collision = collisionDetector.checkCollision(mainCharacterSolidAreaRectangle,
                                                                          candidateSolidAreaRectangle, penetration,
                                                                          PlayerIsQuiet() ? prevVectorDirection
                                                                                          : vectorDirection);

                        if (collision) {
                            //mainCharacterSolidAreaRectangle.Print();
                            //candidateSolidAreaRectangle.Print();
                            collidingSolidObjects.push_back({collisionCandidateObject, penetration.depth.x, penetration.depth.y,&vectorDirection});
                        }
                    }
                }
            }
        }*/
}

void Player::MoveToPositionOfNoCollision(std::vector<ObjectCollisionData> &collidingSolidObjectsData) {
    /*
    if (currentSprite.areas == nullptr) return;

    std::vector<collision::Rectangle> targetRectangles;
    for(auto & solidObjectData : collidingSolidObjectsData) {
        for (auto & solidArea : solidObjectData.object->GetSolidAreas()) {
            targetRectangles.push_back(solidArea.rectangle);
        }
    }

    std::vector<collision::Rectangle> movingRectangles;
    for (auto & solidArea : GetSolidAreas()) {
        movingRectangles.push_back(solidArea.rectangle);
    }

    collisionDetector.updateWithNonCollidingPosition(targetRectangles, movingRectangles, position,
                                                     PlayerIsQuiet() ? prevVectorDirection : vectorDirection);
    */
}

void Player::UpdateCollisions() {
    //std::cout << "TRAJECTORY TANGENT: " << position.getTrajectoryTangent() << "\n";

    std::vector<ObjectCollision> collisions;

    // Search for collisions with solid objects
    this->GetSolidCollisions(collisions);

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
        } else if (maxHorizontalCorrection > 0) {
            // Player collided walking to left direction
            PositionAddX(int16_t(maxHorizontalCorrection));
            isBlockedLeft = true;
        }
    }

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
        // Player collided on his head (during a parabolic jump)
        std::cout << " ))))) COLLIDING ON TOP DURING PARABOLIC JUMP <<<<<<<\n";
        PositionAddY(int16_t(maxVerticalCorrection));
        isJumping = false;
        TopCollisionDuringJump();
    } else if (isJumping && vectorDirection.x != 0 && collisions.size() >= 1 && maxHorizontalCorrection >= 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision when player is falling to the left during a jump
        if (std::abs(maxHorizontalCorrection) > 8) {
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
        if (std::abs(minHorizontalCorrection) > 8) {
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
    } else if (isJumping && vectorDirection.x == 0 && collisions.size() == 1 && maxHorizontalCorrection > 0 && minHorizontalCorrection == 0 && minVerticalCorrection < 0) {
        // Check for single brick collision on the left side when player is falling during a 90 degree jump
        if (std::abs(maxHorizontalCorrection) > 8) {
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
        if (std::abs(minHorizontalCorrection) > 8) {
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
        TopCollisionDuringJump();
    } else if (isFalling && minVerticalCorrection < 0) {
        // Player collided with the ground (during a fall)
        std::cout << " ))))) COLLIDING WITH THE GROUND DURING FALL minVerticalCorrection: " << minVerticalCorrection << "\n";
        PositionAddY(int16_t(minVerticalCorrection));
        FinishFall();
    }
/*
            // Colliding during jump causes finish jump and fall
            if (isJumping) {
                // Check if collision has been detected on top of the main character during jumping
                if (maxVerticalPenetrationDepth > 0) {
                    // Causes main character fall down
                    TopCollisionDuringJump();
                } else {
                    // Causes main character lay on the ground
                    FinishJump();
                }
            } else if (isFalling) {
                // If collision is produced during fall then finish the fall;
                FinishFall();
            }
*/


    /*
    // Process colliding objects according to the current scenario
    uint16_t collidingSolidObjectsCount = collidingSolidObjects.size();
    if (collidingSolidObjectsCount) {

        if ((vectorDirection.x == 0) || (vectorDirection.y == 0)) {
            std::cout << " >>>>>> VERTICAL or HORIZONTAL COLLISION (vectorDirection.x:" << vectorDirection.x << ") (vectorDirection.y:" << vectorDirection.y << ")" << "\n";
            // Collision when object displacement is vertical or horizontal. Non diagonal displacement.
            int16_t maxHorizontalPenetrationDepth = 0;
            int16_t maxVerticalPenetrationDepth = 0;

            for (uint16_t i = 0; i < collidingSolidObjectsCount; i++) {
                ObjectCollisionData objectCollisionData = collidingSolidObjects.at(i);
                if (std::abs(objectCollisionData.penetration_y) > std::abs(maxVerticalPenetrationDepth)) {
                    maxVerticalPenetrationDepth = objectCollisionData.penetration_y;
                }

                if (std::abs(objectCollisionData.penetration_x) > std::abs(maxHorizontalPenetrationDepth)) {
                    maxHorizontalPenetrationDepth = objectCollisionData.penetration_x;
                }
            }

            // Update the array of pillar objects (objects who are currently sustaining the main character from the bottom side)
            pillarObjects.clear();
            for (uint16_t i = 0; i < collidingSolidObjectsCount; i++) {
                ObjectCollisionData objectCollisionData = collidingSolidObjects.at(i);
                if (objectCollisionData.penetration_y == maxVerticalPenetrationDepth) {
                    pillarObjects.push_back(objectCollisionData.object);
                }
            }

            PositionAddX(int16_t(-maxHorizontalPenetrationDepth));
            PositionAddY(int16_t(-maxVerticalPenetrationDepth));

            // Colliding during jump causes finish jump and fall
            if (isJumping) {
                // Check if collision has been detected on top of the main character during jumping
                if (maxVerticalPenetrationDepth > 0) {
                    // Causes main character fall down
                    TopCollisionDuringJump();
                } else {
                    // Causes main character lay on the ground
                    FinishJump();
                }
            } else if (isFalling) {
                // If collision is produced during fall then finish the fall;
                FinishFall();
            }

        } else {
            std::cout << " >>>>>> DIAGONAL COLLISION\n";
            // 
            this->MoveToPositionOfNoCollision(collidingSolidObjects);

            if (isJumping) { FinishJump(); }
        }

        //ProcessPressedKeys(false);
    }*/
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
            //transit<PlayerRunToRightState>();
            //stateMachine.dispatch(KeyRightPressedEvent());
//                        LoadAnimationWithId(PlayerAnimation::RUN_TO_RIGHT);
            headedToRight = true;
            isBlockedLeft = false;
            RightKeyPressed();
        }
        MoveTo(PlayerDirection::RIGHT);
    } else if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) == KeyboardKeyCode::IC_KEY_RIGHT) {
        RightKeyReleased();
    }

    if ((pressedKeys & KeyboardKeyCode::IC_KEY_LEFT) == KeyboardKeyCode::IC_KEY_LEFT) {
        // If is not IC_KEY_LEFT repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) &&
                                            ((prevPressedKeys & KeyboardKeyCode::IC_KEY_LEFT) !=
                                             KeyboardKeyCode::IC_KEY_LEFT))) {
            //cout << "KEY LEFT PRESSED" << endl;
//                        LoadAnimationWithId(PlayerAnimation::RUN_TO_LEFT);
            headedToRight = false;
            isBlockedRight = false;
            LeftKeyPressed();
        }
        MoveTo(PlayerDirection::LEFT);
    } else if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_LEFT) == KeyboardKeyCode::IC_KEY_LEFT) {
        LeftKeyReleased();
    }

    if (!isJumping && !isFalling && ((headedToRight && !isBlockedRight) || (!headedToRight && !isBlockedLeft)) && ((pressedKeys & KeyboardKeyCode::IC_KEY_UP) == KeyboardKeyCode::IC_KEY_UP)) {
        // If is not IC_KEY_LEFT repeated press then change character state
        if ((!checkPreviousPressedKeys) ||
            ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::IC_KEY_UP) != KeyboardKeyCode::IC_KEY_UP))) {
            UpKeyPressed();
        }
    }

    if (!isJumping && !isHitting && ((pressedKeys & KeyboardKeyCode::IC_KEY_SPACE) == KeyboardKeyCode::IC_KEY_SPACE)) {
        // If is not IC_KEY_LEFT repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) &&
                                            ((prevPressedKeys & KeyboardKeyCode::IC_KEY_SPACE) !=
                                             KeyboardKeyCode::IC_KEY_SPACE))) {
            //cout << "KEY SPACE PRESSED" << endl;
            isHitting = true;
            SpaceKeyPressed();
        }
    }

    if (!isJumping && ((pressedKeys & KeyboardKeyCode::IC_KEY_DOWN) == KeyboardKeyCode::IC_KEY_DOWN)) {
        // If is not IC_KEY_LEFT repeated press then change character state
        if ((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) &&
                                            ((prevPressedKeys & KeyboardKeyCode::IC_KEY_DOWN) !=
                                             KeyboardKeyCode::IC_KEY_DOWN))) {
            //cout << "KEY UP PRESSED" << endl;
            DownKeyPressed();
        }
    }

    prevPressedKeys = pressedKeys;
}

void Player::ProcessReleasedKeys() {
    if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_RIGHT) == KeyboardKeyCode::IC_KEY_RIGHT) {
        RightKeyReleased();
    }

    if ((prevPressedKeys & KeyboardKeyCode::IC_KEY_LEFT) == KeyboardKeyCode::IC_KEY_LEFT) {
        LeftKeyReleased();
    }

    // character goes quiet headed in the proper direction
//  LoadAnimationWithId(headedToRight ? PlayerAnimation::STAND_BY_RIGHT : PlayerAnimation::STAND_BY_LEFT);

    prevPressedKeys = KeyboardKeyCode::IC_KEY_NONE;
}

void Player::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    LoadAnimationWithId(PlayerAnimation::STAND_BY_RIGHT);
}

void Player::LoadAnimationWithId(uint16_t animationId) {
    ObjectSpriteSheetAnimation *currentAnimation = spriteSheet->GetAnimationWithId(animationId);
    currentAnimationSprites = currentAnimation->GetSprites();
    animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
    currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
    animationLoaded = true;
    firstSpriteOfCurrentAnimationIsLoaded = false;
    nextSpriteTime = std::chrono::system_clock::now();
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
    //currentSprite.areas = spriteData.areas; DEPRECATED

    // Adjusts objectposition according to the sprite offset
    PositionSetOffset(spriteData.xOffset, spriteData.yOffset);

    recalculateAreasDataIsNeeded = true; // Is necessary because the current sprite may have different areas
    boundingBox = {spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY};
    solidBoundingBox = {spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY};
    firstSpriteOfCurrentAnimationIsLoaded = true;
}

SpriteData Player::NextSpriteData() {
    if (currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        (*currentAnimationSpriteIterator).beginNewLoop = true;
    }

    return *currentAnimationSpriteIterator++;
}

ISceneObject *Player::Create() {
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
    //PositionSetX(hInitialJumpPosition + (hInitialJumpSpeed * tJump));
    float vOffset = -(vInitialJumpSpeed * tJump - (0.5f) * gravity * tJump * tJump);

    /*if(vOffset <= 0.0f) {
      // Last position of trajectory
      PositionSetY(vInitialJumpPosition);
      // Finish jump
      FinishJump();
    } else {*/
    // Update vertical jumpt position
    //PositionSetY(vInitialJumpPosition + vOffset);
    PositionSetXY(hInitialJumpPosition + (hInitialJumpSpeed * tJump), vInitialJumpPosition + vOffset);
    /*}*/
    UpdatePreviousDirection();
    vectorDirection.x = (hInitialJumpSpeed > 0.0f) ? 1 : (hInitialJumpSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = (previous_vOffset < vOffset) ? -1 : 1;
    std::cout << "                          ==================> X: " << vectorDirection.x << " Y: " << vectorDirection.y << "\n";
    previous_vOffset = vOffset;
}

void Player::FinishJump() {
    isJumping = false;
    isLeaningOnTheGround = true;
    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    JumpLanding();
}

void Player::FallDueToLateralCollisionJump() {
    isJumping = false;
    isLeaningOnTheGround = false;
    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = -1;
    LateralCollisionDuringJump();
}

void Player::UpdateFall() {
    tFall += 0.2f;
    PositionSetX(hInitialFallPosition + (hInitialFallSpeed * tFall));
    float vOffset = (0.5f) * gravity * tFall * tFall;
    /*if(vOffset <= 0.0f) {
      // Jump landing
      position.setY(vInitialJumpPosition);
      isJumping = false;
      isLeaningOnTheGround = true;
      hMomentum = 0;
      JumpLanding();
    } else {*/
    PositionSetY(vInitialFallPosition + vOffset);
    /*}*/
    UpdatePreviousDirection();
    vectorDirection.x = (hInitialFallSpeed > 0.0f) ? 1 : (hInitialFallSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = -1;
}

void Player::FinishFall() {
    isFalling = false;
    isLeaningOnTheGround = true;
    UpdatePreviousDirection();
    previous_vOffset = 0.0f;
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    FallLanding();
}

void Player::MoveTo(PlayerDirection direction) {
    if (!isJumping && !isHitting) {
        PositionAddX(direction == PlayerDirection::RIGHT ? 4.0f : -4.0f);
        if (hMomentum < maxMomentum) {
            hMomentum++;
        }
    }
}

void Player::Jump(float vSpeed, float hSpeed) {
    cout << "Player::Jump" << endl;
    UpdatePreviousDirection();
    vectorDirection.x = (hSpeed > 0.0f) ? 1 : (hSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = (vSpeed > 0.0f) ? 1 : (vSpeed < 0.0f) ? -1 : 0;
    vInitialJumpSpeed = vSpeed;
    hInitialJumpSpeed = hSpeed;
    vInitialJumpPosition = position.GetRealY();
    hInitialJumpPosition = position.GetRealX();
    cout << "Initial JUMP X: " << hInitialJumpPosition << " Initial JUMP Y: " << vInitialJumpPosition << endl;
    tJump = 0.0f;
    isJumping = true;
    isFalling = false;
    isLeaningOnTheGround = false;
}

void Player::Fall(float hSpeed) {
    cout << " >>>>>>>>>>>>>>>>>>>>>> Player::Fall" << endl;
    UpdatePreviousDirection();
    vectorDirection.x = (hSpeed > 0.0f) ? 1 : (hSpeed < 0.0f) ? -1 : 0;
    vectorDirection.y = -1;
    vInitialFallSpeed = 0.0f;
    hInitialFallSpeed = hSpeed;
    vInitialFallPosition = position.GetRealY();
    hInitialFallPosition = position.GetRealX();
    cout << "Initial FALL X: " << hInitialFallPosition << " Initial FALL Y: " << vInitialFallPosition << endl;
    tFall = 0.0f;
    isJumping = false;
    isFalling = true;
    isLeaningOnTheGround = false;
}

void Player::RightKeyPressed() {
    cout << "Player::RightKeyPressed()" << endl;
    BEGIN_TRANSITION_MAP                    // - Current State -
            TRANSITION_MAP_ENTRY (STATE_RUN_RIGHT) // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_RIGHT)  // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::RightKeyReleased() {
    cout << "Player::RightKeyReleased()" << endl;
    BEGIN_TRANSITION_MAP                    // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED) // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::LeftKeyPressed() {
    cout << "Player::LeftKeyPressed()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)                 // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::LeftKeyReleased() {
    cout << "Player::LeftKeyReleased()" << endl;
    BEGIN_TRANSITION_MAP                    // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED) // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::UpKeyPressed() {
    cout << "Player::UpKeyPressed()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_RIGHT)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_LEFT)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_JUMP_RUN_RIGHT)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_JUMP_RUN_LEFT)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::DownKeyPressed() {
    cout << "Player::DownKeyPressed()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_RIGHT)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_LEFT)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::TopCollisionDuringJump() {
    cout << "Player::TopCollisionDuringJump()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_RIGHT)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_LEFT)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::LateralCollisionDuringJump() {
    cout << "Player::LateralCollisionDuringJump()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::SpaceKeyPressed() {
    cout << "Player::SpaceKeyPressed()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_HIT_RIGHT)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_HIT_LEFT)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_HIT_RIGHT)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_HIT_LEFT)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::JumpLanding() {
    cout << "Player::JumpLanding()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::FallLanding() {
    cout << "Player::FallLanding()" << endl;
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                 // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::STATE_Idle_Right() {
    cout << "Player::STATE_Idle_Right" << endl;
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
    cout << "Player::STATE_Idle_Left" << endl;
    isBlockedRight = false;
    isBlockedLeft = false;
    hMomentum = 0;
    UpdatePreviousDirection();
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::STAND_BY_LEFT);
    cout << "ProcessPressedKeys(false)" << endl;
    ProcessPressedKeys(false);
}

void Player::STATE_Run_Right() {
    cout << "Player::STATE_Run_Right" << endl;
    UpdatePreviousDirection();
    vectorDirection.x = 1;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::RUN_TO_RIGHT);
    ProcessPressedKeys(false);
}

void Player::STATE_Run_Left() {
    cout << "Player::STATE_Run_Left" << endl;
    UpdatePreviousDirection();
    vectorDirection.x = -1;
    vectorDirection.y = 0;
    LoadAnimationWithId(PlayerAnimation::RUN_TO_LEFT);
    ProcessPressedKeys(false);
}

void Player::STATE_Jump_Idle_Right() {
    Jump(45.0f, 0.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_RIGHT);
    ProcessPressedKeys(false);
}

void Player::STATE_Jump_Idle_Left() {
    Jump(45.0f, 0.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_LEFT);
    ProcessPressedKeys(false);
}

void Player::STATE_Jump_Run_Right() {
    // More momentum produces a longer jump
    Jump(45.0f, hMomentum == maxMomentum ? 10.0f : 4.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_RIGHT);
    ProcessPressedKeys(false);
}

void Player::STATE_Jump_Run_Left() {
    // More momentum produces a longer jump
    Jump(45.0f, hMomentum == maxMomentum ? -10.0f : -4.0f);
    LoadAnimationWithId(PlayerAnimation::JUMP_LEFT);
    ProcessPressedKeys(false);
}

void Player::STATE_Fall_Idle_Right() {
    cout << "Player::STATE_Fall_Idle_Right" << endl;
    Fall(0.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
}

void Player::STATE_Fall_Idle_Left() {
    cout << "Player::STATE_Fall_Idle_Left" << endl;
    Fall(0.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
}

void Player::STATE_Fall_Run_Right() {
    cout << "Player::STATE_Fall_Run_Right" << endl;
    Fall(hMomentum == maxMomentum ? 10.0f : 4.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
    ProcessPressedKeys(false);
}

void Player::STATE_Fall_Run_Left() {
    cout << "Player::STATE_Fall_Run_Left" << endl;
    Fall(hMomentum == maxMomentum ? -10.0f : -4.0f);
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
    ProcessPressedKeys(false);
}

void Player::STATE_Fall_Jump_Run_Right() {
    cout << "Player::STATE_Fall_Jump_Run_Right" << endl;
    LoadAnimationWithId(PlayerAnimation::FALL_RIGHT);
    ProcessPressedKeys(false);
}

void Player::STATE_Fall_Jump_Run_Left() {
    cout << "Player::STATE_Fall_Jump_Run_Left" << endl;
    LoadAnimationWithId(PlayerAnimation::FALL_LEFT);
    ProcessPressedKeys(false);
}

void Player::STATE_Hit_Right() {
    cout << "Player::STATE_Hit_Right" << endl;
    LoadAnimationWithId(PlayerAnimation::HIT_RIGHT);
    ProcessPressedKeys(false);
}

void Player::STATE_Hit_Left() {
    cout << "Player::STATE_Hit_Left" << endl;
    LoadAnimationWithId(PlayerAnimation::HIT_LEFT);
    ProcessPressedKeys(false);
}
