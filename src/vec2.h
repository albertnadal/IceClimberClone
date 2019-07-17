#pragma once

#include <defines.h>

struct vec2
{
        uint16 x, y;

        vec2();
        vec2(uint16 scalar);
        vec2(uint16 x, uint16 y);

        vec2& Add(const vec2& other);
        vec2& Subtract(const vec2& other);
        vec2& Multiply(const vec2& other);
        vec2& Divide(const vec2& other);

        vec2& Add(uint16 value);
        vec2& Subtract(uint16 value);
        vec2& Multiply(uint16 value);
        vec2& Divide(uint16 value);

        friend vec2 operator+(vec2 left, const vec2 &right);
        friend vec2 operator-(vec2 left, const vec2 &right);
        friend vec2 operator*(vec2 left, const vec2 &right);
        friend vec2 operator/(vec2 left, const vec2 &right);

        friend vec2 operator+(vec2 left, uint16 value);
        friend vec2 operator-(vec2 left, uint16 value);
        friend vec2 operator*(vec2 left, uint16 value);
        friend vec2 operator/(vec2 left, uint16 value);

        bool operator==(const vec2 &other) const;
        bool operator!=(const vec2 &other) const;

        vec2& operator+=(const vec2 &other);
        vec2& operator-=(const vec2 &other);
        vec2& operator*=(const vec2 &other);
        vec2& operator/=(const vec2 &other);

        vec2& operator+=(uint16 value);
        vec2& operator-=(uint16 value);
        vec2& operator*=(uint16 value);
        vec2& operator/=(uint16 value);

        bool operator<(const vec2 &other) const;
        bool operator<=(const vec2 &other) const;
        bool operator>(const vec2 &other) const;
        bool operator>=(const vec2 &other) const;

        float Magnitude() const;
        vec2 Normalise() const;
        float Distance(const vec2& other) const;
        uint16 Dot(const vec2& other) const;
};
