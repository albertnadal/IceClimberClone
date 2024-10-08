#include <cmath>
#include <limits>
#include <iostream>
#include <position.h>

Position::Position() {
    initial_x = initial_y = x = y = 0.0f;
    initial_int_x = initial_int_y = int_x = int_y = 0;
    int_x_offset = int_y_offset = 0;
}

float Position::GetX() const {
    return x + static_cast<float>(int_x_offset);
}

float Position::GetRealX() const {
    return x;
}

float Position::GetY() const {
    return y + static_cast<float>(int_y_offset);
}

float Position::GetRealY() const {
    return y;
}

std::pair<float, float> Position::GetCoordinate() const {
    return std::make_pair(x, y);
}

int Position::GetIntX() const {
    return int_x + int_x_offset;
}

int Position::GetIntY() const {
    return int_y + int_y_offset;
}

int Position::GetCellX() const {
    return (int_x + (CELL_WIDTH >> 1)) / CELL_WIDTH;
}

int Position::GetInitialCellX() const {
    return (initial_int_x + (CELL_WIDTH >> 1)) / CELL_WIDTH;
}

int Position::GetCellY() const {
    return (int_y + (CELL_HEIGHT >> 1)) / CELL_HEIGHT;
}

int Position::GetInitialCellY() const {
    return (initial_int_y + (CELL_HEIGHT >> 1)) / CELL_HEIGHT;
}

void Position::setOffset(int _x, int _y) {
    int_x_offset = _x;
    int_y_offset = _y;
}

void Position::setInitialXY(float _x, float _y) {
    initial_x = _x;
    initial_y = _y;
    initial_int_x = static_cast<int>(_x);
    initial_int_y = static_cast<int>(_y);
    setXY(_x, _y);
}

void Position::setXY(float _x, float _y) {
    int_x = static_cast<int>(_x);
    x = _x;
    int_y = static_cast<int>(_y);
    y = _y;
}

void Position::ApplyDelta(float _x, float _y) {
    x += _x;
    int_x = static_cast<int>(x);
    y += _y;
    int_y = static_cast<int>(y);
}

void Position::setX(float _x) {
    int_x = static_cast<int>(_x);
    x = _x;
}

void Position::setY(float _y) {
    int_y = static_cast<int>(_y);
    y = _y;
}

void Position::addX(float _x) {
    x += _x;
    int_x = static_cast<int>(x);
}

void Position::addY(float _y) {
    y += _y;
    int_y = static_cast<int>(y);
}

void Position::recoverInitialPosition() {
    x = initial_x;
    y = initial_y;
    int_x = initial_int_x;
    int_y = initial_int_y;
}

Position::~Position() {
}
