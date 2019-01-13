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
  void STATE_Idle(EventData*);
  void STATE_Stop(EventData*);
  void STATE_Start(MotorData*);
  void STATE_ChangeSpeed(MotorData*);

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&MainCharacter::STATE_Idle)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Stop)
      STATE_MAP_ENTRY(&MainCharacter::STATE_Start)
      STATE_MAP_ENTRY(&MainCharacter::STATE_ChangeSpeed)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum MainCharacterStateIdentificator {
      STATE_IDLE = 0, // Initial state
      STATE_STOP,
      STATE_START,
      STATE_CHANGE_SPEED,
      MAIN_CHARACTER_MAX_STATES
  };
};

#endif
