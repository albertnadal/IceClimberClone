#pragma once

typedef short int16;
typedef int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char uchar;

enum SceneObjectIdentificator: uint16 { NONE = 0, MAIN_CHARACTER = 1, ROCKET = 2 };
enum KeyboardKeyCode: uchar { KEY_LEFT = 0x80, KEY_UP = 0x40, KEY_RIGHT = 0x20, KEY_DOWN = 0x10, KEY_Q = 0x08, KEY_W = 0x04, KEY_A = 0x02, KEY_S = 0x01, KEY_NONE = 0x00 };
