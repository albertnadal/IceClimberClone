#include "scene_object_manager.h"
#include "scene_object_factory.h"
#include "scene_object.h"

SceneObjectManager::SceneObjectManager(SceneObjectDataManager* _textureManager, UInt16DoubleBuffer* _verticesDoubleBuffer, FloatDoubleBuffer* _uvsDoubleBuffer, uint32 _maxObjects) {
        textureManager = _textureManager;
        verticesDoubleBuffer = _verticesDoubleBuffer;
        uvsDoubleBuffer = _uvsDoubleBuffer;
        maxObjects = _maxObjects;
        spacePartitionObjectsTree.setDimension(2);
        currentEscalatedHeight = 0; // height climbed
        cameraIsMoving = false;
        currentRow = 0;
        visibleRows = 56;

        BuildWorld();
}

void SceneObjectManager::BuildWorld() {

  for(uint16 row=0; row<visibleRows; row++) {
    uint16 y = (map_viewport_height - 1) - row - currentRow;
    std::vector<ISceneObject*> rowObjects;
    for(uint16 x=0;x<map_viewport_width;x++) {
      if(SceneObjectIdentificator obj_id = (SceneObjectIdentificator)worldMap[y][x]) {
        if(ISceneObject *objectPtr = SceneObjectFactory::Get(textureManager)->CreateSceneObject(obj_id)) {

          // Set the initial position of the object in the screen
          objectPtr->position.setX(uint16(x*cell_w));
          objectPtr->position.setY(uint16(row*cell_h));
          rowObjects.push_back(objectPtr);

          // Initial update to load the sprites and boundary box
          objectPtr->Update();

          // Insert the object into the space partition tree used for object collision detection
          spacePartitionObjectsTree.insertParticle(objectPtr, objectPtr->GetLowerBound(), objectPtr->GetUpperBound());

          // Save pointers to proper arrays for static objects and mobile objects
          if(objectPtr->Type() == SceneObjectType::TERRAIN) staticObjects[objectPtr->uniqueId] = objectPtr;
          else mobileObjects[objectPtr->uniqueId] = objectPtr;
        }
      }
    }
    rowsBuffer.push_back(rowObjects);
  }

}

void SceneObjectManager::Update(uchar pressedKeys) {
  updateMobileObjects(pressedKeys);
  updateStaticObjects();
  updateVerticalScroll(pressedKeys);
  updateVerticesAndUVSBuffers();
}

void SceneObjectManager::updateVerticesBufferAtIndex(uint16 index, ISceneObject *objectPtr) {
  verticesDoubleBuffer->producer_buffer[index * 12] = objectPtr->position.int_x + objectPtr->Width();
  verticesDoubleBuffer->producer_buffer[index * 12 + 1] = objectPtr->position.int_y;

  // bottom right
  verticesDoubleBuffer->producer_buffer[index * 12 + 2] = objectPtr->position.int_x + objectPtr->Width();
  verticesDoubleBuffer->producer_buffer[index * 12 + 3] = objectPtr->position.int_y + objectPtr->Height();

  // top left
  verticesDoubleBuffer->producer_buffer[index * 12 + 4] = objectPtr->position.int_x;
  verticesDoubleBuffer->producer_buffer[index * 12 + 5] = objectPtr->position.int_y;

  // bottom right
  verticesDoubleBuffer->producer_buffer[index * 12 + 6] = objectPtr->position.int_x + objectPtr->Width();
  verticesDoubleBuffer->producer_buffer[index * 12 + 7] = objectPtr->position.int_y + objectPtr->Height();

  // bottom left
  verticesDoubleBuffer->producer_buffer[index * 12 + 8] = objectPtr->position.int_x;
  verticesDoubleBuffer->producer_buffer[index * 12 + 9] = objectPtr->position.int_y + objectPtr->Height();

  // top left
  verticesDoubleBuffer->producer_buffer[index * 12 + 10] = objectPtr->position.int_x;
  verticesDoubleBuffer->producer_buffer[index * 12 + 11] = objectPtr->position.int_y;
}

void SceneObjectManager::updateUVSBufferAtIndex(uint16 index, ISceneObject *objectPtr) {
  // top right
  uvsDoubleBuffer->producer_buffer[index * 12] = objectPtr->currentSprite.u2;
  uvsDoubleBuffer->producer_buffer[index * 12 + 1] = objectPtr->currentSprite.v2;

  // bottom right
  uvsDoubleBuffer->producer_buffer[index * 12 + 2] = objectPtr->currentSprite.u2;
  uvsDoubleBuffer->producer_buffer[index * 12 + 3] = objectPtr->currentSprite.v1;

  // top left
  uvsDoubleBuffer->producer_buffer[index * 12 + 4] = objectPtr->currentSprite.u1;
  uvsDoubleBuffer->producer_buffer[index * 12 + 5] = objectPtr->currentSprite.v2;

  // bottom right
  uvsDoubleBuffer->producer_buffer[index * 12 + 6] = objectPtr->currentSprite.u2;
  uvsDoubleBuffer->producer_buffer[index * 12 + 7] = objectPtr->currentSprite.v1;

  // bottom left
  uvsDoubleBuffer->producer_buffer[index * 12 + 8] = objectPtr->currentSprite.u1;
  uvsDoubleBuffer->producer_buffer[index * 12 + 9] = objectPtr->currentSprite.v1;

  // top left
  uvsDoubleBuffer->producer_buffer[index * 12 + 10] = objectPtr->currentSprite.u1;
  uvsDoubleBuffer->producer_buffer[index * 12 + 11] = objectPtr->currentSprite.v2;
}

void SceneObjectManager::updateVerticesAndUVSBuffers() {
  uint16 i = 0;
  for (auto const& x : staticObjects) {
    ISceneObject* objectPtr = x.second;
    updateVerticesBufferAtIndex(i, objectPtr);
    updateUVSBufferAtIndex(i, objectPtr);
    i++;
  }

  for (auto const& x : mobileObjects) {
    ISceneObject* objectPtr = x.second;
    updateVerticesBufferAtIndex(i, objectPtr);
    updateUVSBufferAtIndex(i, objectPtr);
    i++;
  }

  // Clean unused buffer area
  verticesDoubleBuffer->cleanDataFromPosition(i*12);
  uvsDoubleBuffer->cleanDataFromPosition(i*12);

  verticesDoubleBuffer->swapBuffers();
  uvsDoubleBuffer->swapBuffers();
}

void SceneObjectManager::updateMobileObjects(uchar pressedKeys) {
  for (auto const& x : mobileObjects) {
    ISceneObject* objectPtr = x.second;
    objectPtr->Update(pressedKeys, spacePartitionObjectsTree);
  }
}

void SceneObjectManager::updateStaticObjects() {
  for (auto const& x : staticObjects) {
    ISceneObject* objectPtr = x.second;
    objectPtr->Update();
  }
}

void SceneObjectManager::updateVerticalScroll(uchar pressedKeys) {

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
              spacePartitionObjectsTree.removeParticle(objectPtr);

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
        objects[o]->position.addY(-pixelDisplacement);
      }
    }
    totalPixelDisplacement+=pixelDisplacement;
  }

  if((pressedKeys & KeyboardKeyCode::KEY_W) == KeyboardKeyCode::KEY_W) {
    if(!cameraIsMoving) {
      cameraIsMoving = true;
      totalPixelDisplacement = 0.0f;
      for(uint16 row=0; row<levelRowOffset; row++) {
        uint16 y = (map_viewport_height - 1) - row - currentRow - visibleRows;
        std::vector<ISceneObject*> rowObjects;
        for(uint16 x=0;x<map_viewport_width;x++) {
          if(SceneObjectIdentificator obj_id = (SceneObjectIdentificator)worldMap[y][x]) {
            if(ISceneObject *objectPtr = SceneObjectFactory::Get(textureManager)->CreateSceneObject(obj_id)) {
              objectPtr->position.setX(uint16(x*cell_w));
              objectPtr->position.setY(uint16((visibleRows+row)*cell_h));
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
}

SceneObjectManager::~SceneObjectManager() {

}
