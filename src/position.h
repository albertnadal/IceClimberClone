#ifndef POSITION_H
#define POSITION_H

#include <defines.h>

class Position
{
public:
  float x, y;
  uint16_t int_x, int_y;
  Position();
  ~Position();
  void setX(uint16_t);
  void setY(uint16_t);
  void setX(float);
  void setY(float);
  void addX(uint16_t);
  void addY(uint16_t);
  void addX(float);
  void addY(float);
};

#endif
