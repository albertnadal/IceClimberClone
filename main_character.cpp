#include "main_character.h"
#include <chrono>

MainCharacter::MainCharacter() {
        id_ = SceneObjectIdentificator::MAIN_CHARACTER;
        this->position.x = 100;
        this->position.y = 100;
        prevPressedKeys = KEY_NONE;
}

uint16 MainCharacter::GetWidth() {
        return this->currentSprite.width;
}

uint16 MainCharacter::GetHeight() {
        return this->currentSprite.height;
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

        if(std::chrono::system_clock::now() >= this->nextSpriteTime) {
                // Load next sprite of the current animation
                SpriteData spriteData = NextSpriteData();
                this->nextSpriteTime = (std::chrono::system_clock::now() + std::chrono::milliseconds(spriteData.duration));

                //std::cout << "u1: " << spriteData.u1 << " v1: " << spriteData.v1 << " u2: " << spriteData.u2 << " v2: " << spriteData.v2 << std::endl;
                this->currentSprite.width = spriteData.width;
                this->currentSprite.height = spriteData.height;
                this->currentSprite.u1 = spriteData.u1;
                this->currentSprite.v1 = spriteData.v1;
                this->currentSprite.u2 = spriteData.u2;
                this->currentSprite.v2 = spriteData.v2;

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
                this->position.x+=3;
        }

        if((pressedKeys & KEY_LEFT) == KEY_LEFT) {
                // If is not KEY_LEFT repeated press then change character state
                if((prevPressedKeys & KEY_LEFT) != KEY_LEFT) {
                        LoadAnimationWithId(3);
                        headedToRight = false;
                }
                this->position.x-=3;
        }

        prevPressedKeys = pressedKeys;
}

void MainCharacter::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
        spriteSheet = _spriteSheet;
        LoadAnimationWithId(0); // stand-by headed-to-right
}

void MainCharacter::LoadAnimationWithId(uint16 animationId) {
        ObjectSpriteSheetAnimation *currentAnimation = this->spriteSheet->GetAnimationWithId(animationId);
        currentAnimationSprites = currentAnimation->GetSprites();
        animationHasOnlyOneSprite = (currentAnimationSprites.size() <= 1);
        this->MoveToFirstSprite();
        animationLoaded = true;
        firstSpriteOfCurrentAnimationIsLoaded = false;
}

void MainCharacter::MoveToFirstSprite()
{
        currentAnimationSpriteIterator = std::begin(currentAnimationSprites);
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
