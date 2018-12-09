#ifndef SCENE_OBJECT_TEXTURE_MANAGER_H
#define SCENE_OBJECT_TEXTURE_MANAGER_H

#include <iostream>
#include <string>
#include <object_type.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem.h>

using namespace std;

class SceneObjectTextureManager
{
  std::vector<ObjectType*> objectTypes;
  std::string textureFilename;
  unsigned int textureId;
public:
  SceneObjectTextureManager();
  ~SceneObjectTextureManager();
  void LoadObjectsDataFromFile(std::string filename);
  unsigned int SaveObjectsTextureToGPU();
  void Print();
};

bool startsWith(std::string mainStr, std::string toMatch);
#endif
