#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(UVColor)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                return{ vertex.uv.x, vertex.uv.y, 0, 1 };
            }
        };
    }
}