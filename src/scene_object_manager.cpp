#include "scene_object_manager.h"
#include "scene_object_factory.h"
#include "scene_object.h"

SceneObjectManager::SceneObjectManager(SceneObjectDataManager* _textureManager, SpriteRectDoubleBuffer* _spriteRectDoubleBuffer, uint32_t _maxObjects) {
        textureManager = _textureManager;
        spriteRectDoubleBuffer = _spriteRectDoubleBuffer;
        maxObjects = _maxObjects;
        spacePartitionObjectsTree = new aabb::Tree<ISceneObject*>();
        spacePartitionObjectsTree->setDimension(2);
        currentEscalatedHeight = 0; // height climbed
        cameraIsMoving = false;
        currentRow = 0;
        visibleRows = 56;
        BuildWorld();
}

void SceneObjectManager::BuildWorld() {
  for(uint16_t row=0; row<7*6; row++) {
    //uint16_t y = (map_viewport_height - 1) - row - currentRow;
    for(uint16_t col=0; col<map_viewport_width; col++) {

      if(SceneObjectIdentificator obj_id = (SceneObjectIdentificator)worldMap[row][col]) {
        std::optional<ISceneObject *> objectPtr = SceneObjectFactory::Get(textureManager, spacePartitionObjectsTree)->CreateSceneObject(obj_id);

        if(objectPtr.has_value()) {
          if (obj_id == SceneObjectIdentificator::MAIN_CHARACTER) {
            player = *objectPtr;
          }

          // Set the initial position of the object in the screen
          (*objectPtr)->position.setX(int16_t(col*cell_w));
          (*objectPtr)->position.setY(int16_t(row*cell_h));

          // Initial update to load the sprites and boundary box
          (*objectPtr)->Update();

          // Insert the object into the space partition tree used for object collision detection
          std::vector<uint16_t> lowerBound = (*objectPtr)->GetLowerBound();
          std::vector<uint16_t> upperBound = (*objectPtr)->GetUpperBound();
          spacePartitionObjectsTree->insertParticle(*objectPtr, lowerBound, upperBound);

          // Save pointers to proper arrays for static objects and mobile objects
          if((*objectPtr)->Type() == SceneObjectType::TERRAIN) staticObjects[(*objectPtr)->uniqueId] = *objectPtr;
          else mobileObjects[(*objectPtr)->uniqueId] = *objectPtr;
        }
      }
    }
  }
}

void SceneObjectManager::Update(uint8_t pressedKeys) {
  updateMobileObjects(pressedKeys);
  updateStaticObjects();
  updateVerticalScroll(pressedKeys);
  updateSpriteRectBuffers();
  deleteUneededObjects();
}

void SceneObjectManager::updateSpriteRectBuffers() {
  int i = 0;
  std::vector<aabb::AABBIntersection<ISceneObject*>> objectIntersections = spacePartitionObjectsTree->query(player->GetLowerBound(), player->GetUpperBound());

  for (auto const& x : staticObjects) {
    ISceneObject* objectPtr = x.second;
    Rectangle src = { objectPtr->currentSprite.u1, objectPtr->currentSprite.v1, objectPtr->currentSprite.u2, objectPtr->currentSprite.v2 };
    Vector2 pos = { objectPtr->position.GetX(), objectPtr->position.GetY() };
    Boundaries boundaries = objectPtr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = objectPtr->GetAbsoluteSolidBoundaries();
    Color tint = WHITE;

    // Tint in RED those objects that are candidates to collide with the player object.
    auto it = std::find_if(objectIntersections.begin(), objectIntersections.end(),
                           [objectPtr](const aabb::AABBIntersection<ISceneObject*>& intersection) {
                               return intersection.particle == objectPtr;
                           });

    if (it != objectIntersections.end()) {
      tint = RED;
    }

    spriteRectDoubleBuffer->producer_buffer[i++] = SpriteRect(src, pos, boundaries, tint);
  }

  for (auto const& x : mobileObjects) {
    ISceneObject* objectPtr = x.second;
    Rectangle src = { objectPtr->currentSprite.u1, objectPtr->currentSprite.v1, objectPtr->currentSprite.u2, objectPtr->currentSprite.v2 };
    Vector2 pos = { objectPtr->position.GetX(), objectPtr->position.GetY() };
    Boundaries boundaries = objectPtr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = objectPtr->GetAbsoluteSolidBoundaries();
    spriteRectDoubleBuffer->producer_buffer[i] = SpriteRect(src, pos, boundaries, WHITE);
    i++;
  }

  spriteRectDoubleBuffer->producer_buffer_length = i;
  spriteRectDoubleBuffer->swapBuffers();
}

void SceneObjectManager::updateObjects(std::map<uint32_t, ISceneObject*>& objects, std::optional<uint8_t> pressedKeys = std::nullopt) {
    for (auto const& x : objects) {
        ISceneObject* objectPtr = x.second;

        if (objectPtr->isMarkedToDelete) {
            objectsToDelete.push_back(objectPtr);
            continue;
        }

        if (pressedKeys.has_value()) {
            objectPtr->Update(pressedKeys.value());
        } else {
            objectPtr->Update();
        }
    }
}

void SceneObjectManager::updateMobileObjects(uint8_t pressedKeys) {
    updateObjects(mobileObjects, pressedKeys);
}

void SceneObjectManager::updateStaticObjects() {
    updateObjects(staticObjects);
}

void SceneObjectManager::deleteUneededObjects() {
  for (auto objectPtr : objectsToDelete) {
    staticObjects.erase(objectPtr->uniqueId);
    mobileObjects.erase(objectPtr->uniqueId);

    // Objects are responsible for removing themselves from the space partition tree, so the
    // following code is just for safety.
    spacePartitionObjectsTree->removeParticle(objectPtr);

    delete objectPtr;
  }

  objectsToDelete.clear();
}

void SceneObjectManager::updateVerticalScroll(uint8_t pressedKeys) {
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
              ISceneObject *objectPtr = objects[o];

              // Remove the object from the spacet partitioning tree
              spacePartitionObjectsTree->removeParticle(objectPtr);

              if(objectPtr->Type() == SceneObjectType::TERRAIN) {
                staticObjects.erase(objectPtr->uniqueId);
              } else {
                mobileObjects.erase(objectPtr->uniqueId);
              }

              delete objectPtr;
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
        std::vector<ISceneObject*> rowObjects;
        for(uint16_t x=0;x<map_viewport_width;x++) {
          if(SceneObjectIdentificator obj_id = (SceneObjectIdentificator)worldMap[y][x]) {
            if(ISceneObject *objectPtr = SceneObjectFactory::Get(textureManager, spacePartitionObjectsTree)->CreateSceneObject(obj_id)) {
              objectPtr->position.setX(int16_t(x*cell_w));
              objectPtr->position.setY(int16_t((visibleRows+row)*cell_h));
              rowObjects.push_back(objectPtr);

              if(objectPtr->Type() == SceneObjectType::TERRAIN) staticObjects[objectPtr->uniqueId] = objectPtr;
              else mobileObjects[objectPtr->uniqueId] = objectPtr;
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

SceneObjectManager::~SceneObjectManager() {
  if(spacePartitionObjectsTree != nullptr) {
    delete spacePartitionObjectsTree;
  }
}
