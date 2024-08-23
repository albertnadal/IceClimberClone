#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <iostream>
#include <entity.h>
#include <entity_manager.h>
#include <state_machine.h>
#include <sprite.h>
#include <defines.h>

using namespace std;


class GameOver: public IEntity
{
  // GameOver action states
  bool isMoving = true;            // GameOver tile is moving up

public:
  GameOver();
  ~GameOver() override;
  void InitWithSpriteSheet(EntitySpriteSheet*) override;
  void PrintName() override;
  bool Update(uint8_t) override;
  static IEntity* Create();

private:

  // state map to define state function order
  BEGIN_STATE_MAP
  END_STATE_MAP

  // state enumeration order must match the order of state
  // method entries in the state map
  enum GameOverStateIdentificator {
      GAME_OVER_MAX_STATES
  };
};

#endif
