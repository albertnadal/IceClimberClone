#include "items/main_character.h"
#include <chrono>

struct ObjectCollisionData { ISceneObject* object; float penetrationDepth; collision::vec2<float> penetrationNormal; };

MainCharacter::MainCharacter() :
        ISceneObject(SceneObjectIdentificator::MAIN_CHARACTER, SceneObjectType::PLAYER, MainCharacterStateIdentificator::MAIN_CHARACTER_MAX_STATES) {
          //Initially the object is quiet
          vectorDirection.x = 0.0f;
          vectorDirection.y = 0.0f;
}

uint16_t MainCharacter::Width() {
        return currentSprite.width;
}

uint16_t MainCharacter::Height() {
        return currentSprite.height;
}

void MainCharacter::PrintName() {
        std::cout << "Main character." << std::endl;
}

bool MainCharacter::Update(const uint8_t pressedKeys_, aabb::Tree<ISceneObject*>& spacePartitionObjectsTree_) {

        bool needRedraw = false;

        if(isJumping) {
          UpdateJump();
          needRedraw = true;
        } else if(isFalling) {
          UpdateFall();
          needRedraw = true;
        }

        pressedKeys = pressedKeys_;

        if(pressedKeys != KeyboardKeyCode::KEY_NONE) {
                ProcessPressedKeys();
        } else if(pressedKeys != prevPressedKeys) {
                ProcessReleasedKeys();
        }

        // CHECK FOR COLLISIONS
        if(currentSprite.areas != nullptr) {
          // Search for collisions with solid objects
          std::vector<ObjectCollisionData> collidingSolidObjects;

          // First check for possible potential collision candidates objects
          std::vector<ISceneObject*> potentialCollisionCandidatesObjects = spacePartitionObjectsTree_.query(GetLowerBound(), GetUpperBound());

          uint16_t potentialCollisionObjectsCount = potentialCollisionCandidatesObjects.size();
          if(potentialCollisionObjectsCount) {
            // Iterate all potential collision candidates and check for real collisions
            collision::GJKCollisionDetector collisionDetector;
            for(uint16_t i=0; i<potentialCollisionObjectsCount; i++) {
              ISceneObject* collisionCandidateObject = potentialCollisionCandidatesObjects[i];
              if(collisionCandidateObject != this) {
//                cout << "#" << i+1 << "/" << potentialCollisionObjectsCount << "POTENTIAL COLLISION WITH OBJECT:" << endl;
//                collisionCandidateObject->PrintName();
//                collisionCandidateObject->PrintBoundaries();

                // Check precise collision of every solid area of the collision candidate object with every solid area of the main character
                std::vector<Area>& collisionCandidateObjectSolidAreas = collisionCandidateObject->GetSolidAreas();
//                cout << "COLLISION OBJECT CANDIDATE HAVE " << collisionCandidateObjectSolidAreas.size() << " SOLID AREAS" << endl;
                for(uint16_t c=0; c<collisionCandidateObjectSolidAreas.size(); c++) {
                  collision::Polygon candidateSolidAreaPolygon = collisionCandidateObjectSolidAreas.at(c).polygon;
//                  candidateSolidAreaPolygon.Print();

                  // Check collision with all main character solid areas
                  std::vector<Area>& mainCharacterSolidAreas = GetSolidAreas();
//                  cout << "MAIN CHARACTER OBJECT HAVE " << mainCharacterSolidAreas.size() << " SOLID AREAS" << endl;
                  for(uint16_t s=0; s<mainCharacterSolidAreas.size(); s++) {
                    uint16_t areaId = mainCharacterSolidAreas.at(s).id;
                    collision::Polygon mainCharacterSolidAreaPolygon = mainCharacterSolidAreas.at(s).polygon;
//                    mainCharacterSolidAreaPolygon.Print();

                    collision::Penetration penetration;
                    bool collision = collisionDetector.detect(mainCharacterSolidAreaPolygon, candidateSolidAreaPolygon, penetration);
//                    cout << "Do we have a collision between main character and candidate collision object: " << collision << endl;
                    if(collision) {
                      collidingSolidObjects.push_back({collisionCandidateObject, penetration.depth, penetration.normal});
                    }
                  }
                }
              }
            }

          } else {
            //cout << " >> NO POTENTIAL COLLISION WITH ANY OBJECT" << endl;
          }

          // Process colliding objects
          uint16_t collidingSolidObjectsCount = collidingSolidObjects.size();
          if(collidingSolidObjectsCount) {
            float maxHorizontalPenetrationDepth = 0.0f;
            float maxVerticalPenetrationDepth = 0.0f;
            std::cout << " > > > > > > > > >" << std::endl;
            for(uint16_t i=0; i<collidingSolidObjectsCount; i++) {
              ObjectCollisionData objectCollisionData = collidingSolidObjects.at(i);
              std::cout << " - - - - - - -" << std::endl;
              std::cout << " ----- penetration depth: " << objectCollisionData.penetrationDepth << std::endl;
              std::cout << " ----- penetration normal: (" << objectCollisionData.penetrationNormal.x << "," << objectCollisionData.penetrationNormal.y << ")" << std::endl;

              if(objectCollisionData.penetrationNormal.y ) {
                float verticalPenetrationDepth = objectCollisionData.penetrationNormal.y * objectCollisionData.penetrationDepth;
                if(std::abs(verticalPenetrationDepth) > std::abs(maxVerticalPenetrationDepth)) {
                  maxVerticalPenetrationDepth = verticalPenetrationDepth;
                }
              } else if(objectCollisionData.penetrationNormal.x ) {
                float horizontalPenetrationDepth = objectCollisionData.penetrationNormal.x * objectCollisionData.penetrationDepth;
                if(std::abs(horizontalPenetrationDepth) > std::abs(maxHorizontalPenetrationDepth)) {
                  maxHorizontalPenetrationDepth = horizontalPenetrationDepth;
                }
              }
            }
            std::cout << " COLLISION >>>>>>>>>>>>>>>>>>>> HORIZONTAL OFFSET: " << maxHorizontalPenetrationDepth << std::endl;
            // Apply horizontal possition corrective
            PositionAddX(-maxHorizontalPenetrationDepth);
            std::cout << " COLLISION >>>>>>>>>>>>>>>>>>>> VERTICAL OFFSET: " << maxVerticalPenetrationDepth << std::endl;
            // Apply vertical possition corrective
            PositionAddY(-maxVerticalPenetrationDepth);

            // Colliding during jump causes finish jump and fall
            if(isJumping) {
              //FinishJump();
              std::cout << " -------------- COLLISION DURING JUMPING VERTICAL PENETRATION: " << maxVerticalPenetrationDepth << std::endl;
              std::cout << " -------------- COLLISION DURING JUMPING HORIZONTAL PENETRATION: " << maxHorizontalPenetrationDepth << std::endl;

              // Check if collision has been detected on top of the main character during jumping
              if(maxVerticalPenetrationDepth>0.0f) {
                // Causes main character fall
                TopCollisionDuringJump();
              } else {
                // Causes maion character lay on the ground
                FinishJump();
              }
            } else if(isFalling) {
              std::cout << " -------------- COLLISION DURING FALL VERTICAL PENETRATION: " << maxVerticalPenetrationDepth << std::endl;
              std::cout << " -------------- COLLISION DURING FALL HORIZONTAL PENETRATION: " << maxHorizontalPenetrationDepth << std::endl;
              std::cout << " ============= FINISH FALL ========" << std::endl;
              //BottomCollisionDuringFall();
              FinishFall();
            }
          }

        } // end check and process collisions

        if(!animationLoaded) {
                return false;
        }

        if(animationHasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
                return false;
        }

        if(chrono::system_clock::now() >= nextSpriteTime) {
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
                        //cout << "KEY LEFT PRESSED" << endl;
//                        LoadAnimationWithId(MainCharacterAnimation::RUN_TO_LEFT);
                        headedToRight = false;
                        LeftKeyPressed();
                }
                MoveTo(MainCharacterDirection::LEFT);
        } else if((prevPressedKeys & KeyboardKeyCode::KEY_LEFT) == KeyboardKeyCode::KEY_LEFT) {
                //cout << "KEY LEFT RELEASED" << endl;
                LeftKeyReleased();
        }

        if(!isJumping && ((pressedKeys & KeyboardKeyCode::KEY_UP) == KeyboardKeyCode::KEY_UP)) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_UP) != KeyboardKeyCode::KEY_UP))) {
                        //cout << "KEY UP PRESSED" << endl;
                        UpKeyPressed();
                }
        }

        if(!isJumping && !isHitting && ((pressedKeys & KeyboardKeyCode::KEY_SPACE) == KeyboardKeyCode::KEY_SPACE)) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_SPACE) != KeyboardKeyCode::KEY_SPACE))) {
                        //cout << "KEY SPACE PRESSED" << endl;
                        isHitting = true;
                        SpaceKeyPressed();
                }
        }

        if(!isJumping && ((pressedKeys & KeyboardKeyCode::KEY_DOWN) == KeyboardKeyCode::KEY_DOWN)) {
                // If is not KEY_LEFT repeated press then change character state
                if((!checkPreviousPressedKeys) || ((checkPreviousPressedKeys) && ((prevPressedKeys & KeyboardKeyCode::KEY_DOWN) != KeyboardKeyCode::KEY_DOWN))) {
                        //cout << "KEY UP PRESSED" << endl;
                        DownKeyPressed();
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

void MainCharacter::LoadAnimationWithId(uint16_t animationId) {
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
          if(ShouldBeginAnimationLoopAgain()) {
            spriteData = NextSpriteData();
          }
  }

  nextSpriteTime = (chrono::system_clock::now() + chrono::milliseconds(spriteData.duration));

  currentSprite.width = spriteData.width;
  currentSprite.height = spriteData.height;
  currentSprite.xOffset = spriteData.xOffset;
  currentSprite.yOffset = spriteData.yOffset;
  currentSprite.u1 = spriteData.u1;
  currentSprite.v1 = spriteData.v1;
  currentSprite.u2 = spriteData.u2;
  currentSprite.v2 = spriteData.v2;
  currentSprite.areas = spriteData.areas;

  // Adjusts objectposition according to the sprite offset
  PositionSetOffset(spriteData.xOffset, spriteData.yOffset);

  recalculateAreasDataIsNeeded = true; // Is necessary because the current sprite may have different areas
  boundingBox = { spriteData.lowerBoundX, spriteData.lowerBoundY, spriteData.upperBoundX, spriteData.upperBoundY };
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

bool MainCharacter::ShouldBeginAnimationLoopAgain()
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
  PositionSetX(hInitialJumpPosition + (hInitialJumpSpeed * tJump));
  float vOffset = (vInitialJumpSpeed * tJump - (0.5f)*gravity*tJump*tJump);
  /*if(vOffset <= 0.0f) {
    // Last position of trajectory
    PositionSetY(vInitialJumpPosition);
    // Finish jump
    FinishJump();
  } else*/ {
    // Update vertical jumpt position
    PositionSetY(vInitialJumpPosition + vOffset);
  }
}

void MainCharacter::FinishJump() {
  isJumping = false;
  isLeaningOnTheGround = true;
  hMomentum = 0;
  JumpLanding();
}

void MainCharacter::UpdateFall() {
  tFall+=0.2f;
  PositionSetX(hInitialFallPosition + (hInitialFallSpeed * tFall));
  float vOffset = -(0.5f)*gravity*tFall*tFall;
  /*if(vOffset <= 0.0f) {
    // Jump landing
    position.setY(vInitialJumpPosition);
    isJumping = false;
    isLeaningOnTheGround = true;
    hMomentum = 0;
    JumpLanding();
  } else {*/
    PositionSetY(vInitialFallPosition + vOffset);
  /*}*/
}

void MainCharacter::FinishFall() {
  isFalling = false;
  isLeaningOnTheGround = true;
  FallLanding();
}

void MainCharacter::MoveTo(MainCharacterDirection direction)
{
  if(!isJumping && !isHitting) {
    PositionAddX(direction == MainCharacterDirection::RIGHT ? 4.0f : -4.0f);
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
  vInitialJumpPosition = position.GetY();
  hInitialJumpPosition = position.GetX();
  cout << "Initial JUMP X: " << hInitialJumpPosition << " Initial JUMP Y: " << vInitialJumpPosition << endl;
  tJump = 0.0f;
  isJumping = true;
  isFalling = false;
  isLeaningOnTheGround = false;
}

void MainCharacter::Fall(float hSpeed)
{
  cout << " >>>>>>>>>>>>>>>>>>>>>> MainCharacter::Fall" << endl;
  vInitialFallSpeed = 0.0f;
  hInitialFallSpeed = hSpeed;
  vInitialFallPosition = position.GetIntY();
  hInitialFallPosition = position.GetIntX();
  cout << "Initial FALL X: " << hInitialFallPosition << " Initial FALL Y: " << vInitialFallPosition << endl;
  tFall = 0.0f;
  isJumping = false;
  isFalling = true;
  isLeaningOnTheGround = false;
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::DownKeyPressed()
{
        cout << "MainCharacter::DownKeyPressed()" << endl;
        BEGIN_TRANSITION_MAP                                  // - Current State -
        TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)          // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)                 // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (STATE_FALL_RUN_RIGHT)                  // STATE_Run_Right
        TRANSITION_MAP_ENTRY (STATE_FALL_RUN_LEFT)                  // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::TopCollisionDuringJump()
{
        cout << "MainCharacter::TopCollisionDuringJump()" << endl;
        BEGIN_TRANSITION_MAP                                  // - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                 // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
        TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_RIGHT)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_FALL_IDLE_LEFT)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (STATE_FALL_RUN_RIGHT)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (STATE_FALL_RUN_LEFT)    // STATE_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
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
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::FallLanding()
{
        cout << "MainCharacter::FallLanding()" << endl;
        BEGIN_TRANSITION_MAP                                  // - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)          // STATE_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                 // STATE_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)                  // STATE_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Idle_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Jump_Run_Left
        TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Fall_Idle_Right
        TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Fall_Idle_Left
        TRANSITION_MAP_ENTRY (STATE_IDLE_RIGHT)    // STATE_Fall_Run_Right
        TRANSITION_MAP_ENTRY (STATE_IDLE_LEFT)    // STATE_Fall_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Fall_Jump_Run_Left
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Right
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)    // STATE_Hit_Left
        END_TRANSITION_MAP(NULL)
}

void MainCharacter::STATE_Idle_Right()
{
        cout << "MainCharacter::STATE_Idle_Right" << endl;
        hMomentum = 0;
        vectorDirection.x = 0.0f;
        vectorDirection.y = 0.0f;
        LoadAnimationWithId(MainCharacterAnimation::STAND_BY_RIGHT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Idle_Left()
{
        cout << "MainCharacter::STATE_Idle_Left" << endl;
        hMomentum = 0;
        vectorDirection.x = 0.0f;
        vectorDirection.y = 0.0f;
        LoadAnimationWithId(MainCharacterAnimation::STAND_BY_LEFT);
        cout << "ProcessPressedKeys(false)" << endl;
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Run_Right()
{
        cout << "MainCharacter::STATE_Run_Right" << endl;
        vectorDirection.x = 1.0f;
        vectorDirection.y = 0.0f;
        LoadAnimationWithId(MainCharacterAnimation::RUN_TO_RIGHT);
        ProcessPressedKeys(false);
}

void MainCharacter::STATE_Run_Left()
{
        cout << "MainCharacter::STATE_Run_Left" << endl;
        vectorDirection.x = -1.0f;
        vectorDirection.y = 0.0f;
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

void MainCharacter::STATE_Fall_Idle_Right()
{
  cout << "MainCharacter::STATE_Fall_Idle_Right" << endl;
  Fall(0.0f);
  LoadAnimationWithId(MainCharacterAnimation::FALL_RIGHT);
  ProcessPressedKeys(false);
}

void MainCharacter::STATE_Fall_Idle_Left()
{
  cout << "MainCharacter::STATE_Fall_Idle_Left" << endl;
  Fall(0.0f);
  LoadAnimationWithId(MainCharacterAnimation::FALL_LEFT);
  ProcessPressedKeys(false);
}

void MainCharacter::STATE_Fall_Run_Right()
{
  cout << "MainCharacter::STATE_Fall_Run_Right" << endl;
  Fall(hMomentum == maxMomentum ? 10.0f : 4.0f);
  LoadAnimationWithId(MainCharacterAnimation::FALL_RIGHT);
  ProcessPressedKeys(false);
}

void MainCharacter::STATE_Fall_Run_Left()
{
  cout << "MainCharacter::STATE_Fall_Run_Left" << endl;
  Fall(hMomentum == maxMomentum ? -10.0f : -4.0f);
  LoadAnimationWithId(MainCharacterAnimation::FALL_LEFT);
  ProcessPressedKeys(false);
}

void MainCharacter::STATE_Fall_Jump_Run_Right()
{
  cout << "MainCharacter::STATE_Fall_Jump_Run_Right" << endl;
  LoadAnimationWithId(MainCharacterAnimation::FALL_RIGHT);
  ProcessPressedKeys(false);
}

void MainCharacter::STATE_Fall_Jump_Run_Left()
{
  cout << "MainCharacter::STATE_Fall_Jump_Run_Left" << endl;
  LoadAnimationWithId(MainCharacterAnimation::FALL_LEFT);
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
