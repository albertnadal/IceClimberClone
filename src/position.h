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
  float GetX() const;
  float GetRealX() const;
  float GetY() const;
  float GetRealY() const;
  std::pair<float, float> GetCoordinate() const;
  void ApplyDelta(float x, float y);
  int GetIntX() const;
  int GetIntY() const;
  int GetCellX() const;
  int GetInitialCellX() const;
  int GetCellY() const;
  int GetInitialCellY() const;
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
