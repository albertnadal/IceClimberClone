#ifndef POSITION_H
#define POSITION_H

#include <defines.h>

class Position
{
private:
  float prev_x, prev_y, x, y;
  int16_t prev_int_x, prev_int_y, int_x, int_y;
  int16_t prev_int_x_offset, prev_int_y_offset, int_x_offset, int_y_offset;
public:
  Position();
  ~Position();

  float GetX();
  float GetPrevX();
  float GetY();
  float GetPrevY();
  int16_t GetIntX();
  int16_t GetPrevIntX();
  int16_t GetIntY();
  int16_t GetPrevIntY();
  void setOffset(int16_t x, int16_t y);
  void setX(int16_t);
  void setY(int16_t);
  void setX(float);
  void setY(float);
  void addX(int16_t);
  void addY(int16_t);
  void addX(float);
  void addY(float);
  void recoverPreviousPosition();
};

#endif
