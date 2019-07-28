#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <vec2.h>
#include <sprite.h>
#include <position.h>

using namespace std;

enum MainCharacterDirection: uint16 { RIGHT = 0, LEFT = 1 };

// structure to hold event data passed into state machine
struct MotorData : public EventData
{
    int speed;
};

class MainCharacter: public ISceneObject
{
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  std::vector<SpriteData> currentAnimationSprites;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool animationLoaded = false;
  bool headedToRight = true;
  uchar prevPressedKeys = KeyboardKeyCode::KEY_NONE;
  uchar pressedKeys = KeyboardKeyCode::KEY_NONE;
  void ProcessPressedKeys(bool = true);
  void ProcessReleasedKeys();
  void LoadAnimationWithId(uint16);
  SpriteData NextSpriteData();
  void LoadNextSprite();
  float hInitialJumpSpeed = 0.0f;
  float vInitialJumpSpeed = 0.0f;
  float tJump = 0.0f;
  float hInitialJumpPosition = 0.0f;
  float vInitialJumpPosition = 0.0f;
  const float gravity = 9.81f;
  uint16 hMomentum = 0;
  const uint16 maxMomentum = 15;
  float isJumping = false;
  float isHitting = false;
  void MoveTo(MainCharacterDirection);
  void UpdateJump();
  void Jump(float, float);
public:
  MainCharacter();
  ~MainCharacter();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16 Width();
  uint16 Height();
  void PrintName();
  bool Update(uchar);
  static ISceneObject* Create();

  void RightKeyPressed();
  void RightKeyReleased();
  void LeftKeyPressed();
  void LeftKeyReleased();
  void UpKeyPressed();
  void SpaceKeyPressed();
  void JumpLanding();
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
  void STATE_Hit_Right();
  void STATE_Hit_Left();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&MainCharacter::STATE_Idle_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Idle_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Run_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Run_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Jump_Idle_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Jump_Idle_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Jump_Run_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Jump_Run_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Hit_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Hit_Left)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum MainCharacterStateIdentificator {
      STATE_IDLE_RIGHT = 0, // Initial state
      STATE_IDLE_LEFT,
      STATE_RUN_RIGHT,
      STATE_RUN_LEFT,
      STATE_JUMP_IDLE_RIGHT,
      STATE_JUMP_IDLE_LEFT,
      STATE_JUMP_RUN_RIGHT,
      STATE_JUMP_RUN_LEFT,
      STATE_HIT_RIGHT,
      STATE_HIT_LEFT,
      MAIN_CHARACTER_MAX_STATES
  };
};

#endif
