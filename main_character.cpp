#include "main_character.h"
#include <chrono>

MainCharacter::MainCharacter() {
        id_ = SceneObjectIdentificator::MAIN_CHARACTER;
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

        if(pressedKeys != KEY_NONE) {
                // character in action
                ProcessPressedKeys(pressedKeys);
        } else if(pressedKeys != prevPressedKeys) {
                // character goes quiet headed in the proper direction
                LoadAnimationWithId(headedToRight ? 0 : 1);
                prevPressedKeys = KEY_NONE;
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
        if((pressedKeys & KEY_RIGHT) == KEY_RIGHT) {
                // If is not KEY_RIGHT repeated press then change character state
                if((prevPressedKeys & KEY_RIGHT) != KEY_RIGHT) {
                        LoadAnimationWithId(2);
                        headedToRight = true;
                }
                position.x+=3;
        }

        if((pressedKeys & KEY_LEFT) == KEY_LEFT) {
                // If is not KEY_LEFT repeated press then change character state
                if((prevPressedKeys & KEY_LEFT) != KEY_LEFT) {
                        LoadAnimationWithId(3);
                        headedToRight = false;
                }
                position.x-=3;
        }

        prevPressedKeys = pressedKeys;
}

void MainCharacter::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(0); // stand-by headed-to-right
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
