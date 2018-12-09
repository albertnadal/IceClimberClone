#pragma once

#include <string>
#include <object_type.h>

using namespace std;

class ObjectManager
{
  std::vector<ObjectType*> objectTypes;
  std::string textureFilename;
public:
  ObjectManager();
  ~ObjectManager();
  void LoadObjectsFromFile(std::string filename);
  void Print();
};
