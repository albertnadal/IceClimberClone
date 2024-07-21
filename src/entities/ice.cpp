#include <entities/ice.h>
#include <chrono>

Ice::Ice() :
        IEntity(EntityIdentificator::ICE, EntityType::ENEMY, SurfaceType::SIMPLE, IceStateIdentificator::ICE_MAX_STATES, false, true) {
    vectorDirection.x = 0;
    vectorDirection.y = 0;
    objectToCarryId = std::nullopt;
}

void Ice::PrintName() {
    std::cout << "Ice." << std::endl;
}

bool Ice::IsCloud() {
  return false;
}

bool Ice::Update(const uint8_t pressedKeys_) {
    bool needRedraw = false;

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

void Ice::GetSolidCollisions(std::vector<ObjectCollision> &collisions, bool& iceIsSuspendedInTheAir, bool& iceFoundAHoleOnTheFloor) {
    // Check for collisions with other objects present in the scene.
    std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(GetSolidLowerBound(), GetSolidUpperBound());
    iceIsSuspendedInTheAir = false;
    iceFoundAHoleOnTheFloor = false;
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
        // Compute position correction when Ice collides when moving to the right
        if ((vectorDirection.x > 0) && (vectorDirection.y == 0) && (intersection.rightIntersectionX < 0) && (intersection.bottomIntersectionY != 0)) {
            horizontalCorrection = intersection.rightIntersectionX;
        }
        // Compute position correction when Ice collides when moving to the left
        else if ((vectorDirection.x < 0) && (vectorDirection.y == 0) && (intersection.leftIntersectionX > 0) && (intersection.bottomIntersectionY != 0)) {
            horizontalCorrection = intersection.leftIntersectionX;
        }
        // Compute position correction when Ice collides with the ground during a fall
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

    std::cout << " > ICE COLLIDES WITH " << objectIntersections.size() << " OBJECTS. " << collisions.size() << " CORRECTIONS NEEDED.\n";

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
    if ((underlyingObjectCandidate == nullptr) || ((underlyingObjectCandidate != nullptr) && (numPixelsUnderlyingObjectsSurface <= (currentSprite.width >> 1)))) {
        iceIsSuspendedInTheAir = true;
        std::cout << "\n\n >>>>>>>>>> ICE is suspended in the air <<<<<<<<<<<\n\n";

        if (underlyingObjectCandidate != nullptr) {
            currentUnderlyingObject = underlyingObjectCandidate;
        }
    }

    cout << "\n >>>>>>> ICE UNDERLYING SURFACE: " << numPixelsUnderlyingObjectsSurface << " currentSprite.width: " << currentSprite.width << "\n";
    // Check if a hole is present on the ground based on an heuristic way.
    // If the number of pixels of the underlying surface is 3 pixels (or more) lower than the width of
    // the Ice then there is a hole under Ice. Note that screen edges are not taken in consideration.
    if ((underlyingObjectCandidate != nullptr) && !((position.GetRealX() < 0.0f) || (position.GetRealX() >= LEVEL_WIDTH_FLOAT - currentSprite.width)) && (numPixelsUnderlyingObjectsSurface <= currentSprite.width - 3)) {
        iceFoundAHoleOnTheFloor = true;
        objectToCarryId = underlyingObjectCandidate->id;
        //cout << " >>>>>>> ICE NEED TO CARRY AN OBJECT OF TYPE: ";
        //underlyingObjectCandidate->PrintName();
        //cout << "\n\n";
    }
}

void Ice::UpdateCollisions() {
    std::vector<ObjectCollision> collisions;
    bool iceIsSuspendedInTheAir = false;
    bool iceFoundAHoleOnTheFloor = false;

    // Search for collisions with solid objects
    this->GetSolidCollisions(collisions, iceIsSuspendedInTheAir, iceFoundAHoleOnTheFloor);

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
}

void Ice::MoveTo(Direction direction, float distance) {
    if (isBeingPushed) {
        PositionAddX(direction == Direction::RIGHT ? distance : -(distance));
        vectorDirection.x = (direction == Direction::RIGHT ? 1 : -1);
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
    // TODO
    return false;
}

void Ice::STATE_Move_Right() {
    cout << "\n >>>>>> ICE::STATE_Move_Right <<<<<<\n";
    isBeingPushed = true;
    direction = Direction::RIGHT;
    LoadAnimationWithId(IceAnimation::ICE_STICKY);
}

void Ice::STATE_Move_Left() {
    cout << "\n >>>>>> ICE::STATE_Move_Left <<<<<<\n";
    isBeingPushed = true;
    direction = Direction::LEFT;
    LoadAnimationWithId(IceAnimation::ICE_STICKY);
}

void Ice::HoleDetectedWhenMoving() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Move_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Move_Left
    END_TRANSITION_MAP(nullptr)
}