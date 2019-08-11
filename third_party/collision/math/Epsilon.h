#pragma once

namespace collision
{

class Epsilon
{
  public:
    static const float &getEpsilon()
    {
        return E;
    }

    static const float compute()
    {
        float e = 0.5;
        while (1.0 + e > 1.0)
        {
            e *= 0.5;
        }
        return e;
    }

  private:
    static const float E;
};

} // namespace collision
