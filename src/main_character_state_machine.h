#ifndef MAIN_CHARACTER_STATE_MACHINE_H
#define MAIN_CHARACTER_STATE_MACHINE_H

#include <iostream>
#include <tinyfsm/tinyfsm.hpp>
#include <scene_object.h>

using namespace std;

/* Finite state machine event declarations */
struct KeyRightPressedEvent: tinyfsm::Event { };
struct KeyLeftPressedEvent: tinyfsm::Event { };
struct KeyUpPressedEvent: tinyfsm::Event { };
struct KeyDownPressedEvent: tinyfsm::Event { };

class MainCharacterStateMachine: public tinyfsm::Fsm<MainCharacterStateMachine>
{
public:
  MainCharacterStateMachine();
  ~MainCharacterStateMachine();

  virtual void react(tinyfsm::Event const &) { cout << "NOT VALID" << endl;};
  virtual void react(KeyRightPressedEvent const & e) { cout << "NOT VALID MoveToRightEvent!" << endl; };
  virtual void entry(void) { };
  void exit(void) { };
};

class MainCharacterIdleState: public MainCharacterStateMachine
{
public:
  void entry() override;
  void react(KeyRightPressedEvent const & e) override;
};

#endif
