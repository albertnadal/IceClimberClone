#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <optional>
#include <entity_factory.h>
#include <entity_data_manager.h>
#include <sprite_rect_double_buffer.h>
#include <AABB/AABB.h>

class EntityManager
{
  aabb::Tree<IEntity*> *spacePartitionObjectsTree = nullptr; // Used for of object collision detection
  std::map<uint32_t, IEntity*> mobileObjects;
  std::map<uint32_t, IEntity*> staticObjects;
  std::vector<IEntity*> objectsToDelete;
  IEntity* player = nullptr;
  uint32_t currentRow;
  uint32_t visibleRows;

  EntityDataManager *textureManager;
  SpriteRectDoubleBuffer *spriteRectDoubleBuffer;
  uint32_t maxObjects;
  uint32_t currentEscalatedHeight;
  void BuildMountain();
  bool cameraIsMoving;
  float totalPixelDisplacement;
  float newCameraPosition;
  float currentCameraPosition;
  std::vector<int> validAltitudes = {84, 78, 72, 66, 60}; // Altitudes of levels 4, 5, 6, 7 and 8. Are multiple of six.

  // Each level is six cells height. The entire mountain have 4 extra rows on top to enforce level floors to be
  // located in vertical position multiple of six. One additional row is appended to show the water.
  const uint16_t mountainMap[4 + 17*6 + 1][32] =
  {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 29, 29, 29, 29, 29, 29, 29, 0, 0, 0, 0, 0, 0, 0, 29, 29, 29, 29, 29, 29, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 40, 0, 0, 45, 45, 45, 46, 0, 0, 0, 0, 47, 45, 0, 46, 0, 47, 40, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 39, 0, 0, 0, 47, 47, 47, 45, 0, 0, 0, 0, 0, 0, 45, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 47, 45, 0, 46, 45, 45, 45, 46, 45, 45, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0 },

    { 0, 0, 0, 0, 0, 39, 0, 0, 47, 0, 45, 46, 45, 45, 0, 0, 45, 46, 45, 45, 45, 45, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 40, 0, 0, 0, 47, 47, 47, 45, 0, 29, 29, 29, 29, 0, 0, 0, 0, 47, 47, 40, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 39, 0, 0, 47, 45, 0, 46, 0, 0, 0, 0, 0, 0, 47, 47, 47, 45, 0, 46, 39, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 40, 0, 0, 47, 47, 47, 0, 0, 47, 47, 47, 45, 0, 0, 0, 0, 46, 46, 0, 40, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 39, 0, 0, 0, 0, 0, 0, 29, 29, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 40, 0, 0, 45, 46, 45, 45, 45, 45, 0, 0, 0, 0, 45, 45, 46, 45, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0 },

    { 0, 0, 0, 0, 0, 39, 0, 0, 46, 0, 0, 46, 47, 47, 0, 46, 0, 0, 0, 29, 29, 29, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 40, 0, 0, 45, 47, 47, 47, 45, 0, 0, 0, 0, 0, 0, 45, 45, 45, 0, 46, 46, 45, 40, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 39, 0, 0, 46, 47, 47, 0, 46, 0, 46, 47, 47, 0, 46, 0, 0, 0, 46, 47, 0, 0, 39, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 40, 0, 0, 0, 29, 29, 29, 0, 0, 0, 0, 0, 45, 45, 46, 45, 0, 0, 47, 47, 47, 40, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 39, 0, 0, 0, 45, 45, 46, 45, 0, 0, 0, 0, 29, 29, 29, 29, 0, 46, 46, 0, 0, 39, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 40, 0, 0, 0, 45, 45, 0, 45, 45, 0, 0, 0, 47, 45, 0, 46, 0, 0, 47, 47, 0, 40, 0, 0, 0, 0, 0, 0 },

    { 0, 0, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 45, 0, 46, 0, 0, 46, 0, 0, 0, 39, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 40, 0, 0, 0, 45, 45, 46, 45, 47, 47, 47, 45, 0, 0, 0, 0, 45, 47, 47, 0, 0, 40, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0, 45, 46, 45, 45, 0, 0, 0, 47, 47, 47, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0, 0 },
    { 37, 0, 0, 40, 0, 0, 0, 45, 45, 46, 45, 0, 47, 45, 0, 46, 0, 0, 0, 47, 46, 46, 0, 0, 0, 45, 40, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 39, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 40, 0, 0, 0, 0, 47, 46, 46, 0, 0, 0, 47, 47, 47, 45, 0, 0, 0, 46, 46, 46, 0, 0, 40, 0, 0, 0, 0, 0 },

    { 0, 0, 0, 39, 0, 0, 45, 0, 0, 0, 47, 47, 48, 0, 46, 45, 46, 46, 0, 0, 45, 47, 47, 0, 45, 0, 39, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 40, 0, 0, 0, 45, 45, 46, 45, 0, 45, 0, 0, 0, 0, 0, 45, 45, 46, 45, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 39, 0, 0, 0, 0, 45, 0, 46, 0, 29, 29, 29, 29, 29, 29, 0, 0, 0, 0, 47, 47, 0, 45, 39, 0, 0, 0, 0, 0 },
    { 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 45, 0, 46, 0, 0, 0, 0, 0, 29, 29, 29, 0, 0, 0, 40, 0, 0, 0, 0 },
    { 0, 0, 39, 0, 0, 0, 0, 47, 47, 47, 45, 0, 0, 45, 0, 0, 0, 47, 47, 0, 0, 0, 45, 46, 45, 45, 0, 39, 0, 0, 0, 0 },
    { 0, 0, 40, 0, 0, 45, 46, 45, 45, 0, 0, 0, 0, 0, 0, 0, 45, 45, 46, 45, 0, 0, 45, 45, 0, 0, 0, 40, 0, 0, 0, 0 },

    { 0, 0, 39, 0, 0, 0, 0, 0, 47, 47, 0, 46, 46, 0, 0, 47, 46, 46, 0, 47, 48, 0, 46, 46, 0, 0, 0, 39, 0, 0, 0, 0 },
    { 0, 0, 40, 0, 0, 29, 29, 29, 29, 29, 29, 29, 0, 45, 46, 45, 45, 46, 45, 45, 47, 0, 45, 46, 45, 45, 0, 40, 0, 0, 0, 0 },
    { 0, 0, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 29, 29, 29, 0, 0, 0, 0, 0, 39, 0, 0, 0, 0 },
    { 0, 0, 40, 0, 0, 0, 46, 47, 47, 0, 46, 0, 0, 0, 0, 0, 45, 0, 0, 0, 47, 47, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0 },
    { 0, 39, 0, 0, 0, 0, 0, 0, 47, 47, 0, 46, 46, 0, 0, 0, 0, 47, 47, 47, 45, 0, 0, 45, 46, 45, 45, 0, 39, 0, 0, 0 },
    { 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 44, 0, 0, 0, 0, 47, 46, 46, 0, 0, 0, 45, 45, 46, 45, 0, 0, 0, 40, 0, 0, 0 },

    { 0, 39, 0, 0, 0, 0, 0, 0, 0, 47, 47, 0, 46, 0, 0, 0, 0, 45, 46, 45, 45, 46, 45, 45, 0, 0, 46, 0, 39, 0, 0, 0 },
    { 37, 40, 0, 0, 0, 47, 47, 47, 45, 0, 0, 0, 0, 0, 0, 0, 47, 47, 0, 46, 0, 0, 0, 45, 45, 46, 45, 0, 40, 0, 0, 0 },
    { 0, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 47, 0, 46, 46, 0, 45, 0, 0, 0, 47, 47, 0, 0, 39, 0, 0, 0 },
    { 0, 40, 0, 0, 46, 47, 47, 0, 46, 0, 0, 0, 47, 47, 0, 46, 46, 0, 0, 45, 45, 46, 45, 0, 0, 0, 0, 0, 40, 0, 0, 0 },
    { 0, 39, 0, 0, 0, 0, 0, 47, 46, 46, 0, 0, 45, 45, 46, 45, 0, 0, 47, 47, 47, 45, 0, 0, 0, 0, 0, 0, 39, 0, 0, 0 },
    { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 47, 0, 46, 0, 0, 0, 0, 0, 47, 46, 46, 48, 0, 0, 0, 0, 40, 0, 0 },

    { 39, 0, 0, 0, 47, 46, 46, 0, 0, 0, 0, 0, 45, 0, 0, 0, 47, 47, 0, 47, 47, 0, 46, 0, 0, 0, 0, 0, 0, 39, 0, 0 },
    { 40, 0, 0, 0, 0, 0, 29, 29, 29, 29, 0, 0, 0, 29, 29, 29, 29, 29, 29, 0, 0, 0, 29, 29, 29, 29, 0, 0, 0, 40, 0, 0 },
    { 39, 0, 0, 0, 0, 47, 0, 46, 45, 0, 0, 0, 47, 47, 47, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 0, 0 },
    { 40, 0, 0, 45, 46, 0, 47, 46, 47, 47, 0, 46, 45, 45, 46, 45, 45, 46, 0, 47, 0, 0, 47, 46, 46, 0, 45, 45, 0, 40, 0, 0 },
    { 39, 0, 0, 0, 45, 0, 46, 45, 0, 47, 47, 47, 45, 0, 46, 0, 0, 46, 45, 45, 0, 45, 47, 47, 0, 46, 46, 0, 47, 39, 0, 0 },
    { 29, 29, 29, 29, 29, 29, 29, 45, 46, 29, 29, 29, 29, 29, 29, 0, 46, 29, 29, 29, 29, 29, 29, 0, 0, 29, 29, 29, 29, 29, 29, 29 },

    { 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 29, 29, 29, 29, 29, 29, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 29, 29, 29, 29, 29, 29 },

    { 17, 0, 0, 0, 0, 0, 7, 0, 0, 7, 4, 4, 4, 4, 7, 0, 0, 7, 4, 7, 0, 0, 0, 7, 4, 4, 18, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 29, 29, 29, 29, 29, 29, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 29, 29, 29, 29, 29, 29 },

    { 17, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 7, 4, 4, 7, 0, 0, 7, 4, 4, 4, 4, 4, 7, 0, 0, 18, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 29, 29, 29, 29, 29, 29, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 29, 29, 29, 29, 29, 29 },

    { 15, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 7, 0, 0, 7, 4, 4, 4, 4, 4, 4, 4, 7, 0, 16, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0 },
    { 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 28, 28, 28, 28, 28, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 28, 28, 28, 28, 28 },

    { 15, 0, 0, 0, 0, 6, 0, 6, 3, 3, 3, 3, 3, 3, 6, 0, 0, 6, 3, 3, 3, 3, 6, 0, 6, 3, 3, 16, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66 },
    { 0, 0, 61, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 61, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 28, 28, 28, 28, 28, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 28, 28, 28, 28, 28 },

    { 15, 0, 0, 0, 0, 3, 6, 0, 0, 6, 3, 6, 0, 6, 3, 3, 3, 6, 0, 0, 0, 6, 3, 3, 3, 3, 3, 16, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 28, 28, 28, 28, 28, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 28, 28, 28, 28, 28 },

    { 9, 0, 0, 0, 0, 6, 3, 3, 3, 6, 0, 0, 0, 0, 0, 6, 3, 3, 3, 3, 3, 6, 0, 6, 3, 3, 3, 0, 10, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 59, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 27, 27, 27, 27, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 27, 27, 27, 27 },

    { 9, 0, 0, 0, 0, 0, 0, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 0, 5, 2, 2, 5, 0, 0, 5, 2, 5, 10, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 58, 0 },
    { 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 27, 27, 27, 27, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 27, 27, 27, 27 },

    { 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43 }
  };

  void deleteUneededObjects();
  void updateEntities(std::map<uint32_t, IEntity*>&, std::optional<uint8_t>);
  void updateMobileObjects(uint8_t);
  void updateStaticObjects();
  void updateSpriteRectBuffers();
public:
  EntityManager(EntityDataManager*, SpriteRectDoubleBuffer*, uint32_t);
  ~EntityManager();
  std::optional<float> Update(uint8_t);
  std::optional<IEntity *> CreateEntityWithId(EntityIdentificator, int , int);
  void PlayerReachedNewAltitude(int);
  void PlayerEnteredBonusStage();
};

#endif
