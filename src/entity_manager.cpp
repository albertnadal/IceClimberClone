#include <entity_manager.h>
#include <entity_factory.h>
#include <entity.h>
#include <fstream>
#include <sstream>

EntityManager::EntityManager(EntityDataManager* _textureManager, SpriteRectDoubleBuffer* _spriteRectDoubleBuffer, uint32_t _maxObjects) {
        textureManager = _textureManager;
        spriteRectDoubleBuffer = _spriteRectDoubleBuffer;
        maxObjects = _maxObjects;
        spacePartitionObjectsTree = new aabb::Tree<IEntity*>();
        spacePartitionObjectsTree->setDimension(2);
        currentCameraVerticalPosition = newCameraVerticalPosition = 0.0f;
        playerEnteredBonusStage = false;
        isGameFinished = false;
        isGameOver = false;
}

void EntityManager::LoadMountainFromFile(const std::string& filename) {
  std::ifstream file(filename);
  assert(file.is_open() && "Error: Unable to open file with mountain data.");

  // Load mountain configuration
  std::string line;
  std::getline(file, line); // Vegetable Id
  Utils::removeCommentAndTrim(line);
  scoreSummary.vegetableId = (EntityIdentificator)(std::stoi(line));

  std::getline(file, line); // Condor unit score
  Utils::removeCommentAndTrim(line);
  scoreSummary.condorUnitScore = std::stoi(line);

  std::getline(file, line); // Vegetable unit score
  Utils::removeCommentAndTrim(line);
  scoreSummary.vegetableUnitScore = std::stoi(line);

  std::getline(file, line); // Ice block unit score
  Utils::removeCommentAndTrim(line);
  scoreSummary.iceUnitScore = std::stoi(line);

  std::getline(file, line); // Nitpicker unit score
  Utils::removeCommentAndTrim(line);
  scoreSummary.nitpickerUnitScore = std::stoi(line);

  std::getline(file, line); // Brick unit score
  Utils::removeCommentAndTrim(line);
  scoreSummary.brickUnitScore = std::stoi(line);

  // Load mountain objects
  int row = 0;
  while (std::getline(file, line) && row < MOUNTAIN_HEIGHT_CELLS) {
      std::stringstream ss(line);
      std::string cell;
      int col = 0;

      while (std::getline(ss, cell, ',') && col < MOUNTAIN_WIDTH_CELLS) {
          CreateEntityWithId((EntityIdentificator)(std::stoi(cell)), col, row);
          ++col;
      }

      ++row;
  }

  file.close();
}

std::optional<IEntity *> EntityManager::CreateEntityWithId(EntityIdentificator entity_id, int x, int y) {
  std::optional<IEntity *> entity_ptr = EntityFactory::Get(this, textureManager, spacePartitionObjectsTree)->CreateEntity(entity_id);

  if(entity_ptr.has_value()) {
    if (entity_id == EntityIdentificator::POPO) {
      player = static_cast<Player*>(*entity_ptr);
      currentCameraVerticalPosition = newCameraVerticalPosition = INITIAL_CAMERA_POSITION;
    }

    // Set the initial position of the object in the screen
    (*entity_ptr)->position.setInitialXY(x*CELL_WIDTH_FLOAT, y*CELL_HEIGHT_FLOAT);

    // Initial update to load the sprites and boundary box
    (*entity_ptr)->Update();

    // Insert the object into the space partition tree used for object collision detection
    std::vector<int> lowerBound = (*entity_ptr)->GetLowerBound();
    std::vector<int> upperBound = (*entity_ptr)->GetUpperBound();
    spacePartitionObjectsTree->insertParticle(*entity_ptr, lowerBound, upperBound);

    // Save pointers to proper arrays for static objects and mobile objects
    if(((*entity_ptr)->Type() == EntityType::TERRAIN) || ((*entity_ptr)->Type() == EntityType::CLOUD) || ((*entity_ptr)->Type() == EntityType::NUMBER)) {
      staticObjects[(*entity_ptr)->uniqueId] = *entity_ptr;
    } else {
      mobileObjects[(*entity_ptr)->uniqueId] = *entity_ptr;
    }
  }

  return entity_ptr;
}

std::optional<float> EntityManager::PlayerReachedNewAltitude(int cellY) {
  if ((std::find(validAltitudes.begin(), validAltitudes.end(), cellY) != validAltitudes.end()) || (cellY <= BONUS_STAGE_CELL_Y)) {
    float padding_top = (cellY != BONUS_STAGE_CELL_Y) ? CAMERA_PADDING_TOP : CAMERA_BONUS_STAGE_PADDING_TOP;
    newCameraVerticalPosition = cellY*CELL_HEIGHT_FLOAT - padding_top;

    // Return the current bottom vertical position of the mountain viewport
    return newCameraVerticalPosition + (MOUNTAIN_VIEWPORT_HEIGHT_CELLS*CELL_HEIGHT_FLOAT);
  }

  return std::nullopt;
}

float EntityManager::GetCurrentCameraVerticalPosition() const {
  return currentCameraVerticalPosition <= 1.0f ? INITIAL_CAMERA_POSITION : currentCameraVerticalPosition;
}

std::optional<Position *> EntityManager::GetPlayerPosition() const {
  if (player == nullptr) {
    return std::nullopt;
  }

  return &(player->position);
}

GameScoreSummary EntityManager::GetGameScoreSummary() const {
  return scoreSummary;
}

bool EntityManager::IsGameOver() const {
  return isGameOver;
}

void EntityManager::PlayerEnteredBonusStage() {
  // Delete all mobile objects below the bonus stage (except the player).
  for (auto const& x : mobileObjects) {
    IEntity* entity_ptr = x.second;
    if ((entity_ptr != player) && (entity_ptr->type != EntityType::VEGETABLE) && (entity_ptr->position.GetInitialCellY() > BONUS_STAGE_CELL_Y)) {
      entity_ptr->isMarkedToDelete = true;
    }
  }

  playerEnteredBonusStage = true;
}

void EntityManager::PlayerCompletedMountain(bool condorHunted, int vegetableCount, int nitpickerCount, int iceCount, int brickCount) {
  scoreSummary.condorHunted = condorHunted;
  scoreSummary.vegetableCount = vegetableCount;
  scoreSummary.nitpickerCount = nitpickerCount;
  scoreSummary.iceCount = iceCount;
  scoreSummary.brickCount = brickCount;
  isGameFinished = true;
}

bool EntityManager::PlayerHasLostALife(int vegetableCount, int nitpickerCount, int iceCount, int brickCount) {
  scoreSummary.vegetableCount = vegetableCount;
  scoreSummary.nitpickerCount = nitpickerCount;
  scoreSummary.iceCount = iceCount;
  scoreSummary.brickCount = brickCount;

  if (lifeCounter <= 0) {
      isGameOver = true;

      // Show the Game Over message moving from player vertical possition to top.
      CreateEntityWithId(EntityIdentificator::GAME_OVER, 4, player->position.GetCellY());
  }

  lifeCounter--;
  return isGameOver;
}

void EntityManager::FinishGame() {
  isGameFinished = true;
}

void EntityManager::SetupMountain(int mountainNumber) {
    // Delete old object instances created in the previous game
    deleteAllObjects();

    // Reset values
    currentCameraVerticalPosition = newCameraVerticalPosition = 0.0f;
    playerEnteredBonusStage = false;
    isGameFinished = false;
    isGameOver = false;
    lifeCounter = MAX_PLAYER_LIFES;

    // Reset player achievements
    scoreSummary.condorHunted = false;
    scoreSummary.vegetableCount = 0;
    scoreSummary.nitpickerCount = 0;
    scoreSummary.iceCount = 0;
    scoreSummary.brickCount = 0;

    // Load the data of the requested mountain
    std::string filename;
    Utils::getMountainFilename(mountainNumber, filename);
    LoadMountainFromFile(filename);
}

UpdateInfo EntityManager::Update(uint8_t pressedKeys) {
  updateMobileObjects(pressedKeys);
  updateStaticObjects();
  updateSpriteRectBuffers();
  deleteUneededObjects();

  UpdateInfo info;

  // Update vertical camera position when player reaches new level height
  if (newCameraVerticalPosition < currentCameraVerticalPosition) {
    currentCameraVerticalPosition -= CAMERA_SPEED; // Progressive update to get an smooth transition
    info.currentCameraVerticalPosition = currentCameraVerticalPosition;
  }

  // Do not provide the life counter if the player is in the bonus stage
  if (!playerEnteredBonusStage) {
    info.lifeCounter = lifeCounter;
  }

  info.gameFinished = isGameFinished;

  return info;
}

void EntityManager::updateSpriteRectBuffers() {
  int i = 0;
  std::vector<aabb::AABBIntersection<IEntity*>> objectIntersections = spacePartitionObjectsTree->query(player->GetLowerBound(), player->GetUpperBound());

  for (auto const& x : staticObjects) {
    IEntity* entity_ptr = x.second;
    Rectangle src = { entity_ptr->currentSprite.u1, entity_ptr->currentSprite.v1, entity_ptr->currentSprite.u2, entity_ptr->currentSprite.v2 };
    Vector2 pos = { entity_ptr->position.GetX(), entity_ptr->position.GetY() };
    Boundaries boundaries = entity_ptr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = entity_ptr->GetAbsoluteSolidBoundaries();
    Boundaries attackBoundaries = entity_ptr->GetAbsoluteAttackBoundaries().value_or(Boundaries{0,0,0,0});
    Color tint = WHITE;

    // Tint in RED those objects that are candidates to collide with the player object.
    auto it = std::find_if(objectIntersections.begin(), objectIntersections.end(),
                           [entity_ptr](const aabb::AABBIntersection<IEntity*>& intersection) {
                               return intersection.particle == entity_ptr;
                           });

    if (it != objectIntersections.end()) {
      tint = RED;
    }

    spriteRectDoubleBuffer->producer_buffer[i++] = SpriteRect(src, pos, boundaries, attackBoundaries, tint);
  }

  for (auto const& x : mobileObjects) {
    IEntity* entity_ptr = x.second;
    Rectangle src = { entity_ptr->currentSprite.u1, entity_ptr->currentSprite.v1, entity_ptr->currentSprite.u2, entity_ptr->currentSprite.v2 };
    Vector2 pos = { entity_ptr->position.GetX(), entity_ptr->position.GetY() };
    Boundaries boundaries = entity_ptr->GetAbsoluteBoundaries();
    Boundaries solidBoundaries = entity_ptr->GetAbsoluteSolidBoundaries();
    Boundaries attackBoundaries = entity_ptr->GetAbsoluteAttackBoundaries().value_or(Boundaries{0,0,0,0});
    spriteRectDoubleBuffer->producer_buffer[i] = SpriteRect(src, pos, boundaries, attackBoundaries, WHITE);
    i++;
  }

  spriteRectDoubleBuffer->producer_buffer_length = i;
  spriteRectDoubleBuffer->swapBuffers();
}

void EntityManager::updateEntities(std::map<uint32_t, IEntity*>& objects, std::optional<uint8_t> pressedKeys = std::nullopt) {
    for (auto const& x : objects) {
        IEntity* entity_ptr = x.second;

        if (entity_ptr->isMarkedToDelete) {
            objectsToDelete.push_back(entity_ptr);
            continue;
        }

        if (pressedKeys.has_value()) {
            entity_ptr->Update(pressedKeys.value());
        } else {
            entity_ptr->Update();
        }
    }
}

void EntityManager::updateMobileObjects(uint8_t pressedKeys) {
    updateEntities(mobileObjects, pressedKeys);
}

void EntityManager::updateStaticObjects() {
    updateEntities(staticObjects);
}

void EntityManager::deleteUneededObjects() {
  for (auto entity_ptr : objectsToDelete) {
    staticObjects.erase(entity_ptr->uniqueId);
    mobileObjects.erase(entity_ptr->uniqueId);

    // Objects are responsible for removing themselves from the space partition tree, so the
    // following code is just for safety.
    spacePartitionObjectsTree->removeParticle(entity_ptr);

    delete entity_ptr;
  }

  objectsToDelete.clear();
}

void EntityManager::deleteAllObjects() {
  // Mark the Player instance to delete.
  if (player != nullptr) {
    player->isMarkedToDelete = true;
    player = nullptr;
  }

  // Delete all instances marked to delete.
  deleteUneededObjects();

  // Delete mobile object instances.
  for (auto& pair : mobileObjects) {
    spacePartitionObjectsTree->removeParticle(pair.second);
    delete pair.second;
  }
  mobileObjects.clear();

  // Delete static object instances.
  for (auto& pair : staticObjects) {
    spacePartitionObjectsTree->removeParticle(pair.second);
    delete pair.second;
  }
  staticObjects.clear();
}

EntityManager::~EntityManager() {
  deleteAllObjects();

  if(spacePartitionObjectsTree != nullptr) {
    delete spacePartitionObjectsTree;
  }
}
