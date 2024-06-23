#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <sprite.h>
#include "collision/collision.h"
#include <AABB/AABB.h>
#include <algorithm>

using namespace std;

enum PlayerDirection: uint8_t { RIGHT = 0, LEFT = 1 };
struct ObjectCollision { ISceneObject* object; int horizontalCorrection; int verticalCorrection; };


class Player: public ISceneObject
{
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  std::vector<SpriteData> currentAnimationSprites;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool animationLoaded = false;
  bool headedToRight = true;
  uint8_t prevPressedKeys = KeyboardKeyCode::IC_KEY_NONE;
  uint8_t pressedKeys = KeyboardKeyCode::IC_KEY_NONE;
  void ProcessPressedKeys(bool = true);
  void ProcessReleasedKeys();
  void LoadAnimationWithId(uint16_t);
  SpriteData NextSpriteData();
  void LoadNextSprite();
  bool PlayerIsQuiet();
  void UpdatePreviousDirection();
  void GetSolidCollisions(std::vector<ObjectCollision>&, bool&);

  // Jump trajectory data
  float hInitialJumpSpeed = 0.0f;
  float vInitialJumpSpeed = 0.0f;
  float tJump = 0.0f;
  float hInitialJumpPosition = 0.0f;
  float vInitialJumpPosition = 0.0f;
  float previous_vOffset = 0.0f;

  // Fall trajectory data
  float hInitialFallSpeed = 0.0f;
  float vInitialFallSpeed = 0.0f;
  float tFall = 0.0f;
  float hInitialFallPosition = 0.0f;
  float vInitialFallPosition = 0.0f;

  // Player global physics values
  const float gravity = 9.81f;
  uint16_t hMomentum = 0;
  const uint16_t maxMomentum = 15;
  collision::CollisionDetector collisionDetector;
  collision::vec2<int16_t> vectorDirection;
  collision::vec2<int16_t> prevVectorDirection;
  std::vector<ISceneObject*> pillarObjects;

  // Player action states
  bool isJumping = false;
  bool isHitting = false;
  bool isLeaningOnTheGround = false;
  bool isFalling = false;
  bool isJumpApex = false;      // Player is at the the max altitude during a jump
  bool isBlockedRight = false;  // Player is stuck running to the right towards a brick
  bool isBlockedLeft = false;   // Player is stuck running to the left towards a brick

  // Player action update functions
  void UpdateCollisions();
  void MoveTo(PlayerDirection);
  void Jump(float vSpeed, float hSpeed);
  void UpdateJump();
  void FinishJump();
  void FallDueToLateralCollisionJump();
  void FallDueToSuspendedInTheAir();
  void Fall(float hSpeed);
  void UpdateFall();
  void FinishFall();
public:
  Player();
  ~Player() override;
  void InitWithSpriteSheet(ObjectSpriteSheet*) override;
  uint16_t Width() override;
  uint16_t Height() override;
  void PrintName() override;
  bool Update(uint8_t) override;
  static ISceneObject* Create();

  void RightKeyPressed();
  void RightKeyReleased();
  void LeftKeyPressed();
  void LeftKeyReleased();
  void UpKeyPressed();
  void SpaceKeyPressed();
  void DownKeyPressed();
  void JumpLanding();
  void FallLanding();
  void TopCollisionDuringJump();
  void LateralCollisionDuringJump();
  void RightKeyPressedAtJumpApex();
  void LeftKeyPressedAtJumpApex();
  void SuspendedInTheAir();
  bool ShouldBeginAnimationLoopAgain();
  //void ReachedSpeedForRunning();

private:
  // state machine state functions
  void STATE_Idle_Right();
  void STATE_Idle_Left();
  void STATE_Run_Right();
  void STATE_Run_Left();
  void STATE_Jump_Idle_Right();
  void STATE_Jump_Idle_Left();
  void STATE_Jump_Run_Right();
  void STATE_Jump_Run_Left();
  void STATE_Fall_Idle_Right();
  void STATE_Fall_Idle_Left();
  void STATE_Fall_Run_Right();
  void STATE_Fall_Run_Left();
  void STATE_Fall_Jump_Run_Right();
  void STATE_Fall_Jump_Run_Left();
  void STATE_Hit_Right();
  void STATE_Hit_Left();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&Player::STATE_Idle_Right)
      STATE_MAP_ENTRY(&Player::STATE_Idle_Left)
      STATE_MAP_ENTRY(&Player::STATE_Run_Right)
      STATE_MAP_ENTRY(&Player::STATE_Run_Left)
      STATE_MAP_ENTRY(&Player::STATE_Jump_Idle_Right)
      STATE_MAP_ENTRY(&Player::STATE_Jump_Idle_Left)
      STATE_MAP_ENTRY(&Player::STATE_Jump_Run_Right)
      STATE_MAP_ENTRY(&Player::STATE_Jump_Run_Left)
      STATE_MAP_ENTRY(&Player::STATE_Fall_Idle_Right)
      STATE_MAP_ENTRY(&Player::STATE_Fall_Idle_Left)
      STATE_MAP_ENTRY(&Player::STATE_Fall_Run_Right)
      STATE_MAP_ENTRY(&Player::STATE_Fall_Run_Left)
      STATE_MAP_ENTRY(&Player::STATE_Fall_Jump_Run_Right)
      STATE_MAP_ENTRY(&Player::STATE_Fall_Jump_Run_Left)
      STATE_MAP_ENTRY(&Player::STATE_Hit_Right)
      STATE_MAP_ENTRY(&Player::STATE_Hit_Left)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum PlayerStateIdentificator {
      STATE_IDLE_RIGHT = 0, // Initial state
      STATE_IDLE_LEFT,
      STATE_RUN_RIGHT,
      STATE_RUN_LEFT,
      STATE_JUMP_IDLE_RIGHT,
      STATE_JUMP_IDLE_LEFT,
      STATE_JUMP_RUN_RIGHT,
      STATE_JUMP_RUN_LEFT,
      STATE_FALL_IDLE_RIGHT,
      STATE_FALL_IDLE_LEFT,
      STATE_FALL_RUN_RIGHT,
      STATE_FALL_RUN_LEFT,
      STATE_FALL_JUMP_RUN_RIGHT,
      STATE_FALL_JUMP_RUN_LEFT,
      STATE_HIT_RIGHT,
      STATE_HIT_LEFT,
      MAIN_CHARACTER_MAX_STATES
  };
};

#endif
