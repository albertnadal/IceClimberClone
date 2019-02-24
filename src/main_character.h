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
public:
    Position *test;
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
  bool BeginAnimationLoopAgain();

private:
  // state machine state functions
  void STATE_Idle_Right();
  void STATE_Idle_Left();
  void STATE_Run_Right();
  void STATE_Run_Left();
  void STATE_Fast_Run_Right();
  void STATE_Fast_Run_Left();
  void STATE_Short_Break_Right();
  void STATE_Short_Break_Left();
  void STATE_Change_Direction_Right();
  void STATE_Change_Direction_Left();

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&MainCharacter::STATE_Idle_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Idle_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Run_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Run_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Fast_Run_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Fast_Run_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Short_Break_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Short_Break_Left)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Change_Direction_Right)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Change_Direction_Left)
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
      STATE_SHORT_BREAK_RIGHT,
      STATE_SHORT_BREAK_LEFT,
      STATE_CHANGE_DIRECTION_RIGHT,
      STATE_CHANGE_DIRECTION_LEFT,
      MAIN_CHARACTER_MAX_STATES
  };
};

#endif
