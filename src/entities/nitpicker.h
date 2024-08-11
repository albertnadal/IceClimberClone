#ifndef NITPICKER_H
#define NITPICKER_H

#include <iostream>
#include <vector>
#include <optional>
#include <chrono>
#include <entity.h>
#include <entity_manager.h>
#include <state_machine.h>
#include <sprite.h>
#include <collision/collision.h>
#include <AABB/AABB.h>
#include <algorithm>
#include <defines.h>

using namespace std;


class Nitpicker: public IEntity
{
  Direction direction;
  bool ReachedScreenEdge();
  void Respawn();
  std::pair<float, float> speedVector{0.0f, 0.0f};
  std::vector<std::pair<float, float>> flyingRoute;
  std::vector<std::pair<float, float>>::iterator flyingRouteIt;
  std::optional<chrono::system_clock::time_point> nextRespawnTime = std::nullopt;

  // Nitpicker action states
  bool isWaitingForRespawn = true; // Nitpicker is hidden and waiting for respawn
  bool isFlying = false;           // Nitpicker is flying
  bool isFalling = false;          // Nitpicker is falling

  // Nitpicker action update functions
  static float calculateDistance(const std::pair<float, float>&, const std::pair<float, float>&);
  void WaitUntilRespawnTime();
  void CalculateNewFlyingRoute();
  void FinishFall();

public:
  Nitpicker();
  ~Nitpicker() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  void PrintName() override;
  bool Update(uint8_t) override;
  static IEntity* Create();

  // state machine triggers
  void ChangeDirection();
  bool ShouldBeginAnimationLoopAgain();

private:
  // state machine state functions
  void STATE_Waiting_Respawn();
  void STATE_Fly_Right();
  void STATE_Fly_Left();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&Nitpicker::STATE_Waiting_Respawn)
      STATE_MAP_ENTRY(&Nitpicker::STATE_Fly_Right)
      STATE_MAP_ENTRY(&Nitpicker::STATE_Fly_Left)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum NitpickerStateIdentificator {
      STATE_WAITING_RESPAWN = 0, // Initial state
      STATE_FLY_RIGHT,
      STATE_FLY_LEFT,
      NITPICKER_MAX_STATES
  };
};

#endif
