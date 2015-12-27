#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(SolidColor)
        {
            UNIFORM_BEGIN
                color col;
                Uniform()
                    : col(1, 0, 0, 1)
                {}
            UNIFORM_END
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                return uniform.col;
            }
        };
    }
}