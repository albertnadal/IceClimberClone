#include "main_character.h"
#include <chrono>

MainCharacter::MainCharacter() {
  id_ = SceneObjectIdentificator::MAIN_CHARACTER;
  this->position.x = (rand() % 1350);
  this->position.y = (rand() % 700);
  this->currentAnimation = nullptr;
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

bool MainCharacter::Update() {
  //this->position.x = 100; //(rand() % 1350);
  //this->position.y = 100; //(rand() % 700);

  if(this->currentAnimation == nullptr) {
    return false;
  }

  if(this->currentAnimation->hasOnlyOneSprite && firstSpriteOfCurrentAnimationIsLoaded) {
    return false;
  }

  if(std::chrono::system_clock::now() >= this->nextSpriteTime) {
    // Load next sprite of the current animation
    SpriteData spriteData = this->currentAnimation->NextSpriteData();
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

void MainCharacter::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
  spriteSheet = _spriteSheet;
  this->currentAnimation = this->spriteSheet->GetAnimationWithId(1);
  this->currentAnimation->MoveToFirstSprite();
  firstSpriteOfCurrentAnimationIsLoaded = false;
}

ISceneObject* MainCharacter::Create() {
  return new MainCharacter();
}

MainCharacter::~MainCharacter() {

}
