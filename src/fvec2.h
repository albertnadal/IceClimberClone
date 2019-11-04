#pragma once

#include <defines.h>
#include "vec2.h"

class fvec2
{
    fvec2();
    //fvec2(float scalar);

    float x, y;

        fvec2(float x, float y);

        fvec2 &Add(const fvec2 &other);
        fvec2 &Subtract(const fvec2 &other);
        fvec2 &Multiply(const fvec2 &other);
        fvec2 &Divide(const fvec2 &other);

        fvec2& Add(float value);
        fvec2& Subtract(float value);
        fvec2& Multiply(float value);
        fvec2& Divide(float value);

        friend fvec2 operator+(fvec2 left, const fvec2 &right);
        friend fvec2 operator-(fvec2 left, const fvec2 &right);
        friend fvec2 operator*(fvec2 left, const fvec2 &right);
        friend fvec2 operator/(fvec2 left, const fvec2 &right);

        friend fvec2 operator+(fvec2 left, float value);
        friend fvec2 operator-(fvec2 left, float value);
        friend fvec2 operator*(fvec2 left, float value);
        friend fvec2 operator/(fvec2 left, float value);

        bool operator==(const fvec2 &other) const;
        bool operator!=(const fvec2 &other) const;

        fvec2& operator+=(const fvec2 &other);
        fvec2& operator-=(const fvec2 &other);
        fvec2& operator*=(const fvec2 &other);
        fvec2& operator/=(const fvec2 &other);

        fvec2& operator+=(float value);
        fvec2& operator-=(float value);
        fvec2& operator*=(float value);
        fvec2& operator/=(float value);

        bool operator<(const fvec2 &other) const;
        bool operator<=(const fvec2 &other) const;
        bool operator>(const fvec2 &other) const;
        bool operator>=(const fvec2 &other) const;

        float Magnitude() const;
/*        fvec2 Normalise() const;
        float Distance(const fvec2& other) const;
        float Dot(const fvec2& other) const;*/
};
