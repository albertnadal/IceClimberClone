#include "entity_manager.h"
#include "entity_factory.h"
#include "entity.h"

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
        BuildWorld();
}

void EntityManager::BuildWorld() {
  for(uint16_t row=0; row<7*6; row++) {
    //uint16_t y = (map_viewport_height - 1) - row - currentRow;
    for(uint16_t col=0; col<map_viewport_width; col++) {

      if(EntityIdentificator obj_id = (EntityIdentificator)worldMap[row][col]) {
        std::optional<IEntity *> entityPtr = EntityFactory::Get(textureManager, spacePartitionObjectsTree)->CreateEntity(obj_id);

        if(entityPtr.has_value()) {
          if (obj_id == EntityIdentificator::MAIN_CHARACTER) {
            player = *entityPtr;
          }

          // Set the initial position of the object in the screen
          (*entityPtr)->position.setX(int16_t(col*cell_w));
          (*entityPtr)->position.setY(int16_t(row*cell_h));

          // Initial update to load the sprites and boundary box
          (*entityPtr)->Update();

          // Insert the object into the space partition tree used for object collision detection
          std::vector<int> lowerBound = (*entityPtr)->GetLowerBound();
          std::vector<int> upperBound = (*entityPtr)->GetUpperBound();
          spacePartitionObjectsTree->insertParticle(*entityPtr, lowerBound, upperBound);

          // Save pointers to proper arrays for static objects and mobile objects
          if((*entityPtr)->Type() == EntityType::TERRAIN) staticObjects[(*entityPtr)->uniqueId] = *entityPtr;
          else mobileObjects[(*entityPtr)->uniqueId] = *entityPtr;
        }
      }
    }
  }
}

void EntityManager::Update(uint8_t pressedKeys) {
  updateMobileObjects(pressedKeys);
  updateStaticObjects();
  updateVerticalScroll(pressedKeys);
  updateSpriteRectBuffers();
  deleteUneededObjects();
}

void EntityManager::updateSpriteRectBuffers() {
  int i = 0;
  std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(player->GetLowerBound(), player->GetUpperBound());

  for (auto const& x : staticObjects) {
    IEntity* entityPtr = x.second;
    Rectangle src = { entityPtr->currentSprite.u1, entityPtr->currentSprite.v1, entityPtr->currentSprite.u2, entityPtr->currentSprite.v2 };
    Vector2 pos = { entityPtr->position.GetX(), entityPtr->position.GetY() };
    Boundaries boundaries = entityPtr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = entityPtr->GetAbsoluteSolidBoundaries();
    Color tint = WHITE;

    // Tint in RED those objects that are candidates to collide with the player object.
    auto it = std::find_if(objectIntersections.begin(), objectIntersections.end(),
                           [entityPtr](const aabb::AABBIntersection<IEntity*>& intersection) {
                               return intersection.particle == entityPtr;
                           });

    if (it != objectIntersections.end()) {
      tint = RED;
    }

    spriteRectDoubleBuffer->producer_buffer[i++] = SpriteRect(src, pos, boundaries, tint);
  }

  for (auto const& x : mobileObjects) {
    IEntity* entityPtr = x.second;
    Rectangle src = { entityPtr->currentSprite.u1, entityPtr->currentSprite.v1, entityPtr->currentSprite.u2, entityPtr->currentSprite.v2 };
    Vector2 pos = { entityPtr->position.GetX(), entityPtr->position.GetY() };
    Boundaries boundaries = entityPtr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = entityPtr->GetAbsoluteSolidBoundaries();
    spriteRectDoubleBuffer->producer_buffer[i] = SpriteRect(src, pos, boundaries, WHITE);
    i++;
  }

  spriteRectDoubleBuffer->producer_buffer_length = i;
  spriteRectDoubleBuffer->swapBuffers();
}

void EntityManager::updateEntities(std::map<uint32_t, IEntity*>& objects, std::optional<uint8_t> pressedKeys = std::nullopt) {
    for (auto const& x : objects) {
        IEntity* entityPtr = x.second;

        if (entityPtr->isMarkedToDelete) {
            objectsToDelete.push_back(entityPtr);
            continue;
        }

        if (pressedKeys.has_value()) {
            entityPtr->Update(pressedKeys.value());
        } else {
            entityPtr->Update();
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
  for (auto entityPtr : objectsToDelete) {
    staticObjects.erase(entityPtr->uniqueId);
    mobileObjects.erase(entityPtr->uniqueId);

    // Objects are responsible for removing themselves from the space partition tree, so the
    // following code is just for safety.
    spacePartitionObjectsTree->removeParticle(entityPtr);

    delete entityPtr;
  }

  objectsToDelete.clear();
}

void EntityManager::updateVerticalScroll(uint8_t pressedKeys) {
/*
  if(cameraIsMoving) {
    float pixelDisplacement = 2.5f;
    if((totalPixelDisplacement + pixelDisplacement) >= levelRowOffset*cell_h) {
      pixelDisplacement = levelRowOffset*cell_h - totalPixelDisplacement;
      cameraIsMoving = false;

      // Remove bottom hidden rows
      for(int r=0; r<levelRowOffset && r<rowsBuffer.size(); r++) {
        auto objects = rowsBuffer.front();
        for(int o=0; o<objects.size(); o++) {
              // Remove the object refefence from all data structures
              IEntity *entityPtr = objects[o];

              // Remove the object from the spacet partitioning tree
              spacePartitionObjectsTree->removeParticle(entityPtr);

              if(entityPtr->Type() == EntityType::TERRAIN) {
                staticObjects.erase(entityPtr->uniqueId);
              } else {
                mobileObjects.erase(entityPtr->uniqueId);
              }

              delete entityPtr;
        }
        rowsBuffer.pop_front();
      }
    }

    for(int r=0; r<rowsBuffer.size(); r++) {
      auto objects = rowsBuffer[r];
      for(int o=0; o<objects.size(); o++) {
        objects[o]->PositionAddY(-pixelDisplacement);
      }
    }
    totalPixelDisplacement+=pixelDisplacement;
  }

  if((pressedKeys & KeyboardKeyCode::IC_KEY_W) == KeyboardKeyCode::IC_KEY_W) {
    if(!cameraIsMoving) {
      cameraIsMoving = true;
      totalPixelDisplacement = 0.0f;
      for(uint16_t row=0; row<levelRowOffset; row++) {
        uint16_t y = (map_viewport_height - 1) - row - currentRow - visibleRows;
        std::vector<IEntity*> rowObjects;
        for(uint16_t x=0;x<map_viewport_width;x++) {
          if(EntityIdentificator obj_id = (EntityIdentificator)worldMap[y][x]) {
            if(IEntity *entityPtr = EntityFactory::Get(textureManager, spacePartitionObjectsTree)->CreateEntity(obj_id)) {
              entityPtr->position.setX(int16_t(x*cell_w));
              entityPtr->position.setY(int16_t((visibleRows+row)*cell_h));
              rowObjects.push_back(entityPtr);

              if(entityPtr->Type() == EntityType::TERRAIN) staticObjects[entityPtr->uniqueId] = entityPtr;
              else mobileObjects[entityPtr->uniqueId] = entityPtr;
            }
          }
        }
        rowsBuffer.push_back(rowObjects);
      }
      currentRow+=levelRowOffset;
    }
  }
*/
}

EntityManager::~EntityManager() {
  if(spacePartitionObjectsTree != nullptr) {
    delete spacePartitionObjectsTree;
  }
}
