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
  void ST_Idle(EventData*);
  void ST_Stop(EventData*);
  void ST_Start(MotorData*);
  void ST_ChangeSpeed(MotorData*);

  // state map to define state function order
  BEGIN_STATE_MAP
      STATE_MAP_ENTRY(&MainCharacter::ST_Idle)
      STATE_MAP_ENTRY(&MainCharacter::ST_Stop)
      STATE_MAP_ENTRY(&MainCharacter::ST_Start)
      STATE_MAP_ENTRY(&MainCharacter::ST_ChangeSpeed)
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum E_States {
      ST_IDLE = 0,
      ST_STOP,
      ST_START,
      ST_CHANGE_SPEED,
      ST_MAX_STATES
  };
};

#endif
