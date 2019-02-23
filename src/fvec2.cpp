#include "fvec2.h"
#include <cmath>

fvec2::fvec2()
        : x(0), y(0)
{
}

fvec2::fvec2(float scalar)
        : x(scalar), y(scalar)
{
}

fvec2::fvec2(float x, float y)
        : x(x), y(y)
{
}

fvec2& fvec2::Add(const fvec2& other)
{
        x += other.x;
        y += other.y;

        return *this;
}

fvec2& fvec2::Subtract(const fvec2& other)
{
        x -= other.x;
        y -= other.y;

        return *this;
}

fvec2& fvec2::Multiply(const fvec2& other)
{
        x *= other.x;
        y *= other.y;

        return *this;
}

fvec2& fvec2::Divide(const fvec2& other)
{
        x /= other.x;
        y /= other.y;

        return *this;
}

fvec2& fvec2::Add(float value)
{
        x += value;
        y += value;

        return *this;
}

fvec2& fvec2::Subtract(float value)
{
        x -= value;
        y -= value;

        return *this;
}

fvec2& fvec2::Multiply(float value)
{
        x *= value;
        y *= value;

        return *this;
}

fvec2& fvec2::Divide(float value)
{
        x /= value;
        y /= value;

        return *this;
}

fvec2 operator+(fvec2 left, const fvec2& right)
{
        return left.Add(right);
}

fvec2 operator-(fvec2 left, const fvec2& right)
{
        return left.Subtract(right);
}

fvec2 operator*(fvec2 left, const fvec2& right)
{
        return left.Multiply(right);
}

fvec2 operator/(fvec2 left, const fvec2& right)
{
        return left.Divide(right);
}

fvec2 operator+(fvec2 left, float value)
{
        return fvec2(left.x + value, left.y + value);
}

fvec2 operator-(fvec2 left, float value)
{
        return fvec2(left.x - value, left.y - value);
}

fvec2 operator*(fvec2 left, float value)
{
        return fvec2(left.x * value, left.y * value);
}

fvec2 operator/(fvec2 left, float value)
{
        return fvec2(left.x / value, left.y / value);
}

fvec2& fvec2::operator+=(const fvec2& other)
{
        return Add(other);
}

fvec2& fvec2::operator-=(const fvec2& other)
{
        return Subtract(other);
}

fvec2& fvec2::operator*=(const fvec2& other)
{
        return Multiply(other);
}

fvec2& fvec2::operator/=(const fvec2& other)
{
        return Divide(other);
}

fvec2& fvec2::operator+=(float value)
{
        return Add(value);
}

fvec2& fvec2::operator-=(float value)
{
        return Subtract(value);
}

fvec2& fvec2::operator*=(float value)
{
        return Multiply(value);
}

fvec2& fvec2::operator/=(float value)
{
        return Divide(value);
}

bool fvec2::operator==(const fvec2& other) const
{
        return x == other.x && y == other.y;
}

bool fvec2::operator!=(const fvec2& other) const
{
        return !(*this == other);
}

bool fvec2::operator<(const fvec2& other) const
{
        return x < other.x && y < other.y;
}

bool fvec2::operator<=(const fvec2& other) const
{
        return x <= other.x && y <= other.y;
}

bool fvec2::operator>(const fvec2& other) const
{
        return x > other.x && y > other.y;
}

bool fvec2::operator>=(const fvec2& other) const
{
        return x >= other.x && y >= other.y;
}

float fvec2::Distance(const fvec2& other) const
{
        float a = x - other.x;
        float b = y - other.y;
        return sqrt(a * a + b * b);
}

float fvec2::Dot(const fvec2& other) const
{
        return x * other.x + y * other.y;
}

float fvec2::Magnitude() const
{
        return sqrt(x * x + y * y);
}

fvec2 fvec2::Normalise() const
{
        float length = Magnitude();
        return fvec2(x / length, y / length);
}
