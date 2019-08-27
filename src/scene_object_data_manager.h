#ifndef SCENE_OBJECT_DATA_MANAGER_H
#define SCENE_OBJECT_DATA_MANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <object_sprite_sheet.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem.h>

#define OBJECT_TYPES_FILENAME "objtypes.dat"

using namespace std;

class SceneObjectDataManager
{
  typedef map<SceneObjectIdentificator, ObjectSpriteSheet*> SpriteSheetsMap;
  SpriteSheetsMap objectSpriteSheetsMap;
  std::string textureFilename;
  uint32_t textureId;
  void LoadObjectsDataFromFile(std::string filename);
  void Print();
public:
  SceneObjectDataManager();
  ~SceneObjectDataManager();
  uint32_t LoadObjectsTextures();
  ObjectSpriteSheet* GetSpriteSheetBySceneObjectIdentificator(SceneObjectIdentificator);
};

bool startsWith(std::string mainStr, std::string toMatch);
#endif
