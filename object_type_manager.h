#pragma once

#include <string>
#include <object_type.h>

#define OBJECT_TYPES_FILENAME "objtypes.dat"

using namespace std;

class ObjectTypeManager
{
  const string object_types_filename = OBJECT_TYPES_FILENAME;
public:
  ObjectTypeManager();
  void Load();
};
