#ifndef POSITION_H
#define POSITION_H

#include <defines.h>

class Position
{
private:
  float prev_x, prev_y, x, y;
  int prev_int_x, prev_int_y, int_x, int_y;
  int prev_int_x_offset, prev_int_y_offset, int_x_offset, int_y_offset;
public:
  Position();
  ~Position();

  float GetX();
  float GetRealX();
  float GetY();
  float GetRealY();
  float GetPrevX();
  float GetPrevY();
  int GetIntX();
  int GetIntY();
  void setOffset(int x, int y);
  void setXY(float, float);
  void setX(float);
  void setY(float);
  void addX(int);
  void addY(int);
  void addX(float);
  void addY(float);
  void recoverPreviousPosition();
};

#endif
