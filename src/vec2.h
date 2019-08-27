#pragma once

#include <defines.h>

struct vec2
{
        uint16_t x, y;

        vec2();
        vec2(uint16_t scalar);
        vec2(uint16_t x, uint16_t y);

        vec2& Add(const vec2& other);
        vec2& Subtract(const vec2& other);
        vec2& Multiply(const vec2& other);
        vec2& Divide(const vec2& other);

        vec2& Add(uint16_t value);
        vec2& Subtract(uint16_t value);
        vec2& Multiply(uint16_t value);
        vec2& Divide(uint16_t value);

        friend vec2 operator+(vec2 left, const vec2 &right);
        friend vec2 operator-(vec2 left, const vec2 &right);
        friend vec2 operator*(vec2 left, const vec2 &right);
        friend vec2 operator/(vec2 left, const vec2 &right);

        friend vec2 operator+(vec2 left, uint16_t value);
        friend vec2 operator-(vec2 left, uint16_t value);
        friend vec2 operator*(vec2 left, uint16_t value);
        friend vec2 operator/(vec2 left, uint16_t value);

        bool operator==(const vec2 &other) const;
        bool operator!=(const vec2 &other) const;

        vec2& operator+=(const vec2 &other);
        vec2& operator-=(const vec2 &other);
        vec2& operator*=(const vec2 &other);
        vec2& operator/=(const vec2 &other);

        vec2& operator+=(uint16_t value);
        vec2& operator-=(uint16_t value);
        vec2& operator*=(uint16_t value);
        vec2& operator/=(uint16_t value);

        bool operator<(const vec2 &other) const;
        bool operator<=(const vec2 &other) const;
        bool operator>(const vec2 &other) const;
        bool operator>=(const vec2 &other) const;

        float Magnitude() const;
        vec2 Normalise() const;
        float Distance(const vec2& other) const;
        uint16_t Dot(const vec2& other) const;
};
