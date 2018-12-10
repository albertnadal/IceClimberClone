#include "scene_object_manager.h"
#include "scene_object_factory.h"
#include "scene_object.h"
#include "main_character.h"

SceneObjectManager::SceneObjectManager(SceneObjectTextureManager* _textureManager) {

  textureManager = _textureManager;

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

void SceneObjectManager::Update() {
  for(int i=0; i<objects.size(); i++) {
    objects[i]->Update();
  }
}

SceneObjectManager::~SceneObjectManager() {

}
