#pragma once

#include <cstdint>
#include <raylib/raylib.h>

// Screen types
enum GameScreenType: uint16_t { MAIN_MENU = 0, MOUNTAIN_GAME_PLAY = 1, PLAYER_SCORE_SUMMARY = 2 };

// Keyboard key codes
enum KeyboardKeyCode: uint8_t { IC_KEY_LEFT = 0x80, IC_KEY_UP = 0x40, IC_KEY_RIGHT = 0x20, IC_KEY_DOWN = 0x10, IC_KEY_Q = 0x08, IC_KEY_W = 0x04, IC_KEY_A = 0x02, IC_KEY_SPACE = 0x01, IC_KEY_NONE = 0x00 };

// Sound identificators
enum SoundIdentificator: uint16_t { JUMP = 0, BREAK_BLOCK = 1, HIT_ENEMY = 2, HIT_NITPICKER = 3, TAKE_VEGETABLE = 4, GAME_OVER_TONE = 5, LOSE_LIFE = 6 };

// Object identificators
enum EntityIdentificator: uint16_t { NONE = 0, POPO = 1, BRICK = 2, BRICK_BROWN = 3, BRICK_BLUE = 4, BRICK_GREEN_HALF = 5, BRICK_BROWN_HALF = 6, BRICK_BLUE_HALF = 7, SIDE_WALL = 8, SIDE_WALL_GREEN_LEFT = 9, SIDE_WALL_GREEN_RIGHT = 10, SIDE_WALL_GREEN_COLUMNS_LEFT = 11, SIDE_WALL_GREEN_COLUMNS_RIGHT = 12, SIDE_WALL_BROWN_COLUMNS_LEFT = 13, SIDE_WALL_BROWN_COLUMNS_RIGHT = 14, SIDE_WALL_BROWN_LEFT = 15, SIDE_WALL_BROWN_RIGHT = 16, SIDE_WALL_BLUE_LEFT = 17, SIDE_WALL_BLUE_RIGHT = 18, SIDE_WALL_BLUE_COLUMNS_LEFT = 19, SIDE_WALL_BLUE_COLUMNS_RIGHT = 20, BRICK_BLUE_CONVEYOR_BELT_RIGHT = 21, BRICK_BLUE_CONVEYOR_BELT_LEFT = 22, BRICK_BROWN_CONVEYOR_BELT_RIGHT = 23, BRICK_BROWN_CONVEYOR_BELT_LEFT = 24, BRICK_GREEN_CONVEYOR_BELT_RIGHT = 25, BRICK_GREEN_CONVEYOR_BELT_LEFT = 26, BRICK_GREEN_UNBREAKABLE = 27, BRICK_BROWN_UNBREAKABLE = 28, BRICK_BLUE_UNBREAKABLE = 29, BRICK_BLUE_CONVEYOR_BELT_RIGHT_UNBREAKABLE = 30, BRICK_BLUE_CONVEYOR_BELT_LEFT_UNBREAKABLE = 31, BRICK_GREEN_CONVEYOR_BELT_RIGHT_UNBREAKABLE = 32, BRICK_GREEN_CONVEYOR_BELT_LEFT_UNBREAKABLE = 33, BRICK_BROWN_CONVEYOR_BELT_RIGHT_UNBREAKABLE = 34, BRICK_BROWN_CONVEYOR_BELT_LEFT_UNBREAKABLE = 35, CLOUD_SMALL = 36, CLOUD_BIG = 37, CLOUD_TINY = 38, SIDE_WALL_ICE_MODEL_A_UNBREAKABLE = 39, SIDE_WALL_ICE_MODEL_B_UNBREAKABLE = 40, TOPI = 41, ICE = 42, WATER = 43, BONUS_STAGE_TEXT = 44, BONUS_STAGE_TILE_A = 45, BONUS_STAGE_TILE_B = 46, BONUS_STAGE_TILE_C = 47, EGGPLANT = 48, CARROT = 49, CABBAGE = 50, ZUCCHINI = 51, CORN = 52, TURNIP = 53, PUMPKIN = 54, ROMAINE_LETTUCE = 55, POTATO = 56, MUSHROOM = 57, ONE = 58, TWO = 59, THREE = 60, FOUR = 61, FIVE = 62, SIX = 63, SEVEN = 64, EIGHT = 65, NITPICKER = 66, CONDOR = 67, GAME_OVER = 68 };

// Object type
enum EntityType: uint16_t { TERRAIN = 0, PLAYER = 1, ENEMY = 2, VEGETABLE = 3, NUMBER = 4, CLOUD = 5 };

// Underlying object surface type
enum SurfaceType: uint16_t { SIMPLE = 0, SLIDING = 1, MOBILE_RIGHT = 2, MOBILE_LEFT = 3 };

// Object movement direction
enum Direction: uint8_t { RIGHT = 0, LEFT = 1 };

// Object animation identificators
enum PlayerAnimation: uint16_t { STAND_BY_RIGHT = 0, STAND_BY_LEFT = 1, RUN_TO_RIGHT = 2, RUN_TO_LEFT = 3, JUMP_RIGHT = 4, JUMP_LEFT = 5, FALL_RIGHT = 6, FALL_LEFT = 7, HIT_RIGHT = 8, HIT_LEFT = 9, SLIP_TO_RIGHT = 10, SLIP_TO_LEFT = 11, JUMP_DEAD = 12, RESPAWN_RIGHT = 13, RESPAWN_LEFT = 14 };
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
enum IceAnimation: uint16_t { ICE_STICKY = 68, ICE_DESTROY = 69 };
enum WaterAnimation: uint16_t { WATER_STICKY = 70 };
enum BonusStageTextAnimation: uint16_t { BONUS_STAGE_TEXT_STICKY = 71 };
enum BonusStageTileAAnimation: uint16_t { BONUS_STAGE_TILE_A_STICKY = 72 };
enum BonusStageTileBAnimation: uint16_t { BONUS_STAGE_TILE_B_STICKY = 73 };
enum BonusStageTileCAnimation: uint16_t { BONUS_STAGE_TILE_C_STICKY = 74 };
enum EggplantAnimation: uint16_t { EGGPLANT_STICKY = 75 };
enum CarrotAnimation: uint16_t { CARROT_STICKY = 76 };
enum CabbageAnimation: uint16_t { CABBAGE_STICKY = 77 };
enum ZucchiniAnimation: uint16_t { ZUCCHINI_STICKY = 78 };
enum CornAnimation: uint16_t { CORN_STICKY = 79 };
enum TurnipAnimation: uint16_t { TURNIP_STICKY = 80 };
enum PumpkinAnimation: uint16_t { PUMPKIN_STICKY = 81 };
enum RomaineLettuceAnimation: uint16_t { ROMAINE_LETTUCE_STICKY = 82 };
enum PotatoAnimation: uint16_t { POTATO_STICKY = 83 };
enum MushroomAnimation: uint16_t { MUSHROOM_STICKY = 84 };
enum OneAnimation: uint16_t { ONE_STICKY = 85 };
enum TwoAnimation: uint16_t { TWO_STICKY = 86 };
enum ThreeAnimation: uint16_t { THREE_STICKY = 87 };
enum FourAnimation: uint16_t { FOUR_STICKY = 88 };
enum FiveAnimation: uint16_t { FIVE_STICKY = 89 };
enum SixAnimation: uint16_t { SIX_STICKY = 90 };
enum SevenAnimation: uint16_t { SEVEN_STICKY = 91 };
enum EightAnimation: uint16_t { EIGHT_STICKY = 92 };
enum NitpickerAnimation: uint16_t { NITPICKER_FLY_TO_RIGHT = 93, NITPICKER_FLY_TO_LEFT = 94, NITPICKER_FALL_RIGHT = 95, NITPICKER_FALL_LEFT = 96 };
enum CondorAnimation: uint16_t { CONDOR_FLY_TO_RIGHT = 97, CONDOR_FLY_TO_LEFT = 98 };
enum GameOverAnimation: uint16_t { GAME_OVER_MOVE_UP = 99 };

// Others
constexpr bool DEBUG = false;                                                    // Debug mode.
constexpr int FPS = 60;                                                          // Frames per second.
constexpr int MILLISECONDS_PER_TICK = 16;                                        // Game logic frequency in milliseconds. 16ms ≈ 60 ticks per second.
constexpr char* ENTITY_TYPES_FILENAME = "objtypes.dat";                          // The data file where entity animations and collision areas are defined.
constexpr char* HIGHSCORE_FILENAME = "highscore.dat";                            // The data file where the highest score is stored.
constexpr char* MOUNTAIN_FILENAME = "levels/mountain?.dat";                      // The data file where mountain information is stored.
constexpr char* MAIN_MENU_MUSIC_FILENAME = "audio/title_screen.mp3";             // Background music for the title screen menu.
constexpr char* GAME_PLAY_MUSIC_FILENAME = "audio/stage_theme.mp3";              // Background music for the mountain game play.
constexpr char* STAGE_CLEAR_MUSIC_FILENAME = "audio/stage_clear.mp3";            // Background music for the stage score summary screen.
constexpr float GRAVITY = 9.81f;                                                 // Gravity value used for physics calculations.
constexpr int MIN_PIXELS_ON_UNDERLYING_SURFACE = 4;                              // Min number of pixels on the edge of an underlying surface to do not fall.
constexpr int SLIPPING_DISTANCE = 30;                                            // The distance, in pixels, the player slips when suddenly stopping on an ice floor.
constexpr int CELL_WIDTH = 16;                                                   // The width, in pixels, of a map cell.
constexpr int CELL_HEIGHT = 16;                                                  // The height, in pixels, of a map cell.
constexpr float CELL_WIDTH_FLOAT = 16.0f;
constexpr float CELL_HEIGHT_FLOAT = 16.0f;
constexpr int MOUNTAIN_VIEWPORT_HEIGHT_CELLS = 31;                               // Vertical viewport of the mountain in cells
constexpr int MOUNTAIN_HEIGHT_CELLS = 107;                                       // The height of a mountain in number of cells
constexpr int MOUNTAIN_WIDTH_CELLS = 32;                                         // The width of a mountain in number of cells.
constexpr int MOUNTAIN_WIDTH = MOUNTAIN_WIDTH_CELLS * CELL_WIDTH;                // The integer width in pixels of a mountain. 32cells x 16px.
constexpr float MOUNTAIN_WIDTH_FLOAT = MOUNTAIN_WIDTH_CELLS * CELL_WIDTH_FLOAT;  // The float width in pixels of a mountain. 32cells x 16px.
constexpr float TOPI_MOUNTAIN_RIGHT_EDGE_MARGIN = 5.0f;
constexpr float NITPICKER_RESPAWN_TOP_MARGIN = 65.0f;
constexpr long long NITPICKER_RESPAWN_WAIT_TIME_MILLISECONDS = 5000;             // Nitpicker waiting time (in milliseconds) before respawn.
constexpr float NITPICKER_MAX_SPEED = 0.6;                                       // Nitpicker max component value of speed vector.
constexpr float NITPICKER_ACCELERATION = 0.01;                                   // Nitpicker acceleration when flying.
constexpr int NITPICKER_NUM_WAYPOINTS = 4;                                       // Number of flight path points the Nitpicker should travel.
constexpr int NITPICKER_MAX_CONSECUTIVE_ATTACK_ATTEMPS = 3;                      // Number of consecutive Nitpicker attack attempts during a flight.
constexpr int NITPICKER_MAX_FALL_CELLS = 50;                                     // Nitpicker fall distance in number of cells before respawn.
constexpr float INITIAL_CAMERA_POSITION = 78 * CELL_HEIGHT_FLOAT;                // Initial camera vertical position when player is at level 1.
constexpr int BONUS_STAGE_CELL_Y = 50;                                           // Vertical cell position where bonus stage starts.
constexpr float CAMERA_PADDING_TOP = 13 * CELL_HEIGHT_FLOAT;                     // Camera viewport top padding space.
constexpr float CAMERA_BONUS_STAGE_PADDING_TOP = 19 * CELL_HEIGHT_FLOAT;         // Camera viewport top padding space in the bonus stage.
constexpr float CAMERA_SPEED = 3.0f;                                             // Camera speed in vertical píxels per tick.
constexpr Rectangle LIFE_COUNTER_SPRITE_RECT = {96, 248, 16, 16};                // Life Counter sprite coordinates in the tilemap.
constexpr float LIFE_COUNTER_X = 112.0f;                                         // Life Counter X coordinate in the screen.
constexpr float LIFE_COUNTER_Y = 64.0f;                                          // Life Counter Y coordinate in the screen.
constexpr int MAX_PLAYER_LIFES = 3;                                              // Total number of lifes of the player.
constexpr int PAUSE_DURATION_BEFORE_GAME_FINISHED_NOTIFICATION = 2000;           // Pause duration in milliseconds before the gameplay finishes, due to Condor hunted or player death.
constexpr float VEGETABLE_SPRITE_WIDTH = 32.0f;                                  // Width in pixels of a vegetable sprite in the tilemap.
constexpr float EGGPLANT_VEGETABLE_X = 112.0f;                                   // X coordinate of the eggplant sprite in the tilemap.
constexpr float NUMBER_SPRITE_WIDTH = 16.0f;                                     // Width in pixels of a number sprite in the tilemap.
constexpr float ZERO_NUMBER_X = 560.0f;                                          // X coordinate of the zero-number sprite in the tilemap.
constexpr int TOTAL_MOUNTAINS = 32;                                              // Numer of mountains available to play.