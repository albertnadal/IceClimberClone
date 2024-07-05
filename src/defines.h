#pragma once

#include <cstdint>

// Keyboard key codes
enum KeyboardKeyCode: uint8_t { IC_KEY_LEFT = 0x80, IC_KEY_UP = 0x40, IC_KEY_RIGHT = 0x20, IC_KEY_DOWN = 0x10, IC_KEY_Q = 0x08, IC_KEY_W = 0x04, IC_KEY_A = 0x02, IC_KEY_SPACE = 0x01, IC_KEY_NONE = 0x00 };

// Object identificators
enum SceneObjectIdentificator: uint16_t { NONE = 0, MAIN_CHARACTER = 1, BRICK = 2, BRICK_BROWN = 3, BRICK_BLUE = 4, BRICK_GREEN_HALF = 5, BRICK_BROWN_HALF = 6, BRICK_BLUE_HALF = 7, SIDE_WALL = 8, SIDE_WALL_GREEN_LEFT = 9, SIDE_WALL_GREEN_RIGHT = 10, SIDE_WALL_GREEN_COLUMNS_LEFT = 11, SIDE_WALL_GREEN_COLUMNS_RIGHT = 12, SIDE_WALL_BROWN_COLUMNS_LEFT = 13, SIDE_WALL_BROWN_COLUMNS_RIGHT = 14, SIDE_WALL_BROWN_LEFT = 15, SIDE_WALL_BROWN_RIGHT = 16, SIDE_WALL_BLUE_LEFT = 17, SIDE_WALL_BLUE_RIGHT = 18, SIDE_WALL_BLUE_COLUMNS_LEFT = 19, SIDE_WALL_BLUE_COLUMNS_RIGHT = 20, BRICK_BLUE_CONVEYOR_BELT_RIGHT = 21, BRICK_BLUE_CONVEYOR_BELT_LEFT = 22, BRICK_BROWN_CONVEYOR_BELT_RIGHT = 23, BRICK_BROWN_CONVEYOR_BELT_LEFT = 24, BRICK_GREEN_CONVEYOR_BELT_RIGHT = 25, BRICK_GREEN_CONVEYOR_BELT_LEFT = 26 };

// Object type
enum SceneObjectType: uint16_t { TERRAIN = 0, PLAYER = 1, ENEMY = 2 };

// Underlying object surface type
enum SurfaceType: uint16_t { SIMPLE = 0, SLIDING = 1, MOBILE = 2 };

// Object animation identificators
enum PlayerAnimation: uint16_t { STAND_BY_RIGHT = 0, STAND_BY_LEFT = 1, RUN_TO_RIGHT = 2, RUN_TO_LEFT = 3, JUMP_RIGHT = 4, JUMP_LEFT = 5, FALL_RIGHT = 6, FALL_LEFT = 7, HIT_RIGHT = 8, HIT_LEFT = 9, SLIP_TO_RIGHT = 10, SLIP_TO_LEFT = 11 };
enum BrickAnimation: uint16_t { BRICK_GREEN_STICKY = 10, BRICK_GREEN_FALLING = 11 };
enum BrickBrownAnimation: uint16_t { BRICK_BROWN_STICKY = 12, BRICK_BROWN_FALLING = 13 };
enum BrickBlueAnimation: uint16_t { BRICK_BLUE_STICKY = 14, BRICK_BLUE_FALLING = 15 };
enum BrickGreenHalfAnimation: uint16_t { BRICK_GREEN_HALF_STICKY = 16, BRICK_GREEN_HALF_FALLING = 17 };
enum BrickBrownHalfAnimation: uint16_t { BRICK_BROWN_HALF_STICKY = 18, BRICK_BROWN_HALF_FALLING = 19 };
enum BrickBlueHalfAnimation: uint16_t { BRICK_BLUE_HALF_STICKY = 20, BRICK_BLUE_HALF_FALLING = 21 };
enum SideWallGreenLeftAnimation: uint16_t { SIDE_WALL_GREEN_LEFT_STICKY = 22 };
enum SideWallGreenRightAnimation: uint16_t { SIDE_WALL_GREEN_RIGHT_STICKY = 23 };
enum SideWallGreenColumnsLeftAnimation: uint16_t { SIDE_WALL_GREEN_COLUMNS_LEFT_STICKY = 24 };
enum SideWallGreenColumnsRightAnimation: uint16_t { SIDE_WALL_GREEN_COLUMNS_RIGHT_STICKY = 25 };
enum SideWallBrownColumnsLeftAnimation: uint16_t { SIDE_WALL_BROWN_COLUMNS_LEFT_STICKY = 26 };
enum SideWallBrownColumnsRightAnimation: uint16_t { SIDE_WALL_BROWN_COLUMNS_RIGHT_STICKY = 27 };
enum SideWallBrownLeftAnimation: uint16_t { SIDE_WALL_BROWN_LEFT_STICKY = 28 };
enum SideWallBrownRightAnimation: uint16_t { SIDE_WALL_BROWN_RIGHT_STICKY = 29 };
enum SideWallBlueLeftAnimation: uint16_t { SIDE_WALL_BLUE_LEFT_STICKY = 30 };
enum SideWallBlueRightAnimation: uint16_t { SIDE_WALL_BLUE_RIGHT_STICKY = 31 };
enum SideWallBlueColumnsLeftAnimation: uint16_t { SIDE_WALL_BLUE_COLUMNS_LEFT_STICKY = 32 };
enum SideWallBlueColumnsRightAnimation: uint16_t { SIDE_WALL_BLUE_COLUMNS_RIGHT_STICKY = 33 };
enum BrickBlueConveyorBeltRightAnimation: uint16_t { BRICK_BLUE_CONVEYOR_BELT_RIGHT_STICKY = 34 };
enum BrickBlueConveyorBeltLeftAnimation: uint16_t { BRICK_BLUE_CONVEYOR_BELT_LEFT_STICKY = 35 };
enum BrickBrownConveyorBeltRightAnimation: uint16_t { BRICK_BROWN_CONVEYOR_BELT_RIGHT_STICKY = 36 };
enum BrickBrownConveyorBeltLeftAnimation: uint16_t { BRICK_BROWN_CONVEYOR_BELT_LEFT_STICKY = 37 };
enum BrickGreenConveyorBeltRightAnimation: uint16_t { BRICK_GREEN_CONVEYOR_BELT_RIGHT_STICKY = 38 };
enum BrickGreenConveyorBeltLeftAnimation: uint16_t { BRICK_GREEN_CONVEYOR_BELT_LEFT_STICKY = 39 };

// Others
const float GRAVITY = 9.81;
const int MIN_PIXELS_ON_PLATFORM = 4;  // Min number of pixels on the edge of a platform to do not fall.
const int SLIPPING_DISTANCE = 30;      // The distance, in pixels, the player slips when suddenly stopping on an ice floor.
