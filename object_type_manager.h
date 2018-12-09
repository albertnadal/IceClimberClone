#pragma once

#include <string>
#include <object_type.h>

using namespace std;

class ObjectTypeManager
{
  std::vector<ObjectType*> objectTypes;
public:
  ObjectTypeManager();
  ~ObjectTypeManager();
  void LoadFromFile(std::string filename);
  void Print();
};
