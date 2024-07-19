#include <entity.h>
#include <collision/collision.h>
#include <MersenneTwister/MersenneTwister.h>

IEntity::IEntity() {
  id = EntityIdentificator::NONE;
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = solidBoundingBox = {0, 0, 0, 0};
  recalculateAreasDataIsNeeded = true;
  isBreakable = false;
  isTraversable = false;
  isMarkedToDelete = false;
}

IEntity::IEntity(EntityIdentificator _id, EntityType _type, SurfaceType surface_type, unsigned char _maxStates, bool _isBreakable, bool _isTraversable) :
  StateMachine(_maxStates),
  id(_id),
  type(_type),
  surfaceType(surface_type),
  isBreakable(_isBreakable),
  isTraversable(_isTraversable) {
  MersenneTwister rng;
  uniqueId = rng.integer(0, UINT_MAX);
  boundingBox = solidBoundingBox = {0, 0, 0, 0};
  recalculateAreasDataIsNeeded = true;
}

void IEntity::SetSpacePartitionObjectsTree(aabb::Tree<IEntity*> *_spacePartitionObjectsTree) {
  spacePartitionObjectsTree = _spacePartitionObjectsTree;
}

/*
std::vector<Area>& IEntity::GetSolidAreas() {
  if(recalculateAreasDataIsNeeded) {
    // Updates the object solid areas values according to the current object position and current sprite areas
    solidAreas.clear();
    if(currentSprite.areas!=nullptr) {
      for(uint16_t i=0; i<currentSprite.areas->solidAreas.size(); i++) {
        std::vector<collision::vec2<float>> updatedPoints;
        std::vector<collision::vec2<float>> &currentPoints = currentSprite.areas->solidAreas.at(i).rectangle.vertices;
        for(uint16_t j=0; j<currentPoints.size(); j++) {
          // Apply the current position to the current area point
          updatedPoints.push_back(collision::vec2<float>(currentPoints.at(j).x + position.GetX(), currentPoints.at(j).y + position.GetY()));
        }
        collision::Rectangle updatedRectangle(updatedPoints);
        solidAreas.push_back({currentSprite.areas->solidAreas.at(i).id, updatedRectangle});
      }
    }
    recalculateAreasDataIsNeeded = false;
  }
  return solidAreas;
}
*/
/*
std::vector<Area>& IEntity::GetSimpleAreas() {

}
*/

void IEntity::PositionSetXY(float x, float y) {
    position.setXY(x, y);
    recalculateAreasDataIsNeeded = true;
}

void IEntity::PositionSetX(float x) {
  position.setX(x);
  recalculateAreasDataIsNeeded = true;
}

void IEntity::PositionSetY(float y) {
  position.setY(y);
  recalculateAreasDataIsNeeded = true;
}

void IEntity::PositionAddX(float x) {
  position.addX(x);
  recalculateAreasDataIsNeeded = true;
}

void IEntity::PositionAddY(float y) {
  position.addY(y);
  recalculateAreasDataIsNeeded = true;
}

void IEntity::PositionSetOffset(int16_t x, int16_t y) {
  position.setOffset(x, y);
  recalculateAreasDataIsNeeded = true;
}

void IEntity::RemoveFromSpacePartitionObjectsTree() {
  spacePartitionObjectsTree->removeParticle(this);
}

void IEntity::LoadAnimationWithId(uint16_t animationId) {
    std::optional<EntitySpriteSheetAnimation *> currentAnimation = spriteSheet->GetAnimationWithId(animationId);
    assert(currentAnimation != std::nullopt);
    currentAnimationSprites = (*currentAnimation)->GetSprites();
    animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
    currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
    animationLoaded = true;
    firstSpriteOfCurrentAnimationIsLoaded = false;
    nextSpriteTime = std::chrono::system_clock::now();
}

void IEntity::LoadNextSprite()
{
  SpriteData spriteData = NextSpriteData();
  if(spriteData.beginNewLoop) {
          if(ShouldBeginAnimationLoopAgain()) {
            spriteData = NextSpriteData();
          }
  }

  nextSpriteTime = (chrono::system_clock::now() + std::chrono::milliseconds(spriteData.duration));

  currentSprite.width = spriteData.width;
  currentSprite.height = spriteData.height;
  currentSprite.xOffset = spriteData.xOffset;
  currentSprite.yOffset = spriteData.yOffset;
  currentSprite.u1 = spriteData.u1;
  currentSprite.v1 = spriteData.v1;
  currentSprite.u2 = spriteData.u2;
  currentSprite.v2 = spriteData.v2;
  currentSprite.areas = spriteData.areas;
  recalculateAreasDataIsNeeded = true; // Is necessary because the current sprite may have different areas
  boundingBox = { spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY };
  firstSpriteOfCurrentAnimationIsLoaded = true;
}

SpriteData IEntity::NextSpriteData() {
    if (currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        (*currentAnimationSpriteIterator).beginNewLoop = true;
    }

    return *currentAnimationSpriteIterator++;
}

bool IEntity::ShouldBeginAnimationLoopAgain()
{
    return false;
}

std::vector<int> IEntity::GetLowerBound() {
  std::vector<int> lowerBound{position.GetIntX() + boundingBox.lowerBoundX, position.GetIntY() + boundingBox.lowerBoundY};
  return lowerBound;
}

std::vector<int> IEntity::GetUpperBound() {
  std::vector<int> upperBound{position.GetIntX() + boundingBox.upperBoundX, position.GetIntY() + boundingBox.upperBoundY};
  return upperBound;
}

std::vector<int> IEntity::GetSolidLowerBound() {
  std::vector<int> lowerBound{position.GetIntX() + solidBoundingBox.lowerBoundX, position.GetIntY() + solidBoundingBox.lowerBoundY};
  return lowerBound;
}

std::vector<int> IEntity::GetSolidUpperBound() {
  std::vector<int> upperBound{position.GetIntX() + solidBoundingBox.upperBoundX, position.GetIntY() + solidBoundingBox.upperBoundY};
  return upperBound;
}

Boundaries IEntity::GetAbsoluteBoundaries() {
  return {position.GetIntX() + boundingBox.upperBoundX,
          position.GetIntY() + boundingBox.upperBoundY,
          position.GetIntX() + boundingBox.lowerBoundX,
          position.GetIntY() + boundingBox.lowerBoundY};
}

Boundaries IEntity::GetAbsoluteSolidBoundaries() {
  return {position.GetIntX() + boundingBox.upperBoundX,
          position.GetIntY() + boundingBox.upperBoundY,
          position.GetIntX() + boundingBox.lowerBoundX,
          position.GetIntY() + boundingBox.lowerBoundY};
}

EntityIdentificator IEntity::Id() {
  return id;
}

EntityType IEntity::Type() {
  return type;
}

int IEntity::Width() {
  return currentSprite.width;
}

int IEntity::Height() {
  return currentSprite.height;
}

void IEntity::PrintName() {
  std::cout << "PrintName not overloaded for object." << std::endl;
}

bool IEntity::IsCloud() {
  return false;
}

void IEntity::PrintBoundaries() {
  std::vector<int> lowerBound = GetLowerBound();
  std::vector<int> upperBound = GetUpperBound();
  std:cout << "Lowerbound X: " << lowerBound[0] << " Y: " << lowerBound[1] << " | Upperbound X: " << upperBound[0] << " Y: " << upperBound[1] << endl;
}

bool IEntity::Update() {
  return Update(KeyboardKeyCode::IC_KEY_NONE);
}

bool IEntity::Update(const uint8_t pressedKeys) {
  return false;
}

bool IEntity::Update(const uint8_t pressedKeys, aabb::Tree<IEntity*>& spacePartitionObjectsTree) {
  return false;
}

void IEntity::InitWithSpriteSheet(EntitySpriteSheet *_spriteSheet) {
  spriteSheet = _spriteSheet;
}

void IEntity::Hit(bool) {
}
