#include "main_character.h"
#include <chrono>

enum MainCharacterAnimation: uint16 { STAND_BY_RIGHT = 0, STAND_BY_LEFT = 1, RUN_TO_RIGHT = 2, RUN_TO_LEFT = 3, JUMP_IDLE_RIGHT = 4, JUMP_IDLE_LEFT = 5, JUMP_RUN_RIGHT = 6, JUMP_RUN_LEFT = 7 };

class MainCharacterIdleState;
class MainCharacterRunToRightState;

MainCharacter::MainCharacter() :
        ISceneObject(SceneObjectIdentificator::MAIN_CHARACTER, MainCharacterStateIdentificator::MAIN_CHARACTER_MAX_STATES) {
        cout << " INSTANCIA NOVA DE MainCharacter CREADA" << endl;
        position.x = 100;
        position.y = 100;
        //speed = 0.0f;
}

uint16 MainCharacter::Width() {
        return currentSprite.width;
}

uint16 MainCharacter::Height() {
        return currentSprite.height;
}

void MainCharacter::PrintName() {
        std::cout << "Main character." << std::endl;
}

bool MainCharacter::Update(uchar pressedKeys_) {

        pressedKeys = pressedKeys_;

        if(pressedKeys != KeyboardKeyCode::KEY_NONE) {
                ProcessPressedKeys();
        } else if(pressedKeys != prevPressedKeys) {
                ProcessReleasedKeys();
        }

        if(!animationLoaded) {
                return false;
        }

        if(animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
                return false;
        }

        if(std::chrono::system_clock::now() >= nextSpriteTime) {
                // Load next sprite of the current animation
                LoadNextSprite();
                return true;
        }

        return false;
}

void MainCharacter::ProcessPressedKeys(bool checkPreviousPressedKeys)
{
        // User pressed KEY_RIGHT
        if((pressedKeys & KeyboardKeyCode::KEY_RIGHT) == KeyboardKeyCode::KEY_RIGHT) {
                // If is not KEY_RIGHT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_RIGHT) != KeyboardKeyCode::KEY_RIGHT))) {
                        //transit<MainCharacterRunToRightState>();
                        //stateMachine.dispatch(KeyRightPressedEvent());
//                        LoadAnimationWithId(MainCharacterAnimation::RUN_TO_RIGHT);
                        headedToRight = true;
                        RightKeyPressed();
                }
                position.addX(2.5f);
/*
                speed+=0.2f;
                cout << "SPEED: " << speed << endl;
                if((int)speed == 24 ) {
                  cout << "ReachedSpeedForRunning()" << endl;
                  ReachedSpeedForRunning();
                }
*/

        } else if((prevPressedKeys & KeyboardKeyCode::KEY_RIGHT) == KeyboardKeyCode::KEY_RIGHT) {
                RightKeyReleased();
                //speed = 0.0f;
        }

        if((pressedKeys & KeyboardKeyCode::KEY_LEFT) == KeyboardKeyCode::KEY_LEFT) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_LEFT) != KeyboardKeyCode::KEY_LEFT))) {
                        cout << "KEY LEFT PRESSED" << endl;
//                        LoadAnimationWithId(MainCharacterAnimation::RUN_TO_LEFT);
                        headedToRight = false;
                        LeftKeyPressed();
                }
                position.addX(-2.5f);
/*
                speed+=0.2f;
                cout << "SPEED: " << speed << endl;
                if((int)speed == 24 ) {
                  cout << "ReachedSpeedForRunning()" << endl;
                  ReachedSpeedForRunning();
                }
*/
        } else if((prevPressedKeys & KeyboardKeyCode::KEY_LEFT) == KeyboardKeyCode::KEY_LEFT) {
                cout << "KEY LEFT RELEASED" << endl;
                LeftKeyReleased();
                //speed = 0.0f;
        }

        if((pressedKeys & KeyboardKeyCode::KEY_UP) == KeyboardKeyCode::KEY_UP) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_UP) != KeyboardKeyCode::KEY_UP))) {
                        cout << "KEY YP PRESSED" << endl;
                        UpKeyPressed();
                }
        }

        prevPressedKeys = pressedKeys;
}

void MainCharacter::ProcessReleasedKeys() {
        if((prevPressedKeys & KeyboardKeyCode::KEY_RIGHT) == KeyboardKeyCode::KEY_RIGHT) {
                RightKeyReleased();
        }

        if((prevPressedKeys & KeyboardKeyCode::KEY_LEFT) == KeyboardKeyCode::KEY_LEFT) {
                LeftKeyReleased();
        }

        // character goes quiet headed in the proper direction
//  LoadAnimationWithId(headedToRight ? MainCharacterAnimation::STAND_BY_RIGHT : MainCharacterAnimation::STAND_BY_LEFT);

        prevPressedKeys = KeyboardKeyCode::KEY_NONE;
}

void MainCharacter::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(MainCharacterAnimation::STAND_BY_RIGHT);
}

void MainCharacter::LoadAnimationWithId(uint16 animationId) {
        ObjectSpriteSheetAnimation *currentAnimation = spriteSheet->GetAnimationWithId(animationId);
        currentAnimationSprites = currentAnimation->GetSprites();
        animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        animationLoaded = true;
        firstSpriteOfCurrentAnimationIsLoaded = false;
        nextSpriteTime = std::chrono::system_clock::now();
}

void MainCharacter::LoadNextSprite()
{
  SpriteData spriteData = NextSpriteData();
  if(spriteData.beginNewLoop) {
          cout << "*** BEGIN NEW LOOP ***" << endl;
          if(BeginAnimationLoopAgain()) {
            spriteData = NextSpriteData();
          }
  }

  nextSpriteTime = (std::chrono::system_clock::now() + std::chrono::milliseconds(spriteData.duration));

  currentSprite.width = spriteData.width;
  currentSprite.height = spriteData.height;
  currentSprite.u1 = spriteData.u1;
  currentSprite.v1 = spriteData.v1;
  currentSprite.u2 = spriteData.u2;
  currentSprite.v2 = spriteData.v2;

  firstSpriteOfCurrentAnimationIsLoaded = true;
}

SpriteData MainCharacter::NextSpriteData()
{
        if(currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
                currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
                (*currentAnimationSpriteIterator).beginNewLoop = true;
        }

        return *currentAnimationSpriteIterator++;
}

ISceneObject* MainCharacter::Create() {
        return new MainCharacter();
}

MainCharacter::~MainCharacter() {

}

bool MainCharacter::BeginAnimationLoopAgain()
{
/*
        if(currentState == MainCharacterStateIdentificator::STATE_CHANGE_DIRECTION_RIGHT) {
          cout << "CANVIANT A IDLE RIGHT" << endl;
          ExternalEvent(MainCharacterStateIdentificator::STATE_IDLE_RIGHT, NULL);
          return true;
        } else if(currentState == MainCharacterStateIdentificator::STATE_CHANGE_DIRECTION_LEFT) {
          cout << "CANVIANT A IDLE LEFT" << endl;
          ExternalEvent(MainCharacterStateIdentificator::STATE_IDLE_LEFT, NULL);
          return true;
        } else {
          return false;
        }
*/
  return false;
}
/*
void MainCharacter::ReachedSpeedForRunning()
{
  cout << "MainCharacter::ReachedSpeedForRunning()" << endl;
  BEGIN_TRANSITION_MAP                    // - Current State -
  TRANSITION_MAP_ENTRY (EVENT_IGNORED) // STATE_Idle_Right
  TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Left
  TRANSITION_MAP_ENTRY (STATE_FAST_RUN_RIGHT)    // STATE_Run_Right
  TRANSITION_MAP_ENTRY (STATE_FAST_RUN_LEFT)    // STATE_Run_Left
  TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
  TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
  TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
  TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
  END_TRANSITION_MAP(NULL)
}
*/
void MainCharacter::RightKeyPressed()
{
        cout << "MainCharacter::RightKeyPressed()" << endl;
        BEGIN_TRANSITION_MAP                    // - Current State -
        TRANSITION_MAP_ENTRY (STATE_RUN_RIGHT) // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_RUN_RIGHT)  // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::RightKeyReleased()
{
        cout << "MainCharacter::RightKeyReleased()" << endl;
        BEGIN_TRANSITION_MAP                    // - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED) // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::LeftKeyPressed()
{
        cout << "MainCharacter::LeftKeyPressed()" << endl;
        BEGIN_TRANSITION_MAP                                  // - Current State -
        TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)                 // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_RUN_LEFT)                 // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::LeftKeyReleased()
{
        cout << "MainCharacter::LeftKeyReleased()" << endl;
        BEGIN_TRANSITION_MAP                    // - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED) // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Run_Right
        TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::UpKeyPressed()
{
        cout << "MainCharacter::UpKeyPressed()" << endl;
        BEGIN_TRANSITION_MAP                                  // - Current State -
        TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_RIGHT)          // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_JUMP_IDLE_LEFT)                 // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (STATE_JUMP_RUN_RIGHT)                  // STATE_Run_Right
        TRANSITION_MAP_ENTRY (STATE_JUMP_RUN_LEFT)                  // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::STATE_Idle_Right()
{
        cout << "MainCharacter::STATE_Idle_Right" << endl;
        LoadAnimationWithId(MainCharacterAnimation::STAND_BY_RIGHT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Idle_Left()
{
        cout << "MainCharacter::STATE_Idle_Left" << endl;
        LoadAnimationWithId(MainCharacterAnimation::STAND_BY_LEFT);
        cout << "ProcessPressedKeys(false)" << endl;
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Run_Right()
{
        cout << "MainCharacter::STATE_Run_Right" << endl;
        LoadAnimationWithId(MainCharacterAnimation::RUN_TO_RIGHT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Run_Left()
{
        cout << "MainCharacter::STATE_Run_Left" << endl;
        LoadAnimationWithId(MainCharacterAnimation::RUN_TO_LEFT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Jump_Idle_Right()
{
        cout << "MainCharacter::STATE_Jump_Idle_Right" << endl;
        LoadAnimationWithId(MainCharacterAnimation::JUMP_IDLE_RIGHT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Jump_Idle_Left()
{
        cout << "MainCharacter::STATE_Jump_Idle_Left" << endl;
        LoadAnimationWithId(MainCharacterAnimation::JUMP_IDLE_LEFT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Jump_Run_Right()
{
        cout << "MainCharacter::STATE_Jump_Run_Right" << endl;
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Jump_Run_Left()
{
        cout << "MainCharacter::STATE_Jump_Run_Left" << endl;
        ProcessPressedKeys(false);
}
/*
void MainCharacter::STATE_Change_Direction_Left()
{
        cout << "MainCharacter::STATE_Change_Direction_Left" << endl;
        LoadAnimationWithId(MainCharacterAnimation::DIRECTION_TO_LEFT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Change_Direction_Right()
{
        cout << "MainCharacter::STATE_Change_Direction_Right" << endl;
        LoadAnimationWithId(MainCharacterAnimation::DIRECTION_TO_RIGHT);
        ProcessPressedKeys(false);
}*/
