#ifndef POSITION_H
#define POSITION_H

#include <defines.h>

class Position
{
private:
  float x, y;
  uint16_t int_x, int_y;
  int16_t int_x_offset, int_y_offset;
public:
  Position();
  ~Position();

  float GetX();
  float GetY();
  uint16_t GetIntX();
  uint16_t GetIntY();
  void setOffset(int16_t x, int16_t y);
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
