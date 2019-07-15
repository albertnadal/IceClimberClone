#include "brick.h"
#include <chrono>

Brick::Brick() :
        ISceneObject(SceneObjectIdentificator::BRICK, BrickStateIdentificator::BRICK_MAX_STATES) {
        cout << " INSTANCIA NOVA DE Brick CREADA" << endl;
        position.setX(300.0f);
        position.setY(100.0f);
}

uint16 Brick::Width() {
        return currentSprite.width;
}

uint16 Brick::Height() {
        return currentSprite.height;
}

void Brick::PrintName() {
        std::cout << "Brick." << std::endl;
}

bool Brick::Update(uchar pressedKeys_) {

        bool needRedraw = false;

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

void Brick::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(BrickAnimation::STICKY);
}

void Brick::LoadAnimationWithId(uint16 animationId) {
        ObjectSpriteSheetAnimation *currentAnimation = spriteSheet->GetAnimationWithId(animationId);
        currentAnimationSprites = currentAnimation->GetSprites();
        animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
        animationLoaded = true;
        firstSpriteOfCurrentAnimationIsLoaded = false;
        nextSpriteTime = std::chrono::system_clock::now();
}

void Brick::LoadNextSprite()
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

SpriteData Brick::NextSpriteData()
{
        if(currentAnimationSpriteIterator == std::end(currentAnimationSprites)) {
                currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
                (*currentAnimationSpriteIterator).beginNewLoop = true;
        }

        return *currentAnimationSpriteIterator++;
}

ISceneObject* Brick::Create() {
        return new Brick();
}

Brick::~Brick() {

}

bool Brick::BeginAnimationLoopAgain()
{
/*
        if(currentState == BrickStateIdentificator::STATE_STICKY) {
          ExternalEvent(BrickStateIdentificator::STATE_FALLING, NULL);
          return true;
        } else {
          return false;
        }
*/
        return false;
}

void Brick::ReceiveHammerImpact()
{
        cout << "Brick::ReceiveHammerImpact()" << endl;
        BEGIN_TRANSITION_MAP                    // - Current State -
        TRANSITION_MAP_ENTRY (STATE_FALLING) // STATE_Sticky
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)  // STATE_Falling
        END_TRANSITION_MAP(NULL)
}

void Brick::STATE_Sticky()
{
        cout << "Brick::STATE_Sticky" << endl;
        LoadAnimationWithId(BrickAnimation::STICKY);
}

void Brick::STATE_Falling()
{
        cout << "Brick::STATE_Falling" << endl;
        LoadAnimationWithId(BrickAnimation::FALLING);
}
