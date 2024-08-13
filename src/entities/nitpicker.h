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
  std::pair<float, float> speedVector{0.0f, 0.0f};
  std::vector<std::pair<float, float>> flyingRoute;
  std::vector<std::pair<float, float>>::iterator flyingRouteIt;
  std::optional<chrono::system_clock::time_point> nextRespawnTime = std::nullopt;
  int numAttackAttempts;

  // Nitpicker action states
  bool isWaitingForRespawn = true; // Nitpicker is hidden and waiting for respawn
  bool isFlying = false;           // Nitpicker is flying
  bool isRetreating = false;       // Nitpicker is retreating from attacking Popo
  bool isFalling = false;          // Nitpicker is falling

  // Nitpicker action update functions
  static float calculateDistance(const std::pair<float, float>&, const std::pair<float, float>&);
  void WaitUntilRespawnTime();
  void UpdateFlight();
  void CalculateNewFlyingRoute();
  void FinishFall();

public:
  Nitpicker();
  ~Nitpicker() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  void PrintName() override;
  bool Update(uint8_t) override;
  void Hit(bool) override;
  static IEntity* Create();

  // state machine triggers
  void StartFlight();
  void EndFlight();
  void StartFall();
  void EndFall();
  bool ShouldBeginAnimationLoopAgain();

private:
  // state machine state functions
  void STATE_Waiting_Respawn();
  void STATE_Flying();
  void STATE_Falling();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&Nitpicker::STATE_Waiting_Respawn)
      STATE_MAP_ENTRY(&Nitpicker::STATE_Flying)
      STATE_MAP_ENTRY(&Nitpicker::STATE_Falling)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum NitpickerStateIdentificator {
      STATE_WAITING_RESPAWN = 0, // Initial state
      STATE_FLYING,
      STATE_FALLING,
      NITPICKER_MAX_STATES
  };
};

#endif
