#include "scene_object_manager.h"
#include "scene_object_factory.h"
#include "scene_object.h"
#include "main_character.h"

SceneObjectManager::SceneObjectManager(SceneObjectTextureManager* _textureManager, uint32 _maxObjects, uint16 *_vertices,  uint32 *_vbo, float *_uvs, uint32 *_ubo) {
        vertices = _vertices;
        uvs = _uvs;
        vbo = _vbo;
        ubo = _ubo;
        textureManager = _textureManager;
        maxObjects = _maxObjects;

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
        }
}

void SceneObjectManager::Update(uchar pressedKeys) {
        for(int i=0; i<objects.size(); i++) {
                ISceneObject *object = objects[i];
                bool needRedraw = object->Update(pressedKeys);

                if(needRedraw) {
                        vertices[i * 12] = object->position.x + object->Width();
                        vertices[i * 12 + 1] = object->position.y;

                        // bottom right
                        vertices[i * 12 + 2] = object->position.x + object->Width();
                        vertices[i * 12 + 3] = object->position.y + object->Height();

                        // top left
                        vertices[i * 12 + 4] = object->position.x;
                        vertices[i * 12 + 5] = object->position.y;

                        // bottom right
                        vertices[i * 12 + 6] = object->position.x + object->Width();
                        vertices[i * 12 + 7] = object->position.y + object->Height();

                        // bottom left
                        vertices[i * 12 + 8] = object->position.x;
                        vertices[i * 12 + 9] = object->position.y + object->Height();

                        // top left
                        vertices[i * 12 + 10] = object->position.x;
                        vertices[i * 12 + 11] = object->position.y;

                        // top right
                        uvs[i * 12] = object->currentSprite.u2;
                        uvs[i * 12 + 1] = object->currentSprite.v2;

                        // bottom right
                        uvs[i * 12 + 2] = object->currentSprite.u2;
                        uvs[i * 12 + 3] = object->currentSprite.v1;

                        // top left
                        uvs[i * 12 + 4] = object->currentSprite.u1;
                        uvs[i * 12 + 5] = object->currentSprite.v2;

                        // bottom right
                        uvs[i * 12 + 6] = object->currentSprite.u2;
                        uvs[i * 12 + 7] = object->currentSprite.v1;

                        // bottom left
                        uvs[i * 12 + 8] = object->currentSprite.u1;
                        uvs[i * 12 + 9] = object->currentSprite.v1;

                        // top left
                        uvs[i * 12 + 10] = object->currentSprite.u1;
                        uvs[i * 12 + 11] = object->currentSprite.v2;
                }

                //cout << "X: " << object->position.x << " Y: " << object->position.y << " | UVS u2:" << uvs[i * 12] << " v2:" << uvs[i * 12 + 1] << " u2:" << uvs[i * 12 + 2] << " v1:" << uvs[i * 12 + 3] << " u1:" << uvs[i * 12 + 4] << " v2:" << uvs[i * 12 + 5] << " u2:" << uvs[i * 12 + 6] << " v1:" << uvs[i * 12 + 7] << " u1:" << uvs[i * 12 + 8] << " v1:" << uvs[i * 12 + 9] << " u1:" << uvs[i * 12 + 10] << " v2:" << uvs[i * 12 + 11] << endl;
        }

cout << "OK0 ";
//        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, OBJECT_COUNT * (12 * sizeof(uint16)), vertices);
//cout << "OK1 " << endl;
//        glBindBuffer(GL_ARRAY_BUFFER, *ubo);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, OBJECT_COUNT * (12 * sizeof(float)), uvs);
//cout << "OK2 " << endl;
}

SceneObjectManager::~SceneObjectManager() {

}
