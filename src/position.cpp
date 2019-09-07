#include "position.h"

Position::Position() {
  x = y = 0.0f;
  int_x = int_y = 0;
  int_x_offset = int_y_offset = 0;
}

float Position::GetX() {
  return x + static_cast<float>(int_x_offset);
}

float Position::GetY() {
  return y + static_cast<float>(int_y_offset);
}

uint16_t Position::GetIntX() {
  return int_x + int_x_offset;
}

uint16_t Position::GetIntY() {
  return int_y + int_y_offset;
}

void Position::setOffset(int16_t _x, int16_t _y) {
  int_x_offset = _x;
  int_y_offset = _y;
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
