#include "main_character.h"

MainCharacter::MainCharacter() {
  id_ = SceneObjectIdentificator::MAIN_CHARACTER;
  this->position.x = 100.0;
  this->position.y = 100.0;
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

void MainCharacter::Update() {
  this->currentAnimation = this->spriteSheet->GetAnimationWithId(1);
  SpriteData spriteData = this->currentAnimation->sprites.front();
  this->currentSprite.width = spriteData.width;
  this->currentSprite.height = spriteData.height;
  this->currentSprite.u1 = spriteData.u1;
  this->currentSprite.v1 = spriteData.v1;
  this->currentSprite.u2 = spriteData.u2;
  this->currentSprite.v2 = spriteData.v2;
}

void MainCharacter::InitWithSpriteSheet(ObjectSpriteSheet *_spriteSheet) {
  cout << "InitWithSpriteSheet PRINT spriteSheet: " << endl;
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
