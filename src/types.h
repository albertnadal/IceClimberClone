#pragma once

typedef short int16;
typedef int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char uchar;

// Object identificators
enum SceneObjectIdentificator: uint16 { NONE = 0, MAIN_CHARACTER = 1, BRICK = 2 };

// Keyboard key codes
enum KeyboardKeyCode: uchar { KEY_LEFT = 0x80, KEY_UP = 0x40, KEY_RIGHT = 0x20, KEY_DOWN = 0x10, KEY_Q = 0x08, KEY_W = 0x04, KEY_A = 0x02, KEY_SPACE = 0x01, KEY_NONE = 0x00 };

// Object animation identificators
enum MainCharacterAnimation: uint16 { STAND_BY_RIGHT = 0, STAND_BY_LEFT = 1, RUN_TO_RIGHT = 2, RUN_TO_LEFT = 3, JUMP_RIGHT = 4, JUMP_LEFT = 5, HIT_RIGHT = 6, HIT_LEFT = 7 };
enum BrickAnimation: uint16 { STICKY = 8, FALLING = 9 };
