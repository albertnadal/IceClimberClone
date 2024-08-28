#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include <optional>
#include <entity_factory.h>
#include <entities/player.h>
#include <sound_manager.h>
#include <entity_data_manager.h>
#include <sprite_rect_double_buffer.h>
#include <AABB/AABB.h>
#include <utils.h>

class Player;

struct UpdateInfo { std::optional<float> currentCameraVerticalPosition; int lifeCounter; bool gameFinished; };
struct GameScoreSummary { EntityIdentificator vegetableId = EntityIdentificator::NONE; bool condorHunted = false; int vegetableCount = 0, nitpickerCount = 0, iceCount = 0, brickCount = 0; int condorUnitScore = 0, vegetableUnitScore = 0, nitpickerUnitScore = 0, iceUnitScore = 0, brickUnitScore = 0; };

class GameManager
{
  aabb::Tree<IEntity*> *spacePartitionObjectsTree = nullptr; // Used for of object collision detection
  std::map<uint32_t, IEntity*> mobileObjects;
  std::map<uint32_t, IEntity*> staticObjects;
  std::vector<IEntity*> objectsToDelete;
  Player* player = nullptr;
  int lifeCounter = MAX_PLAYER_LIFES;
  bool isGameFinished = false;
  bool isGameOver = false;
  GameScoreSummary scoreSummary;
  SoundManager *soundManager;
  EntityDataManager *textureManager;
  SpriteRectDoubleBuffer *spriteRectDoubleBuffer;
  uint32_t maxObjects;
  void LoadMountainFromFile(const std::string&);
  float newCameraVerticalPosition;
  float currentCameraVerticalPosition;
  std::vector<int> validAltitudes = {84, 78, 72, 66, 60}; // Altitudes of levels 4, 5, 6, 7 and 8. Are multiple of six.
  bool playerEnteredBonusStage;
  void deleteUneededObjects();
  void deleteAllObjects();
  void updateEntities(std::map<uint32_t, IEntity*>&, std::optional<uint8_t>);
  void updateMobileObjects(uint8_t);
  void updateStaticObjects();
  void updateSpriteRectBuffers();
public:
  GameManager(SoundManager*, EntityDataManager*, SpriteRectDoubleBuffer*, uint32_t);
  ~GameManager();
  void SetupMountain(int);
  UpdateInfo Update(uint8_t);
  std::optional<IEntity *> CreateEntityWithId(EntityIdentificator, int , int);
  float GetCurrentCameraVerticalPosition() const;
  std::optional<Position *> GetPlayerPosition() const;
  GameScoreSummary GetGameScoreSummary() const;
  bool IsGameOver() const;
  void PlaySoundById(SoundIdentificator) const;
  std::optional<float> PlayerReachedNewAltitude(int);
  void PlayerEnteredBonusStage();
  void PlayerCompletedMountain(bool, int, int, int, int);
  bool PlayerHasLostALife(int, int, int, int);
  void FinishGame(bool);
};

#endif
