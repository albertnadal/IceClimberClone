#include "scene_object_manager.h"
#include "scene_object_factory.h"
#include "scene_object.h"
#include "main_character.h"

SceneObjectManager::SceneObjectManager(SceneObjectTextureManager* _textureManager, UInt16DoubleBuffer* _verticesDoubleBuffer, FloatDoubleBuffer* _uvsDoubleBuffer, uint32 _maxObjects) {
        textureManager = _textureManager;
        verticesDoubleBuffer = _verticesDoubleBuffer;
        uvsDoubleBuffer = _uvsDoubleBuffer;
        maxObjects = _maxObjects;
        currentEscalatedHeight = 0; // height climbed

        BuildWorld();

        ISceneObject *object = SceneObjectFactory::Get(textureManager)->CreateSceneObject("MainCharacter");
        if(object) {
                object->PrintName();
                objects.push_back(object);
        }


/*
        for(int i=0; i<maxObjects; i++) {
                ISceneObject *object = SceneObjectFactory::Get(textureManager)->CreateSceneObject("MainCharacter");
                if(object) {
                        object->PrintName();
                        objects.push_back(object);
                }
                else
                {
                        cout << "That scene object doesn't exist!" << endl;
                }
        }*/
}

void SceneObjectManager::BuildWorld() {
  //ISceneObject *object;
  uint16 cell_w = 16, cell_h = 16; // pixels
  uint16 map_viewport_width = 32; // cells
  uint16 map_viewport_height = 28; // cells
  for(uint16 y=0;y<map_viewport_height;y++) {
    for(uint16 x=0;x<map_viewport_width;x++) {
      if(ISceneObject *object = SceneObjectFactory::Get(textureManager)->CreateSceneObject("Brick")) {
        object->position.setX(uint16(x*cell_w));
        object->position.setY(uint16(y*cell_h));
        objects.push_back(object);
      }
    }
  }
}

void SceneObjectManager::Update(uchar pressedKeys) {
        for(int i=0; i<objects.size(); i++) {
                ISceneObject *object = objects[i];
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
        }

        verticesDoubleBuffer->swapBuffers();
        uvsDoubleBuffer->swapBuffers();
}

SceneObjectManager::~SceneObjectManager() {

}
