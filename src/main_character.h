#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <iostream>
#include <vector>
#include <scene_object.h>
#include <state_machine.h>
#include <vec2.h>
#include <sprite.h>

using namespace std;

// structure to hold event data passed into state machine
struct MotorData : public EventData
{
    int speed;
};

class MainCharacter: public ISceneObject, StateMachine
{
  std::vector<SpriteData>::iterator currentAnimationSpriteIterator;
  std::vector<SpriteData> currentAnimationSprites;
  bool firstSpriteOfCurrentAnimationIsLoaded = false;
  bool animationHasOnlyOneSprite = false;
  bool animationLoaded = false;
  bool headedToRight = true;
  uchar prevPressedKeys = KeyboardKeyCode::KEY_NONE;
  void ProcessPressedKeys(uchar);
  void LoadAnimationWithId(uint16);
  SpriteData NextSpriteData();
public:
  MainCharacter();
  ~MainCharacter();
  void InitWithSpriteSheet(ObjectSpriteSheet*);
  uint16 Width();
  uint16 Height();
  void PrintName();
  bool Update(uchar);
  static ISceneObject* Create();

  void Halt();
  void SetSpeed(MotorData*);

private:
  // state machine state functions
  void STATE_Idle_Right(EventData*);
  void STATE_Idle_Left(EventData*);
  void STATE_Run_Right(MotorData*);
  void STATE_Run_Left(MotorData*);
  void STATE_Fast_Run_Right(MotorData*);
  void STATE_Fast_Run_Left(MotorData*);

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&MainCharacter::STATE_Idle_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Idle_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Run_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Run_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Fast_Run_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Fast_Run_Left)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum MainCharacterStateIdentificator {
      STATE_IDLE_RIGHT = 0, // Initial state
      STATE_IDLE_LEFT,
      STATE_RUN_RIGHT,
      STATE_RUN_LEFT,
      STATE_FAST_RUN_RIGHT,
      STATE_FAST_RUN_LEFT,
      MAIN_CHARACTER_MAX_STATES
  };
};

#endif
