#include <entities/topi.h>

void Topi::HoleDetectedWhenWalking() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_LEFT)   // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_RIGHT)  // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_LEFT)   // STATE_Bring_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_RIGHT)  // STATE_Bring_Ice_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::SuspendedInTheAir() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_RIGHT)          // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_LEFT)           // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_RIGHT)          // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (STATE_FALL_DAZED_LEFT)           // STATE_Run_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::FallLanding() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_LEFT)            // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_RIGHT)           // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::HitReceived() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_LEFT)            // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_RIGHT)           // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_LEFT)            // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_RIGHT)           // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_LEFT)            // STATE_Bring_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_RIGHT)           // STATE_Bring_Ice_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::ChangeDirection() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (STATE_WALK_LEFT)                 // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (STATE_WALK_RIGHT)                // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_LEFT)   // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_TO_PICK_UP_ICE_RIGHT)  // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_LEFT)            // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (STATE_RUN_DAZED_RIGHT)           // STATE_Run_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Left
    END_TRANSITION_MAP(nullptr)
}

void Topi::BringIceToFillHole() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Walk_Left
            TRANSITION_MAP_ENTRY (STATE_BRING_ICE_LEFT)            // STATE_Run_To_Pick_Up_Ice_Right
            TRANSITION_MAP_ENTRY (STATE_BRING_ICE_RIGHT)           // STATE_Run_To_Pick_Up_Ice_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fall_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Run_Dazed_Left
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Bring_Ice_Left
    END_TRANSITION_MAP(nullptr)
}