#include "position.h"

Position::Position() {
  x = y = 0.0f;
  int_x = int_x = 0;
}

void Position::setX(uint16_t _x) {
  int_x = _x;
  x = static_cast<float>(_x);
}

void Position::setY(uint16_t _y) {
  int_y = _y;
  y = static_cast<float>(_y);
}

void Position::setX(float _x) {
  int_x = static_cast<uint16_t>(_x);
  x = _x;
}

void Position::setY(float _y) {
  int_y = static_cast<uint16_t>(_y);
  y = _y;
}

void Position::addX(uint16_t _x) {
  int_x += _x;
  x += static_cast<float>(int_x);
}

void Position::addY(uint16_t _y) {
  int_y += _y;
  y += static_cast<float>(int_y);
}

void Position::addX(float _x) {
  x += _x;
  int_x = static_cast<uint16_t>(x);
}

void Position::addY(float _y) {
  y += _y;
  int_y = static_cast<uint16_t>(y);
}

Position::~Position() {

}
