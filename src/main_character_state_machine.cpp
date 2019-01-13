#include "main_character_state_machine.h"

class MainCharacterIdleState;
class MainCharacterRunToRightState;

MainCharacterStateMachine::MainCharacterStateMachine() {
        transit<MainCharacterIdleState>();
}

MainCharacterStateMachine::~MainCharacterStateMachine() {

}

void MainCharacterIdleState::entry() {
    //send_event(MotorStop());
    cout << "MainCharacterIdleState entry" << endl;
}

void MainCharacterIdleState::react(KeyRightPressedEvent const & e) {
    cout << "EVENT CALL RECEIVED" << endl;
}
