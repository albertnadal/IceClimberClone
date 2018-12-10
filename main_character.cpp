#include "main_character.h"

MainCharacter::MainCharacter() {
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

ISceneObject* MainCharacter::Create() {
  return new MainCharacter();
}

MainCharacter::~MainCharacter() {

}
