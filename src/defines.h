#pragma once

typedef short int16;
typedef int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char uchar;

// Keyboard key codes
enum KeyboardKeyCode: uchar { KEY_LEFT = 0x80, KEY_UP = 0x40, KEY_RIGHT = 0x20, KEY_DOWN = 0x10, KEY_Q = 0x08, KEY_W = 0x04, KEY_A = 0x02, KEY_SPACE = 0x01, KEY_NONE = 0x00 };

// Object identificators
enum SceneObjectIdentificator: uint16 { NONE = 0, MAIN_CHARACTER = 1, BRICK = 2, BRICK_BROWN = 3, BRICK_BLUE = 4, BRICK_GREEN_HALF = 5, BRICK_BROWN_HALF = 6, BRICK_BLUE_HALF = 7 };

// Object animation identificators
enum MainCharacterAnimation: uint16 { STAND_BY_RIGHT = 0, STAND_BY_LEFT = 1, RUN_TO_RIGHT = 2, RUN_TO_LEFT = 3, JUMP_RIGHT = 4, JUMP_LEFT = 5, HIT_RIGHT = 6, HIT_LEFT = 7 };
enum BrickAnimation: uint16 { BRICK_GREEN_STICKY = 8, BRICK_GREEN_FALLING = 9 };
enum BrickBrownAnimation: uint16 { BRICK_BROWN_STICKY = 10, BRICK_BROWN_FALLING = 11 };
enum BrickBlueAnimation: uint16 { BRICK_BLUE_STICKY = 12, BRICK_BLUE_FALLING = 13 };
enum BrickGreenHalfAnimation: uint16 { BRICK_GREEN_HALF_STICKY = 14, BRICK_GREEN_HALF_FALLING = 15 };
enum BrickBrownHalfAnimation: uint16 { BRICK_BROWN_HALF_STICKY = 16, BRICK_BROWN_HALF_FALLING = 17 };
enum BrickBlueHalfAnimation: uint16 { BRICK_BLUE_HALF_STICKY = 18, BRICK_BLUE_HALF_FALLING = 19 };
