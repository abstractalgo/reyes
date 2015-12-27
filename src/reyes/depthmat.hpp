#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(DepthColor)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                /*float d = vertex.p.z > 1.0f ? 1.0f : vertex.p.z;
                d = vertex.p.z < 0 ? 0 : vertex.p.z;*/
                float d = vertex.p.z*.5f + .5f;
                return{ d, d, d, 1 };
            }
        };
    }
}