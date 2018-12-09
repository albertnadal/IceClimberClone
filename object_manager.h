#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <iostream>
#include <string>
#include <object_type.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem.h>

using namespace std;

class ObjectManager
{
  std::vector<ObjectType*> objectTypes;
  std::string textureFilename;
  unsigned int textureId;
public:
  ObjectManager();
  ~ObjectManager();
  void LoadObjectsDataFromFile(std::string filename);
  unsigned int LoadObjectsSpritesToGPU();
  void Print();
};

bool startsWith(std::string mainStr, std::string toMatch);
#endif
