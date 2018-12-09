#include "object_type_manager.h"
#include <fstream>
#include <sstream>
#include <utils.h>

ObjectTypeManager::ObjectTypeManager()
{

}

ObjectTypeManager::~ObjectTypeManager() {
  for (auto *objectType : objectTypes)
  {
    delete objectType;
  }

  objectTypes.clear();
}

void ObjectTypeManager::Print()
{
  printf("Total object types: %lu\n", objectTypes.size());
  for (auto *objectType : objectTypes)
  {
    objectType->Print();
  }
}

void ObjectTypeManager::LoadFromFile(std::string filename)
{
  enum LineType { OBJ_TYPE_DEF, OBJ_ACTION_DEF, OBJ_FRAME_DEF };

  std::ifstream infile(filename);
  std::string line;
  ObjectType *currentObjectType;
  ObjectAction *currentObjectTypeAction;
  int currentObjectTypeId, currentObjectTypeActionId;

  while (std::getline(infile, line))
  {
      std::istringstream iss(line);
      string token;
      bool commentFound = false;
      std::vector<string> *currentframeValues = new std::vector<string>;
      LineType currentLineType;

      while((iss >> token) && (!commentFound)) {
        if(startsWith(token, "//")) {
          commentFound = true;
        }
        else if(startsWith(token, "##")) {
          currentLineType = OBJ_TYPE_DEF;
          int objectTypeId = std::stoi(token.substr(2));
          currentObjectType = new ObjectType(objectTypeId);
          objectTypes.push_back(currentObjectType);
        } else if(startsWith(token, "#")) {
          currentLineType = OBJ_ACTION_DEF;
          int objectTypeActionId = std::stoi(token.substr(1));
          currentObjectTypeAction = new ObjectAction(objectTypeActionId);
          currentObjectType->AddAction(currentObjectTypeAction);
        } else {
          currentLineType = OBJ_FRAME_DEF;
          currentframeValues->push_back(token);
        }
      }

      if(currentLineType == OBJ_FRAME_DEF) {
        if(currentframeValues->size() == 6) {
          uint16 width = stoul(currentframeValues->at(0));
          uint16 height = stoul(currentframeValues->at(1));
          float u1 = stof(currentframeValues->at(2));
          float v1 = stof(currentframeValues->at(3));
          float u2 = stof(currentframeValues->at(4));
          float v2 = stof(currentframeValues->at(5));
          currentObjectTypeAction->AddFrame(new ObjectFrame(width, height, u1, v1, u2, v2));
        }
      }
  }
}
