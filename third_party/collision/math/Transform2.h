#pragma once

#include <collision/structures/vec2.hpp>
#include <collision/structures/mat2x2.hpp>
#include <cmath>

namespace collision
{

class Transform2
{
  public:
    Transform2() : translation(0, 0), scaleV(1, 1)
    {
    }

    Transform2(collision::vec2<float> position) : translation(position), scaleV(1, 1)
    {
    }

    const collision::vec2<float> &getTranslation() const
    {
        return translation;
    }

    void translate(collision::vec2<float> vector)
    {
        translation += vector;
    }

    void translate(float x, float y)
    {
        translate(collision::vec2<float>(x, y));
    }

    void scale(float x, float y)
    {
        scale(collision::vec2<float>(x, y));
    }

    void scale(collision::vec2<float> o)
    {
        scaleV = o;
    }

    void transform(collision::vec2<float> &v) const
    {
        float x = v.x;
        float y = v.y;

        v.x = rotation[0][0] * x + rotation[0][1] * y + translation.x;
        v.y = rotation[1][0] * x + rotation[1][1] * y + translation.y;
    }

    collision::vec2<float> getTransformed(const collision::vec2<float> &v) const
    {
        collision::vec2<float> tv;

        float x = v.x;
        float y = v.y;

        tv.x = rotation[0][0] * x + rotation[0][1] * y + translation.x;
        tv.y = rotation[1][0] * x + rotation[1][1] * y + translation.y;

        return tv;
    }

    collision::vec2<float> getInverseTransformedR(collision::vec2<float> v) const
    {
        collision::vec2<float> tv;

        float x = v.x;
        float y = v.y;

        // since the transpose of a rotation matrix is the inverse
        tv.x = rotation[0][0] * x + rotation[1][0] * y;
        tv.y = rotation[0][1] * x + rotation[1][1] * y;

        return tv;
    }

  private:
    collision::vec2<float> translation;
    collision::vec2<float> scaleV;
    collision::tmat2x2<float> rotation;
};

} // namespace collision
