#include "position.h"

Position::Position() {
  prev_x = prev_y = x = y = 0.0f;
  prev_int_x = prev_int_y = int_x = int_y = 0;
  prev_int_x_offset = prev_int_y_offset = int_x_offset = int_y_offset = 0;
}

float Position::GetX() {
  return x + static_cast<float>(int_x_offset);
}

float Position::GetY() {
  return y + static_cast<float>(int_y_offset);
}

int16_t Position::GetIntX() {
  return int_x + int_x_offset;
}

int16_t Position::GetIntY() {
  return int_y + int_y_offset;
}

void Position::setOffset(int16_t _x, int16_t _y) {
  prev_int_x_offset = int_x_offset;
  prev_int_y_offset = int_y_offset;
  int_x_offset = _x;
  int_y_offset = _y;
}

void Position::setX(int16_t _x) {
  prev_int_x = int_x;
  int_x = _x;
  prev_x = x;
  x = static_cast<float>(_x);
}

void Position::setY(int16_t _y) {
  prev_int_y = int_y;
  int_y = _y;
  prev_y = y;
  y = static_cast<float>(_y);
}

void Position::setX(float _x) {
  prev_int_x = int_x;
  int_x = static_cast<int16_t>(_x);
  prev_x = x;
  x = _x;
}

void Position::setY(float _y) {
  prev_int_y = int_y;
  int_y = static_cast<int16_t>(_y);
  prev_y = y;
  y = _y;
}

void Position::addX(int16_t _x) {
  prev_int_x = int_x;
  int_x += _x;
  prev_x = x;
  x = static_cast<float>(int_x);
}

void Position::addY(int16_t _y) {
  prev_int_y = int_y;
  int_y += _y;
  prev_y = y;
  y = static_cast<float>(int_y);
}

void Position::addX(float _x) {
  prev_x = x;
  x += _x;
  prev_int_x = int_x;
  int_x = static_cast<int16_t>(x);
}

void Position::addY(float _y) {
  prev_y = y;
  y += _y;
  prev_int_y = int_y;
  int_y = static_cast<int16_t>(y);
}

Position::~Position() {

}
