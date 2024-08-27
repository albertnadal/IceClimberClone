#include <entities/ice.h>
#include <chrono>

Ice::Ice() :
        IEntity(EntityIdentificator::ICE, EntityType::ENEMY, SurfaceType::SIMPLE, IceStateIdentificator::ICE_MAX_STATES, false, true) {
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    fillHoleEntityId = std::nullopt;
}

void Ice::PrintName() const {
    std::cout << "Ice." << std::endl;
}

inline bool Ice::ReachedScreenEdge() const {
    return (position.GetRealX() < (Width() >> 1)) || (position.GetRealX() >= MOUNTAIN_WIDTH_FLOAT - Width());
}

bool Ice::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;

    // Delete the block of ice if reached screen edges
    if (firstSpriteOfCurrentAnimationIsLoaded && ReachedScreenEdge()) {
        isMarkedToDelete = true;
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

void Ice::Hit(bool hitFromLeft) {
    entityManager->PlaySoundById(SoundIdentificator::HIT_ENEMY_SOUND);
    RemoveFromSpacePartitionObjectsTree();
    isBeingPushed = false;
    isBeingDestroyed = true;
    LoadAnimationWithId(IceAnimation::ICE_DESTROY);  // The object will be marked to delete once the animation finishes
}

void Ice::GetSolidCollisions(std::vector<ObjectCollision> &collisions, bool& iceIsSuspendedInTheAir, bool& iceFoundAHoleOnTheFloor) {
    // Check for collisions with other objects present in the scene.
    std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(GetLowerBound(), GetUpperBound());
    iceIsSuspendedInTheAir = false;
    iceFoundAHoleOnTheFloor = false;
    IEntity* underlyingObjectCandidate = nullptr;
    currentUnderlyingObject = nullptr;
    int minBottomIntersectionYUnderlyingObjectCandidate = 9999;
    int minIntersectionXDiffUnderlyingObjectCandidate = 9999;
    int numPixelsUnderlyingObjectsSurface = 0;

    for (auto intersection : objectIntersections) {
        if (intersection.particle == this) {
            continue;
        }

        int horizontalCorrection = 0, verticalCorrection = 0;

        if (intersection.bottomIntersectionY < minBottomIntersectionYUnderlyingObjectCandidate) {
            minBottomIntersectionYUnderlyingObjectCandidate = intersection.bottomIntersectionY;
            underlyingObjectCandidate = intersection.particle;
        }

        // Compute position correction when Ice collides when moving to the right
        if ((intersection.particle->id == EntityIdentificator::TOPI) && (intersection.particle->vectorDirection.x < 0) && (intersection.particle->vectorDirection.y == 0) && (intersection.rightIntersectionX < 0)) {
            horizontalCorrection = intersection.rightIntersectionX;
        }
        // Compute position correction when Ice collides when moving to the left
        else if ((intersection.particle->id == EntityIdentificator::TOPI) && (intersection.particle->vectorDirection.x > 0) && (intersection.particle->vectorDirection.y == 0) && (intersection.leftIntersectionX > 0)) {
            horizontalCorrection = intersection.leftIntersectionX;
        }
        else {
            continue;
        }

        collisions.push_back({intersection.particle, horizontalCorrection, verticalCorrection});
    }

    // Check if Ice is suspended in the air
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

    // Ice is considered to be suspended in the air when there are no underlying object or the underlying object surface
    // covers less o equal the half width of the Ice.
    if ((underlyingObjectCandidate == nullptr) || (numPixelsUnderlyingObjectsSurface <= (currentSprite.width >> 1))) {
        iceIsSuspendedInTheAir = true;
    } else {
        currentUnderlyingObject = underlyingObjectCandidate;
        fillHoleEntityId = currentUnderlyingObject->id;
    }
}

void Ice::UpdateCollisions() {
    if (isBeingDestroyed) {
        return;
    }

    std::vector<ObjectCollision> collisions;
    bool iceIsSuspendedInTheAir = false;
    bool iceFoundAHoleOnTheFloor = false;

    // Search for collisions with solid objects
    this->GetSolidCollisions(collisions, iceIsSuspendedInTheAir, iceFoundAHoleOnTheFloor);

    // Change state when the ice is suspended in the air (almost no underlying surface).
    if (iceIsSuspendedInTheAir && hasBeenPushedByTopi) {
        if (fillHoleEntityId.has_value()) {
            // Try to fill the three potential underlying holes of the block of ice
            for (int i=-1; i<2; i++) {
                int cell_x = position.GetCellX() + i;

                // Do not fill holes out of the level boundaries
                if ((cell_x < 0) || (cell_x >= MOUNTAIN_WIDTH_CELLS)) {
                    continue;
                }

                int cell_y = position.GetCellY() + (Height() / CELL_HEIGHT);
                int x = cell_x * CELL_WIDTH;
                int y = cell_y * CELL_HEIGHT;
                std::vector<int> lowerBound{x + 1, y + 1};
                std::vector<int> upperBound{x + CELL_WIDTH - 1, y + CELL_HEIGHT - 2};

                std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(lowerBound, upperBound);
                bool object_collides = false;

                for (auto intersection : objectIntersections) {
                    if (!intersection.particle->isTraversable && !intersection.particle->isMarkedToDelete && (intersection.particle->type != EntityType::CLOUD) && (intersection.particle->id != EntityIdentificator::TOPI)) {
                        object_collides = true;
                        break;
                    }
                }

                if (!object_collides) {
                    entityManager->CreateEntityWithId(*fillHoleEntityId, cell_x, cell_y);
                }
            }
        }
        isMarkedToDelete = true;
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

    // Check for horizontal collisions
    if (minHorizontalCorrection < 0 || maxHorizontalCorrection > 0) {
        float correction = (minHorizontalCorrection < 0) ? static_cast<float>(minHorizontalCorrection) : static_cast<float>(maxHorizontalCorrection);
        PositionAddX(correction);
        UpdatePositionInSpacePartitionTree();
        hasBeenPushedByTopi = true;
        return;
    }
}

void Ice::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
    spriteSheet = _spriteSheet;
    LoadAnimationWithId(IceAnimation::ICE_STICKY);
}

IEntity *Ice::Create() {
    return new Ice();
}

Ice::~Ice() = default;

bool Ice::ShouldBeginAnimationLoopAgain() {
    // Delete the entity once the destroying animation finishes
    if (isBeingDestroyed) {
        isMarkedToDelete = true;
    }

    return false;
}
