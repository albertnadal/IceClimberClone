#include <entities/player.h>

void Player::RightKeyPressed() {
    BEGIN_TRANSITION_MAP                            // - Current State -
            TRANSITION_MAP_ENTRY (STATE_RUN_RIGHT)  // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_RIGHT)  // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Killed
            TRANSITION_MAP_ENTRY (STATE_RUN_RIGHT)  // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)   // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::RightKeyReleased() {
    BEGIN_TRANSITION_MAP                            // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT) // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Killed
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT) // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::LeftKeyPressed() {
    BEGIN_TRANSITION_MAP                            // - Current State -
            TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)   // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)   // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)   // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::LeftKeyReleased() {
    BEGIN_TRANSITION_MAP                            // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)  // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)  // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::UpKeyPressed() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_RIGHT)  // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_LEFT)   // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_JUMP_RUN_RIGHT)   // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_JUMP_RUN_LEFT)    // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_RIGHT)  // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_LEFT)   // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::TopCollisionDuringJump() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)  // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)   // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_RIGHT)   // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_LEFT)    // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::LateralCollisionDuringJump() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)  // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)   // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)  // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)   // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::RightKeyPressedAtJumpApex() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_RIGHT)   // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::LeftKeyPressedAtJumpApex() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_RUN_LEFT)    // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::SuspendedInTheAir() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)  // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)   // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)  // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)   // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::StopRunningOnSlidingSurface() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_SLIP_RIGHT)       // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_SLIP_LEFT)        // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::StopSlipping() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)       // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)        // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::SpaceKeyPressed() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_HIT_RIGHT)        // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_HIT_LEFT)         // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_HIT_RIGHT)        // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_HIT_LEFT)         // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (STATE_HIT_RIGHT)        // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (STATE_HIT_LEFT)         // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::JumpLanding() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)       // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)        // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)       // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)        // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::FallLanding() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)       // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)        // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)       // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)        // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::Killed() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Run_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Run_Left
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (STATE_KILLED)           // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}

void Player::StartRespawn() {
    BEGIN_TRANSITION_MAP                                  // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Run_Left
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_RIGHT)    // STATE_Jump_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_LEFT)     // STATE_Jump_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_RIGHT)    // STATE_Jump_Run_Right
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_LEFT)     // STATE_Jump_Run_Left
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_RIGHT)    // STATE_Fall_Idle_Right
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_LEFT)     // STATE_Fall_Idle_Left
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_RIGHT)    // STATE_Fall_Run_Right
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_LEFT)     // STATE_Fall_Run_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Hit_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Slip_Left
            TRANSITION_MAP_ENTRY (STATE_RESPAWN_RIGHT)    // STATE_Killed
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Respawn_Left
    END_TRANSITION_MAP(nullptr)
}