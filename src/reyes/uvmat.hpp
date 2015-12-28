#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(UVColor)
        {
            struct uniform_tag
            {
                vec3 S;
                vec3 T;
            } uniform;
            DISPLACE
            {
                return vertex.p * uniform.S + uniform.T;
            }

            SHADE
            {
                return{ vertex.uv.x, vertex.uv.y, 0, 1 };
            }
        };
    }
}