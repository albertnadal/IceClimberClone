#include "main_character.h"

MainCharacter::MainCharacter() {
  id_ = SceneObjectIdentificator::MAIN_CHARACTER;
  this->position.x = 100.0;
  this->position.y = 100.0;
}

uint16 MainCharacter::getWidth() {
  return this->currentSprite.width;
}

uint16 MainCharacter::getHeight() {
  return this->currentSprite.height;
}

void MainCharacter::PrintName(){
  std::cout << "Main character." << std::endl;
}

void MainCharacter::Update() {

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
