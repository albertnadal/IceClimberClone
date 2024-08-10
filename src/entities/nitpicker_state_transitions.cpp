#include <entities/nitpicker.h>

void Nitpicker::ChangeDirection() {
    BEGIN_TRANSITION_MAP                                           // - Current State -
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fly_Right
            TRANSITION_MAP_ENTRY (EVENT_IGNORED)                   // STATE_Fly_Left
    END_TRANSITION_MAP(nullptr)
}
