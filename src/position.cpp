#include "position.h"

Position::Position() {
  x = y = 0.0f;
  int_x = int_x = 0;
}

void Position::setX(uint16 _x) {
  int_x = _x;
  x = static_cast<float>(_x);
}

void Position::setY(uint16 _y) {
  int_y = _y;
  y = static_cast<float>(_y);
}

void Position::setX(float _x) {
  int_x = static_cast<uint16>(_x);
  x = _x;
}

void Position::setY(float _y) {
  int_y = static_cast<uint16>(_y);
  y = _y;
}

void Position::addX(int16 _x) {
  int_x += _x;
  x += static_cast<float>(int_x);
}

void Position::addY(int16 _y) {
  int_y += _y;
  y += static_cast<float>(int_y);
}

void Position::addX(float _x) {
  x += _x;
  int_x = static_cast<uint16>(x);
}

void Position::addY(float _y) {
  y += _y;
  int_y = static_cast<uint16>(y);
}

Position::~Position() {

}