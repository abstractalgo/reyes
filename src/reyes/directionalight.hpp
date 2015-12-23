#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        struct DirectionalLight : public Light
        {
            vec3 direction;
            color col;
            DirectionalLight(color c, vec3 d)
                : col(c)
                , direction(d)
            {}
            color sample(PosNormal point)
            {
                vec3 n = point.n.normalize();
                direction.normalize();
                float k = n.x*direction.x + n.y*direction.y + n.z*direction.z;
                k = k < 0.0f ? 0.0f : k;
                return{ col.r*k, col.g*k, col.b*k, 1.0f };
            }
        };
    }
}