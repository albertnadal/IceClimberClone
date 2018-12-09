#include "object_manager.h"
#include <fstream>
#include <sstream>
#include <utils.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager() {
  for (auto *objectType : objectTypes)
  {
    delete objectType;
  }

  objectTypes.clear();
}

void ObjectManager::Print()
{
  printf("Texture filename: %s\n", textureFilename.c_str());
  printf("Total object types: %lu\n", objectTypes.size());
  for (auto *objectType : objectTypes)
  {
    objectType->Print();
  }
}

void ObjectManager::LoadObjectsDataFromFile(std::string filename)
{
  enum LineType { OBJ_TYPE_TEX_FILENAME, OBJ_TYPE_DEF, OBJ_ACTION_DEF, OBJ_FRAME_DEF };

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
        else if(startsWith(token, "###")) {
          currentLineType = OBJ_TYPE_TEX_FILENAME;
          textureFilename = token.substr(3);
        } else if(startsWith(token, "##")) {
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

unsigned int ObjectManager::LoadObjectsSpritesToGPU() {
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(FileSystem::getPath(textureFilename).c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
          std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return textureId;
}


bool startsWith(std::string mainStr, std::string toMatch)
{
	// Convert mainStr to lower case
	std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
	// Convert toMatch to lower case
	std::transform(toMatch.begin(), toMatch.end(), toMatch.begin(), ::tolower);

	return (mainStr.find(toMatch) == 0);
}
