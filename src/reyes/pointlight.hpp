#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        struct PointLight : public Light
        {
            color col;
            position pos;
            color sample(PosNormal point)
            {
                float dist = (pos - point.p).len();
                float max_dist = 10.0f;
                float k = 1.0f - dist / max_dist;
                k = (k < 0.0 ? 0.0 : k);
                return{ col.r*k, col.g*k, col.b*k, 1.0f };
            }
        };
    }
}