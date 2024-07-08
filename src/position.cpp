#include "position.h"
#include <cmath>
#include <limits>
#include <iostream>

Position::Position() {
    prev_x = prev_y = x = y = 0.0f;
    prev_int_x = prev_int_y = int_x = int_y = 0;
    prev_int_x_offset = prev_int_y_offset = int_x_offset = int_y_offset = 0;
}

float Position::GetX() {
    return x + static_cast<float>(int_x_offset);
}

float Position::GetRealX() {
    return x;
}

float Position::GetY() {
    return y + static_cast<float>(int_y_offset);
}

float Position::GetPrevX() {
    return prev_x;
}

float Position::GetPrevY() {
    return prev_y;
}

float Position::GetRealY() {
    return y;
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

void Position::setXY(float _x, float _y) {
    prev_y = y;
    prev_x = x;
    prev_int_x = int_x;
    int_x = static_cast<int16_t>(_x);
    x = _x;
    prev_int_y = int_y;
    int_y = static_cast<int16_t>(_y);
    y = _y;
}

void Position::setX(float _x) {
    prev_y = y;
    prev_int_x = int_x;
    int_x = static_cast<int16_t>(_x);
    prev_x = x;
    x = _x;
}

void Position::setY(float _y) {
    prev_x = x;
    prev_int_y = int_y;
    int_y = static_cast<int16_t>(_y);
    prev_y = y;
    y = _y;
}

void Position::addX(float _x) {
    prev_y = y;
    prev_x = x;
    x += _x;
    std::cout << "addX: " << x << "\n";
    prev_int_x = int_x;
    int_x = static_cast<int16_t>(x);
}

void Position::addY(float _y) {
    prev_x = x;
    prev_y = y;
    y += _y;
    std::cout << "addY: " << y << "\n";
    prev_int_y = int_y;
    int_y = static_cast<int16_t>(y);
}

void Position::recoverPreviousPosition() {
    x = prev_x;
    y = prev_y;
    int_x = prev_int_x;
    int_y = prev_int_y;
    int_x_offset = prev_int_x_offset;
    int_y_offset = prev_int_y_offset;
}

Position::~Position() {

}
