#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        UNIFORM(SolidColor)
        {
            color col;
            SolidColorUnifBlk()
                : col(1,0,0,1)
            {}
        };
        MATERIAL(SolidColor)
        {
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