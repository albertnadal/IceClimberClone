#ifndef POSITION_H
#define POSITION_H

#include <defines.h>

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
  void addX(uint16);
  void addY(uint16);
  void addX(float);
  void addY(float);
};

#endif
