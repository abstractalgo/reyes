#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        UNIFORM(NormalColor) {};
        MATERIAL(NormalColor)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                vec3 n(vertex.n.x*0.5f + 0.5f, vertex.n.y*0.5f + 0.5f, vertex.n.z*0.5f + 0.5f);
                n.normalize();
                return{ n.x, n.y, n.z, 1 };
            }
        };
    }
}