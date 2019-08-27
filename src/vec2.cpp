#include "vec2.h"
#include <cmath>

vec2::vec2()
        : x(0), y(0)
{
}

vec2::vec2(uint16_t scalar)
        : x(scalar), y(scalar)
{
}

vec2::vec2(uint16_t x, uint16_t y)
        : x(x), y(y)
{
}

vec2& vec2::Add(const vec2& other)
{
        x += other.x;
        y += other.y;

        return *this;
}

vec2& vec2::Subtract(const vec2& other)
{
        x -= other.x;
        y -= other.y;

        return *this;
}

vec2& vec2::Multiply(const vec2& other)
{
        x *= other.x;
        y *= other.y;

        return *this;
}

vec2& vec2::Divide(const vec2& other)
{
        x /= other.x;
        y /= other.y;

        return *this;
}

vec2& vec2::Add(uint16_t value)
{
        x += value;
        y += value;

        return *this;
}

vec2& vec2::Subtract(uint16_t value)
{
        x -= value;
        y -= value;

        return *this;
}

vec2& vec2::Multiply(uint16_t value)
{
        x *= value;
        y *= value;

        return *this;
}

vec2& vec2::Divide(uint16_t value)
{
        x /= value;
        y /= value;

        return *this;
}

vec2 operator+(vec2 left, const vec2& right)
{
        return left.Add(right);
}

vec2 operator-(vec2 left, const vec2& right)
{
        return left.Subtract(right);
}

vec2 operator*(vec2 left, const vec2& right)
{
        return left.Multiply(right);
}

vec2 operator/(vec2 left, const vec2& right)
{
        return left.Divide(right);
}

vec2 operator+(vec2 left, uint16_t value)
{
        return vec2(left.x + value, left.y + value);
}

vec2 operator-(vec2 left, uint16_t value)
{
        return vec2(left.x - value, left.y - value);
}

vec2 operator*(vec2 left, uint16_t value)
{
        return vec2(left.x * value, left.y * value);
}

vec2 operator/(vec2 left, uint16_t value)
{
        return vec2(left.x / value, left.y / value);
}

vec2& vec2::operator+=(const vec2& other)
{
        return Add(other);
}

vec2& vec2::operator-=(const vec2& other)
{
        return Subtract(other);
}

vec2& vec2::operator*=(const vec2& other)
{
        return Multiply(other);
}

vec2& vec2::operator/=(const vec2& other)
{
        return Divide(other);
}

vec2& vec2::operator+=(uint16_t value)
{
        return Add(value);
}

vec2& vec2::operator-=(uint16_t value)
{
        return Subtract(value);
}

vec2& vec2::operator*=(uint16_t value)
{
        return Multiply(value);
}

vec2& vec2::operator/=(uint16_t value)
{
        return Divide(value);
}

bool vec2::operator==(const vec2& other) const
{
        return x == other.x && y == other.y;
}

bool vec2::operator!=(const vec2& other) const
{
        return !(*this == other);
}

bool vec2::operator<(const vec2& other) const
{
        return x < other.x && y < other.y;
}

bool vec2::operator<=(const vec2& other) const
{
        return x <= other.x && y <= other.y;
}

bool vec2::operator>(const vec2& other) const
{
        return x > other.x && y > other.y;
}

bool vec2::operator>=(const vec2& other) const
{
        return x >= other.x && y >= other.y;
}

float vec2::Distance(const vec2& other) const
{
        float a = x - other.x;
        float b = y - other.y;
        return sqrt(a * a + b * b);
}

uint16_t vec2::Dot(const vec2& other) const
{
        return x * other.x + y * other.y;
}

float vec2::Magnitude() const
{
        return sqrt(x * x + y * y);
}

vec2 vec2::Normalise() const
{
        float length = Magnitude();
        return vec2(x / length, y / length);
}
