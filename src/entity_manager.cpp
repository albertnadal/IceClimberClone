#include <entity_manager.h>
#include <entity_factory.h>
#include <entity.h>

EntityManager::EntityManager(EntityDataManager* _textureManager, SpriteRectDoubleBuffer* _spriteRectDoubleBuffer, uint32_t _maxObjects) {
        textureManager = _textureManager;
        spriteRectDoubleBuffer = _spriteRectDoubleBuffer;
        maxObjects = _maxObjects;
        spacePartitionObjectsTree = new aabb::Tree<IEntity*>();
        spacePartitionObjectsTree->setDimension(2);
        currentEscalatedHeight = 0; // height climbed
        cameraIsMoving = false;
        currentRow = 0;
        visibleRows = 56;
        currentCameraPosition = newCameraPosition = 0.0f;
        BuildMountain();
}

void EntityManager::BuildMountain() {
  // Each level is six cells height. The entire mountain have 4 extra rows on top to enforce level floors to be
  // located in vertical position multiple of six. One additional row is appended to show the water.
  for(int row=0; row<4 + 30*6 + 1; row++) {
    for(int col=0; col<map_viewport_width; col++) {
      if(EntityIdentificator entity_id = (EntityIdentificator)mountainMap[row][col]) {
        std::optional<IEntity *> entity_ptr = CreateEntityWithId(entity_id, col, row);
      }
    }
  }
}

std::optional<IEntity *> EntityManager::CreateEntityWithId(EntityIdentificator entity_id, int x, int y) {
  std::optional<IEntity *> entity_ptr = EntityFactory::Get(this, textureManager, spacePartitionObjectsTree)->CreateEntity(entity_id);

  if(entity_ptr.has_value()) {
    if (entity_id == EntityIdentificator::POPO) {
      player = *entity_ptr;
      currentCameraPosition = newCameraPosition = INITIAL_CAMERA_POSITION;
    }

    // Set the initial position of the object in the screen
    (*entity_ptr)->position.setInitialXY(x*CELL_WIDTH_FLOAT, y*CELL_HEIGHT_FLOAT);

    // Initial update to load the sprites and boundary box
    (*entity_ptr)->Update();

    // Insert the object into the space partition tree used for object collision detection
    std::vector<int> lowerBound = (*entity_ptr)->GetLowerBound();
    std::vector<int> upperBound = (*entity_ptr)->GetUpperBound();
    spacePartitionObjectsTree->insertParticle(*entity_ptr, lowerBound, upperBound);

    // Save pointers to proper arrays for static objects and mobile objects
    if((*entity_ptr)->Type() == EntityType::TERRAIN) staticObjects[(*entity_ptr)->uniqueId] = *entity_ptr;
    else mobileObjects[(*entity_ptr)->uniqueId] = *entity_ptr;
  }

  return entity_ptr;
}

void EntityManager::PlayerReachedNewAltitude(int cellY) {
  if (std::find(validAltitudes.begin(), validAltitudes.end(), cellY) != validAltitudes.end()) {
    float padding_top = (cellY > BONUS_STAGE_CELL_Y) ? CAMERA_PADDING_TOP : CAMERA_BONUS_STAGE_PADDING_TOP;
    newCameraPosition = cellY*CELL_HEIGHT_FLOAT - padding_top;
  }
}

void EntityManager::PlayerEnteredBonusStage() {
  // TODO
}

std::optional<float> EntityManager::Update(uint8_t pressedKeys) {
  updateMobileObjects(pressedKeys);
  updateStaticObjects();
  updateSpriteRectBuffers();
  deleteUneededObjects();

  // Update vertical camera position when player reaches new level height
  if (newCameraPosition < currentCameraPosition) {
    currentCameraPosition -= CAMERA_SPEED; // Progressive update to get an smooth transition
    return currentCameraPosition;
  }

  return std::nullopt;
}

void EntityManager::updateSpriteRectBuffers() {
  int i = 0;
  std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(player->GetLowerBound(), player->GetUpperBound());

  for (auto const& x : staticObjects) {
    IEntity* entity_ptr = x.second;
    Rectangle src = { entity_ptr->currentSprite.u1, entity_ptr->currentSprite.v1, entity_ptr->currentSprite.u2, entity_ptr->currentSprite.v2 };
    Vector2 pos = { entity_ptr->position.GetX(), entity_ptr->position.GetY() };
    Boundaries boundaries = entity_ptr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = entity_ptr->GetAbsoluteSolidBoundaries();
    Color tint = WHITE;

    // Tint in RED those objects that are candidates to collide with the player object.
    auto it = std::find_if(objectIntersections.begin(), objectIntersections.end(),
                           [entity_ptr](const aabb::AABBIntersection<IEntity*>& intersection) {
                               return intersection.particle == entity_ptr;
                           });

    if (it != objectIntersections.end()) {
      tint = RED;
    }

    spriteRectDoubleBuffer->producer_buffer[i++] = SpriteRect(src, pos, boundaries, tint);
  }

  for (auto const& x : mobileObjects) {
    IEntity* entity_ptr = x.second;
    Rectangle src = { entity_ptr->currentSprite.u1, entity_ptr->currentSprite.v1, entity_ptr->currentSprite.u2, entity_ptr->currentSprite.v2 };
    Vector2 pos = { entity_ptr->position.GetX(), entity_ptr->position.GetY() };
    Boundaries boundaries = entity_ptr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = entity_ptr->GetAbsoluteSolidBoundaries();
    spriteRectDoubleBuffer->producer_buffer[i] = SpriteRect(src, pos, boundaries, WHITE);
    i++;
  }

  spriteRectDoubleBuffer->producer_buffer_length = i;
  spriteRectDoubleBuffer->swapBuffers();
}

void EntityManager::updateEntities(std::map<uint32_t, IEntity*>& objects, std::optional<uint8_t> pressedKeys = std::nullopt) {
    for (auto const& x : objects) {
        IEntity* entity_ptr = x.second;

        if (entity_ptr->isMarkedToDelete) {
            objectsToDelete.push_back(entity_ptr);
            continue;
        }

        if (pressedKeys.has_value()) {
            entity_ptr->Update(pressedKeys.value());
        } else {
            entity_ptr->Update();
        }
    }
}

void EntityManager::updateMobileObjects(uint8_t pressedKeys) {
    updateEntities(mobileObjects, pressedKeys);
}

void EntityManager::updateStaticObjects() {
    updateEntities(staticObjects);
}

void EntityManager::deleteUneededObjects() {
  for (auto entity_ptr : objectsToDelete) {
    staticObjects.erase(entity_ptr->uniqueId);
    mobileObjects.erase(entity_ptr->uniqueId);

    // Objects are responsible for removing themselves from the space partition tree, so the
    // following code is just for safety.
    spacePartitionObjectsTree->removeParticle(entity_ptr);

    delete entity_ptr;
  }

  objectsToDelete.clear();
}

EntityManager::~EntityManager() {
  if(spacePartitionObjectsTree != nullptr) {
    delete spacePartitionObjectsTree;
  }
}
