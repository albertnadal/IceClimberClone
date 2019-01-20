#include "main_character.h"
#include <chrono>

enum MainCharacterAnimation: uint16 { STAND_BY_RIGHT = 0, STAND_BY_LEFT = 1, WALK_TO_RIGHT = 2, WALK_TO_LEFT = 3 };

class MainCharacterIdleState;
class MainCharacterRunToRightState;

MainCharacter::MainCharacter():
  StateMachine(MainCharacterStateIdentificator::MAIN_CHARACTER_MAX_STATES),
  ISceneObject(SceneObjectIdentificator::MAIN_CHARACTER) {
        cout << " INSTANCIA NOVA DE MainCharacter CREADA" << endl;
        //stateMachine.transit<MainCharacterIdleState>();
        position.x = 100;
        position.y = 100;
}

uint16 MainCharacter::Width() {
        return currentSprite.width;
}

uint16 MainCharacter::Height() {
        return currentSprite.height;
}

void MainCharacter::PrintName(){
        std::cout << "Main character." << std::endl;
}

bool MainCharacter::Update(uchar pressedKeys) {

        if(pressedKeys != KeyboardKeyCode::KEY_NONE) {
                // character in action
                ProcessPressedKeys(pressedKeys);
        } else if(pressedKeys != prevPressedKeys) {
                // character goes quiet headed in the proper direction
                LoadAnimationWithId(headedToRight ? MainCharacterAnimation::STAND_BY_RIGHT : MainCharacterAnimation::STAND_BY_LEFT);
                prevPressedKeys = KeyboardKeyCode::KEY_NONE;
        }

        if(!animationLoaded) {
                return false;
        }

        if(animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
                return false;
        }

        if(std::chrono::system_clock::now() >= nextSpriteTime) {
                // Load next sprite of the current animation
                SpriteData spriteData = NextSpriteData();
                nextSpriteTime = (std::chrono::system_clock::now() + std::chrono::milliseconds(spriteData.duration));

                currentSprite.width = spriteData.width;
                currentSprite.height = spriteData.height;
                currentSprite.u1 = spriteData.u1;
                currentSprite.v1 = spriteData.v1;
                currentSprite.u2 = spriteData.u2;
                currentSprite.v2 = spriteData.v2;

                firstSpriteOfCurrentAnimationIsLoaded = true;
                return true;
        }

        return false;
}

void MainCharacter::ProcessPressedKeys(uchar pressedKeys)
{
        if((pressedKeys & KeyboardKeyCode::KEY_RIGHT) == KeyboardKeyCode::KEY_RIGHT) {
                // If is not KEY_RIGHT repeated press then change character state
                if((prevPressedKeys & KeyboardKeyCode::KEY_RIGHT) != KeyboardKeyCode::KEY_RIGHT) {
                        //transit<MainCharacterRunToRightState>();
                        //stateMachine.dispatch(KeyRightPressedEvent());
                        LoadAnimationWithId(MainCharacterAnimation::WALK_TO_RIGHT);
                        headedToRight = true;
                }
                position.x+=3;
        }

        if((pressedKeys & KeyboardKeyCode::KEY_LEFT) == KeyboardKeyCode::KEY_LEFT) {
                // If is not KEY_LEFT repeated press then change character state
                if((prevPressedKeys & KeyboardKeyCode::KEY_LEFT) != KeyboardKeyCode::KEY_LEFT) {
                        LoadAnimationWithId(MainCharacterAnimation::WALK_TO_LEFT);
                        headedToRight = false;
                }
                position.x-=3;
        }

        prevPressedKeys = pressedKeys;
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

SpriteData MainCharacter::NextSpriteData()
{
        if(currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
                currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        }
        return *currentAnimationSpriteIterator++;
}

ISceneObject* MainCharacter::Create() {
        return new MainCharacter();
}

MainCharacter::~MainCharacter() {

}

// halt motor external event
void MainCharacter::Halt(void)
{
/*
    // given the Halt event, transition to a new state based upon
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      // - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)  // ST_Idle
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)  // ST_Stop
        TRANSITION_MAP_ENTRY (STATE_STOP)     // ST_Start
        TRANSITION_MAP_ENTRY (STATE_STOP)     // ST_ChangeSpeed
    END_TRANSITION_MAP(NULL)
*/
}

// set motor speed external event
void MainCharacter::SetSpeed(MotorData* pData)
{
/*
    BEGIN_TRANSITION_MAP                          // - Current State -
        TRANSITION_MAP_ENTRY (STATE_START)        // ST_Idle
        TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)      // ST_Stop
        TRANSITION_MAP_ENTRY (STATE_CHANGE_SPEED) // ST_Start
        TRANSITION_MAP_ENTRY (STATE_CHANGE_SPEED) // ST_ChangeSpeed
    END_TRANSITION_MAP(pData)
*/
}

// state machine sits here when motor is not running
void MainCharacter::STATE_Idle_Right(EventData* pData)
{
	cout << "Motor::STATE_Idle_Right" << endl;
}

// stop the motor
void MainCharacter::STATE_Idle_Left(EventData* pData)
{
	cout << "Motor::STATE_Idle_Left" << endl;
/*
    // perform the stop motor processing here
    // transition to ST_Idle via an internal event
    InternalEvent(STATE_IDLE);
*/
}

// start the motor going
void MainCharacter::STATE_Run_Right(MotorData* pData)
{
	cout << "Motor::STATE_Run_Right" << endl;
    // set initial motor speed processing here
}

// changes the motor speed once the motor is moving
void MainCharacter::STATE_Run_Left(MotorData* pData)
{
	cout << "Motor::STATE_Run_Left" << endl;
    // perform the change motor speed to pData->speed here
}

// start the motor going
void MainCharacter::STATE_Fast_Run_Right(MotorData* pData)
{
	cout << "Motor::STATE_Fast_Run_Right" << endl;
    // set initial motor speed processing here
}

// changes the motor speed once the motor is moving
void MainCharacter::STATE_Fast_Run_Left(MotorData* pData)
{
	cout << "Motor::STATE_Fast_Run_Left" << endl;
    // perform the change motor speed to pData->speed here
}
