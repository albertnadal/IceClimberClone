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
  float GetRealX();
  float GetY();
  float GetRealY();
  float GetPrevX();
  float GetPrevY();
  int16_t GetIntX();
  int16_t GetIntY();
  void setOffset(int16_t x, int16_t y);
  void setXY(float, float);
  void setX(float);
  void setY(float);
  void addX(int16_t);
  void addY(int16_t);
  void addX(float);
  void addY(float);
  void recoverPreviousPosition();
  float getTrajectoryTangent();
};

#endif
