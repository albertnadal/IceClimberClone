#pragma once

#include <cstdint>

// Keyboard key codes
enum KeyboardKeyCode: uint8_t { IC_KEY_LEFT = 0x80, IC_KEY_UP = 0x40, IC_KEY_RIGHT = 0x20, IC_KEY_DOWN = 0x10, IC_KEY_Q = 0x08, IC_KEY_W = 0x04, IC_KEY_A = 0x02, IC_KEY_SPACE = 0x01, IC_KEY_NONE = 0x00 };

// Object identificators
enum EntityIdentificator: uint16_t { NONE = 0, POPO = 1, BRICK = 2, BRICK_BROWN = 3, BRICK_BLUE = 4, BRICK_GREEN_HALF = 5, BRICK_BROWN_HALF = 6, BRICK_BLUE_HALF = 7, SIDE_WALL = 8, SIDE_WALL_GREEN_LEFT = 9, SIDE_WALL_GREEN_RIGHT = 10, SIDE_WALL_GREEN_COLUMNS_LEFT = 11, SIDE_WALL_GREEN_COLUMNS_RIGHT = 12, SIDE_WALL_BROWN_COLUMNS_LEFT = 13, SIDE_WALL_BROWN_COLUMNS_RIGHT = 14, SIDE_WALL_BROWN_LEFT = 15, SIDE_WALL_BROWN_RIGHT = 16, SIDE_WALL_BLUE_LEFT = 17, SIDE_WALL_BLUE_RIGHT = 18, SIDE_WALL_BLUE_COLUMNS_LEFT = 19, SIDE_WALL_BLUE_COLUMNS_RIGHT = 20, BRICK_BLUE_CONVEYOR_BELT_RIGHT = 21, BRICK_BLUE_CONVEYOR_BELT_LEFT = 22, BRICK_BROWN_CONVEYOR_BELT_RIGHT = 23, BRICK_BROWN_CONVEYOR_BELT_LEFT = 24, BRICK_GREEN_CONVEYOR_BELT_RIGHT = 25, BRICK_GREEN_CONVEYOR_BELT_LEFT = 26, BRICK_GREEN_UNBREAKABLE = 27, BRICK_BROWN_UNBREAKABLE = 28, BRICK_BLUE_UNBREAKABLE = 29, BRICK_BLUE_CONVEYOR_BELT_RIGHT_UNBREAKABLE = 30, BRICK_BLUE_CONVEYOR_BELT_LEFT_UNBREAKABLE = 31, BRICK_GREEN_CONVEYOR_BELT_RIGHT_UNBREAKABLE = 32, BRICK_GREEN_CONVEYOR_BELT_LEFT_UNBREAKABLE = 33, BRICK_BROWN_CONVEYOR_BELT_RIGHT_UNBREAKABLE = 34, BRICK_BROWN_CONVEYOR_BELT_LEFT_UNBREAKABLE = 35, CLOUD_SMALL = 36, CLOUD_BIG = 37, CLOUD_TINY = 38, SIDE_WALL_ICE_MODEL_A_UNBREAKABLE = 39, SIDE_WALL_ICE_MODEL_B_UNBREAKABLE = 40, TOPI = 41, ICE = 42, WATER = 43, BONUS_STAGE_TEXT = 44 };

// Object type
enum EntityType: uint16_t { TERRAIN = 0, PLAYER = 1, ENEMY = 2 };

// Underlying object surface type
enum SurfaceType: uint16_t { SIMPLE = 0, SLIDING = 1, MOBILE_RIGHT = 2, MOBILE_LEFT = 3 };

// Object movement direction
enum Direction: uint8_t { RIGHT = 0, LEFT = 1 };

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
enum BrickBlueConveyorBeltRightAnimation: uint16_t { BRICK_BLUE_CONVEYOR_BELT_RIGHT_STICKY = 34, BRICK_BLUE_CONVEYOR_BELT_RIGHT_FALLING = 35 };
enum BrickBlueConveyorBeltLeftAnimation: uint16_t { BRICK_BLUE_CONVEYOR_BELT_LEFT_STICKY = 36, BRICK_BLUE_CONVEYOR_BELT_LEFT_FALLING = 37 };
enum BrickBrownConveyorBeltRightAnimation: uint16_t { BRICK_BROWN_CONVEYOR_BELT_RIGHT_STICKY = 38, BRICK_BROWN_CONVEYOR_BELT_RIGHT_FALLING = 39 };
enum BrickBrownConveyorBeltLeftAnimation: uint16_t { BRICK_BROWN_CONVEYOR_BELT_LEFT_STICKY = 40, BRICK_BROWN_CONVEYOR_BELT_LEFT_FALLING = 41 };
enum BrickGreenConveyorBeltRightAnimation: uint16_t { BRICK_GREEN_CONVEYOR_BELT_RIGHT_STICKY = 42, BRICK_GREEN_CONVEYOR_BELT_RIGHT_FALLING = 43 };
enum BrickGreenConveyorBeltLeftAnimation: uint16_t { BRICK_GREEN_CONVEYOR_BELT_LEFT_STICKY = 44, BRICK_GREEN_CONVEYOR_BELT_LEFT_FALLING = 45 };
enum BrickGreenUnbreakableAnimation: uint16_t { BRICK_GREEN_UNBREAKABLE_STICKY = 46 };
enum BrickBrownUnbreakableAnimation: uint16_t { BRICK_BROWN_UNBREAKABLE_STICKY = 47 };
enum BrickBlueUnbreakableAnimation: uint16_t { BRICK_BLUE_UNBREAKABLE_STICKY = 48 };
enum BrickBlueConveyorBeltRightUnbreakableAnimation: uint16_t { BRICK_BLUE_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY = 49 };
enum BrickBlueConveyorBeltLeftUnbreakableAnimation: uint16_t { BRICK_BLUE_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY = 50 };
enum BrickGreenConveyorBeltRightUnbreakableAnimation: uint16_t { BRICK_GREEN_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY = 51 };
enum BrickGreenConveyorBeltLeftUnbreakableAnimation: uint16_t { BRICK_GREEN_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY = 52 };
enum BrickBrownConveyorBeltRightUnbreakableAnimation: uint16_t { BRICK_BROWN_CONVEYOR_BELT_RIGHT_UNBREAKABLE_STICKY = 53 };
enum BrickBrownConveyorBeltLeftUnbreakableAnimation: uint16_t { BRICK_BROWN_CONVEYOR_BELT_LEFT_UNBREAKABLE_STICKY = 54 };
enum CloudSmallAnimation: uint16_t { CLOUD_SMALL_FLYING = 55 };
enum CloudBigAnimation: uint16_t { CLOUD_BIG_FLYING = 56 };
enum CloudTinyAnimation: uint16_t { CLOUD_TINY_FLYING = 57 };
enum SideWallIceModelAUnbreakableAnimation: uint16_t { SIDE_WALL_ICE_MODEL_A_UNBREAKABLE_STICKY = 58 };
enum SideWallIceModelBUnbreakableAnimation: uint16_t { SIDE_WALL_ICE_MODEL_B_UNBREAKABLE_STICKY = 59 };
enum TopiAnimation: uint16_t { TOPI_WALK_TO_RIGHT = 60, TOPI_WALK_TO_LEFT = 61, TOPI_RUN_TO_RIGHT = 62, TOPI_RUN_TO_LEFT = 63, TOPI_FALL_DAZED_RIGHT = 64, TOPI_FALL_DAZED_LEFT = 65, RUN_DAZED_RIGHT = 66, RUN_DAZED_LEFT = 67 };
enum IceAnimation: uint16_t { ICE_STICKY = 68 };
enum WaterAnimation: uint16_t { WATER_STICKY = 69 };
enum BonusStageTextAnimation: uint16_t { BONUS_STAGE_TEXT_STICKY = 70 };

// Others
constexpr const char* ENTITY_TYPES_FILENAME = "objtypes.dat";              // Data file where entity animations and collision areas are defined.
constexpr float GRAVITY = 9.81f;                                           // Gravity value used for physics calculations.
constexpr int MIN_PIXELS_ON_UNDERLYING_SURFACE = 4;                        // Min number of pixels on the edge of an underlying surface to do not fall.
constexpr int SLIPPING_DISTANCE = 30;                                      // The distance, in pixels, the player slips when suddenly stopping on an ice floor.
constexpr int CELL_WIDTH = 16;                                             // The width, in pixels, of a map cell.
constexpr int CELL_HEIGHT = 16;                                            // The height, in pixels, of a map cell.
constexpr float CELL_WIDTH_FLOAT = 16.0f;
constexpr float CELL_HEIGHT_FLOAT = 16.0f;
constexpr int LEVEL_WIDTH_CELLS = 32;                                      // The width of a level in number of cells.
constexpr int LEVEL_WIDTH = LEVEL_WIDTH_CELLS * CELL_WIDTH;                // The integer width in pixels of a level. 32cells x 16px
constexpr float LEVEL_WIDTH_FLOAT = LEVEL_WIDTH_CELLS * CELL_WIDTH_FLOAT;  // The float width in pixels of a level. 32cells x 16px
constexpr float TOPI_LEVEL_RIGHT_EDGE_MARGIN = 5.0f;
constexpr float INITIAL_CAMERA_POSITION = 156 * CELL_HEIGHT_FLOAT;         // Initial camera vertical position when player is at level 1
constexpr int BONUS_STAGE_CELL_Y = 128;                                    // Vertical cell position where bonus stage starts
constexpr float CAMERA_PADDING_TOP = 13 * CELL_HEIGHT_FLOAT;               // Camera viewport top padding space
constexpr float CAMERA_BONUS_STAGE_PADDING_TOP = 19 * CELL_HEIGHT_FLOAT;   // Camera viewport top padding space in the bonus stage
constexpr float CAMERA_SPEED = 3.0f;                                       // Camera speed in vertical píxels per tick