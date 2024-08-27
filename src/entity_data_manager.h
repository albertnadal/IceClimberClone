#ifndef ENTITY_DATA_MANAGER_H
#define ENTITY_DATA_MANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <entity_sprite_sheet.h>
#include <raylib/raylib.h>
#include <filesystem.h>

using namespace std;

class EntityDataManager
{
  typedef map<EntityIdentificator, EntitySpriteSheet*> SpriteSheetsMap;
  SpriteSheetsMap entitySpriteSheetsMap;
  std::string textureFilename;
  void LoadObjectsDataFromFile(std::string filename);
public:
  EntityDataManager();
  ~EntityDataManager();
  Texture2D LoadTextureAtlas() const;
  std::optional<EntitySpriteSheet*> GetSpriteSheetByEntityIdentificator(EntityIdentificator) const;
};

bool startsWith(std::string mainStr, std::string toMatch);
#endif
