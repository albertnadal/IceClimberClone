#ifndef POSITION_H
#define POSITION_H

#include <defines.h>

class Position
{
private:
  float initial_x, initial_y, x, y;
  int initial_int_x, initial_int_y, int_x, int_y, int_x_offset, int_y_offset;

public:
  Position();
  ~Position();
  float GetX();
  float GetRealX();
  float GetY();
  float GetRealY();
  std::pair<float, float> GetCoordinate();
  void ApplyDelta(float x, float y);
  int GetIntX();
  int GetIntY();
  int GetCellX();
  int GetCellY();
  void setInitialXY(float x, float y);
  void setOffset(int x, int y);
  void setXY(float, float);
  void setX(float);
  void setY(float);
  void addX(int);
  void addY(int);
  void addX(float);
  void addY(float);
  void recoverInitialPosition();
};

#endif
