#ifndef POSITION_H
#define POSITION_H

#include <types.h>

class Position
{
public:
  float x, y;
  uint16 int_x, int_y;
  Position();
  ~Position();
  void setX(uint16);
  void setY(uint16);
  void setX(float);
  void setY(float);
  void addX(int16);
  void addY(int16);
  void addX(float);
  void addY(float);
};

#endif
