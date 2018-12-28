#include "main_character.h"

MainCharacter::MainCharacter() {
  id_ = SceneObjectIdentificator::MAIN_CHARACTER;
  this->position.x = 100;
  this->position.y = 100;
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
  this->position.x = 100; //(rand() % 1350);
  this->position.y = 100; //(rand() % 700);

  if(this->currentAnimation == nullptr) {
    this->currentAnimation = this->spriteSheet->GetAnimationWithId(1);
    this->currentAnimation->MoveToFirstSprite();
  }

  SpriteData spriteData = this->currentAnimation->GetNextSpriteData();
  //std::cout << "u1: " << spriteData.u1 << " v1: " << spriteData.v1 << " u2: " << spriteData.u2 << " v2: " << spriteData.v2 << std::endl;
  this->currentSprite.width = spriteData.width;
  this->currentSprite.height = spriteData.height;
  this->currentSprite.u1 = spriteData.u1;
  this->currentSprite.v1 = spriteData.v1;
  this->currentSprite.u2 = spriteData.u2;
  this->currentSprite.v2 = spriteData.v2;

  return ((rand() % 1) == 0);
}

void MainCharacter::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
  spriteSheet = _spriteSheet;
  if(spriteSheet != nullptr) {
    spriteSheet->Print();
    //TODO: Update the currentSprite according to the current object status
  }
}

ISceneObject* MainCharacter::Create() {
  return new MainCharacter();
}

MainCharacter::~MainCharacter() {

}
