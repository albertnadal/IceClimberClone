#include "main_character.h"
#include <chrono>

MainCharacter::MainCharacter() :
        ISceneObject(SceneObjectIdentificator::MAIN_CHARACTER, MainCharacterStateIdentificator::MAIN_CHARACTER_MAX_STATES) {
        cout << " INSTANCIA NOVA DE MainCharacter CREADA" << endl;
        position.setX(100.0f);
        position.setY(100.0f);
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

        bool needRedraw = false;

        if(isJumping) {
          UpdateJump();
          needRedraw = true;
        }

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

        return needRedraw;
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
                MoveTo(MainCharacterDirection::RIGHT);
        } else if((prevPressedKeys & KeyboardKeyCode::KEY_RIGHT) == KeyboardKeyCode::KEY_RIGHT) {
                RightKeyReleased();
        }

        if((pressedKeys & KeyboardKeyCode::KEY_LEFT) == KeyboardKeyCode::KEY_LEFT) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_LEFT) != KeyboardKeyCode::KEY_LEFT))) {
                        cout << "KEY LEFT PRESSED" << endl;
//                        LoadAnimationWithId(MainCharacterAnimation::RUN_TO_LEFT);
                        headedToRight = false;
                        LeftKeyPressed();
                }
                MoveTo(MainCharacterDirection::LEFT);
        } else if((prevPressedKeys & KeyboardKeyCode::KEY_LEFT) == KeyboardKeyCode::KEY_LEFT) {
                cout << "KEY LEFT RELEASED" << endl;
                LeftKeyReleased();
        }

        if(!isJumping && ((pressedKeys & KeyboardKeyCode::KEY_UP) == KeyboardKeyCode::KEY_UP)) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_UP) != KeyboardKeyCode::KEY_UP))) {
                        cout << "KEY UP PRESSED" << endl;
                        UpKeyPressed();
                }
        }

        if(!isJumping && !isHitting && ((pressedKeys & KeyboardKeyCode::KEY_SPACE) == KeyboardKeyCode::KEY_SPACE)) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_SPACE) != KeyboardKeyCode::KEY_SPACE))) {
                        cout << "KEY SPACE PRESSED" << endl;
                        isHitting = true;
                        SpaceKeyPressed();
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
        if(currentState == MainCharacterStateIdentificator::STATE_HIT_RIGHT) {
          isHitting = false;
          ExternalEvent(MainCharacterStateIdentificator::STATE_IDLE_RIGHT, NULL);
          return true;
        } else if(currentState == MainCharacterStateIdentificator::STATE_HIT_LEFT) {
          isHitting = false;
          ExternalEvent(MainCharacterStateIdentificator::STATE_IDLE_LEFT, NULL);
          return true;
        } else {
          return false;
        }
}

void MainCharacter::UpdateJump() {
  tJump+=0.2f;
  position.setX(hInitialJumpPosition + (hInitialJumpSpeed * tJump));
  float vOffset = (vInitialJumpSpeed * tJump - (0.5f)*gravity*tJump*tJump);
  if(vOffset <= 0.0f) {
    // Jump landing
    position.setY(vInitialJumpPosition);
    isJumping = false;
    hMomentum = 0;
    JumpLanding();
  } else {
    position.setY(vInitialJumpPosition + vOffset);
  }
}

void MainCharacter::MoveTo(MainCharacterDirection direction)
{
  if(!isJumping && !isHitting) {
    position.addX(direction == MainCharacterDirection::RIGHT ? 4.0f : -4.0f);
    if(hMomentum < maxMomentum) {
      hMomentum++;
    }
  }
}

void MainCharacter::Jump(float vSpeed, float hSpeed)
{
  cout << "MainCharacter::Jump" << endl;
  vInitialJumpSpeed = vSpeed;
  hInitialJumpSpeed = hSpeed;
  vInitialJumpPosition = position.y;
  hInitialJumpPosition = position.x;
  cout << "Initial JUMP X: " << hInitialJumpPosition << " Initial JUMP Y: " << vInitialJumpPosition << endl;
  tJump = 0.0f;
  isJumping = true;
}

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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::SpaceKeyPressed()
{
        cout << "MainCharacter::UpKeyPressed()" << endl;
        BEGIN_TRANSITION_MAP                                  // - Current State -
        TRANSITION_MAP_ENTRY (STATE_HIT_RIGHT)          // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_HIT_LEFT)                 // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (STATE_HIT_RIGHT)                  // STATE_Run_Right
        TRANSITION_MAP_ENTRY (STATE_HIT_LEFT)                  // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::JumpLanding()
{
        cout << "MainCharacter::JumpLanding()" << endl;
        BEGIN_TRANSITION_MAP                                  // - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                 // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
        TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::STATE_Idle_Right()
{
        cout << "MainCharacter::STATE_Idle_Right" << endl;
        hMomentum = 0;
        LoadAnimationWithId(MainCharacterAnimation::STAND_BY_RIGHT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Idle_Left()
{
        cout << "MainCharacter::STATE_Idle_Left" << endl;
        hMomentum = 0;
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
    Jump(45.0f, 0.0f);
    LoadAnimationWithId(MainCharacterAnimation::JUMP_RIGHT);
    ProcessPressedKeys(false);
}

void MainCharacter::STATE_Jump_Idle_Left()
{
    Jump(45.0f, 0.0f);
    LoadAnimationWithId(MainCharacterAnimation::JUMP_LEFT);
    ProcessPressedKeys(false);
}

void MainCharacter::STATE_Jump_Run_Right()
{
    // More momentum produces a longer jump
    Jump(45.0f, hMomentum == maxMomentum ? 10.0f : 4.0f);
    LoadAnimationWithId(MainCharacterAnimation::JUMP_RIGHT);
    ProcessPressedKeys(false);
}

void MainCharacter::STATE_Jump_Run_Left()
{
  // More momentum produces a longer jump
  Jump(45.0f, hMomentum == maxMomentum ? -10.0f : -4.0f);
  LoadAnimationWithId(MainCharacterAnimation::JUMP_LEFT);
  ProcessPressedKeys(false);
}

void MainCharacter::STATE_Hit_Right()
{
        cout << "MainCharacter::STATE_Hit_Right" << endl;
        LoadAnimationWithId(MainCharacterAnimation::HIT_RIGHT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Hit_Left()
{
        cout << "MainCharacter::STATE_Hit_Left" << endl;
        LoadAnimationWithId(MainCharacterAnimation::HIT_LEFT);
        ProcessPressedKeys(false);
}
