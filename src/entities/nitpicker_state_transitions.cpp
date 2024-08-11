#include <entities/nitpicker.h>

void Nitpicker::StartFlight() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (STATE_FLYING)                    // STATE_Waiting_Respawn
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Flying
    END_TRANSITION_MAP(nullptr)
}

void Nitpicker::EndFlight() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Waiting_Respawn
            TRANSITION_MAP_ENTRY (STATE_WAITING_RESPAWN)           // STATE_Flying
    END_TRANSITION_MAP(nullptr)
}