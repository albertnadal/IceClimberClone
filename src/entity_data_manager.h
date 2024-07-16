#ifndef SCENE_ENTITY_DATA_MANAGER_H
#define SCENE_ENTITY_DATA_MANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <entity_sprite_sheet.h>
#include <raylib/raylib.h>
#include <filesystem.h>

#define ENTITY_TYPES_FILENAME "objtypes.dat"

using namespace std;

class EntityDataManager
{
  typedef map<EntityIdentificator, EntitySpriteSheet*> SpriteSheetsMap;
  SpriteSheetsMap entitySpriteSheetsMap;
  std::string textureFilename;
  uint32_t textureId;
  void LoadObjectsDataFromFile(std::string filename);
  void Print();
public:
  EntityDataManager();
  ~EntityDataManager();
  Texture2D LoadTextureAtlas();
  std::optional<EntitySpriteSheet*> GetSpriteSheetByEntityIdentificator(EntityIdentificator);
};

bool startsWith(std::string mainStr, std::string toMatch);
#endif
