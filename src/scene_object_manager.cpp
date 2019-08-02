#include "scene_object_manager.h"
#include "scene_object_factory.h"
#include "scene_object.h"

SceneObjectManager::SceneObjectManager(SceneObjectTextureManager* _textureManager, UInt16DoubleBuffer* _verticesDoubleBuffer, FloatDoubleBuffer* _uvsDoubleBuffer, uint32 _maxObjects) {
        textureManager = _textureManager;
        verticesDoubleBuffer = _verticesDoubleBuffer;
        uvsDoubleBuffer = _uvsDoubleBuffer;
        maxObjects = _maxObjects;
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
        if(ISceneObject *object = SceneObjectFactory::Get(textureManager)->CreateSceneObject(obj_id)) {
          object->position.setX(uint16(x*cell_w));
          object->position.setY(uint16(row*cell_h));
          rowObjects.push_back(object);


        }
      }
    }
    rowsBuffer.push_back(rowObjects);
  }

}

void SceneObjectManager::Update(uchar pressedKeys) {

        if(cameraIsMoving) {
          float pixelDisplacement = 2.5f;
          if((totalPixelDisplacement + pixelDisplacement) >= levelRowOffset*cell_h) {
            pixelDisplacement = levelRowOffset*cell_h - totalPixelDisplacement;
            cameraIsMoving = false;

            // Remove bottom hidden rows
            for(int r=0; r<levelRowOffset && r<rowsBuffer.size(); r++) {
              auto objects = rowsBuffer.front();
              for(int o=0; o<objects.size(); o++) {
                    ISceneObject *object = objects[o];
                    delete object;
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
                  if(ISceneObject *object = SceneObjectFactory::Get(textureManager)->CreateSceneObject(obj_id)) {
                    object->position.setX(uint16(x*cell_w));
                    object->position.setY(uint16((visibleRows+row)*cell_h));
                    rowObjects.push_back(object);
                  }
                }
              }
              rowsBuffer.push_back(rowObjects);
            }
            currentRow+=levelRowOffset;
          }
        }

        int i = 0;
        for(int r=0; r<rowsBuffer.size(); r++) {
          auto objects = rowsBuffer[r];
          for(int o=0; o<objects.size(); o++) {
                ISceneObject *object = objects[o];
                bool needRedraw = object->Update(pressedKeys);

                verticesDoubleBuffer->producer_buffer[i * 12] = object->position.int_x + object->Width();
                verticesDoubleBuffer->producer_buffer[i * 12 + 1] = object->position.int_y;

                // bottom right
                verticesDoubleBuffer->producer_buffer[i * 12 + 2] = object->position.int_x + object->Width();
                verticesDoubleBuffer->producer_buffer[i * 12 + 3] = object->position.int_y + object->Height();

                // top left
                verticesDoubleBuffer->producer_buffer[i * 12 + 4] = object->position.int_x;
                verticesDoubleBuffer->producer_buffer[i * 12 + 5] = object->position.int_y;

                // bottom right
                verticesDoubleBuffer->producer_buffer[i * 12 + 6] = object->position.int_x + object->Width();
                verticesDoubleBuffer->producer_buffer[i * 12 + 7] = object->position.int_y + object->Height();

                // bottom left
                verticesDoubleBuffer->producer_buffer[i * 12 + 8] = object->position.int_x;
                verticesDoubleBuffer->producer_buffer[i * 12 + 9] = object->position.int_y + object->Height();

                // top left
                verticesDoubleBuffer->producer_buffer[i * 12 + 10] = object->position.int_x;
                verticesDoubleBuffer->producer_buffer[i * 12 + 11] = object->position.int_y;

                // top right
                uvsDoubleBuffer->producer_buffer[i * 12] = object->currentSprite.u2;
                uvsDoubleBuffer->producer_buffer[i * 12 + 1] = object->currentSprite.v2;

                // bottom right
                uvsDoubleBuffer->producer_buffer[i * 12 + 2] = object->currentSprite.u2;
                uvsDoubleBuffer->producer_buffer[i * 12 + 3] = object->currentSprite.v1;

                // top left
                uvsDoubleBuffer->producer_buffer[i * 12 + 4] = object->currentSprite.u1;
                uvsDoubleBuffer->producer_buffer[i * 12 + 5] = object->currentSprite.v2;

                // bottom right
                uvsDoubleBuffer->producer_buffer[i * 12 + 6] = object->currentSprite.u2;
                uvsDoubleBuffer->producer_buffer[i * 12 + 7] = object->currentSprite.v1;

                // bottom left
                uvsDoubleBuffer->producer_buffer[i * 12 + 8] = object->currentSprite.u1;
                uvsDoubleBuffer->producer_buffer[i * 12 + 9] = object->currentSprite.v1;

                // top left
                uvsDoubleBuffer->producer_buffer[i * 12 + 10] = object->currentSprite.u1;
                uvsDoubleBuffer->producer_buffer[i * 12 + 11] = object->currentSprite.v2;
                i++;
              }
        }


        // Clean unused buffer area
        verticesDoubleBuffer->cleanDataFromPosition(i*12);
        uvsDoubleBuffer->cleanDataFromPosition(i*12);

        verticesDoubleBuffer->swapBuffers();
        uvsDoubleBuffer->swapBuffers();
}

SceneObjectManager::~SceneObjectManager() {

}
