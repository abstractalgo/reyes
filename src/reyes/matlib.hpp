#pragma once

#include "material.hpp"

#define UNIFORM(matName) struct matName##UnifBlk
#define MATERIAL(matName) struct matName : public Material<matName##UnifBlk>
#define DISPLACE position pShdr(PosNormalUV& vertex)
#define SHADE color cShdr(PosNormalUV& vertex)

namespace reyes
{
    namespace matlib
    {
        UNIFORM(Lambert) {};
        MATERIAL(Lambert)
        {
            DISPLACE
            {
                return{ 0, 0, 0};
            }

            SHADE
            {
                return{ 0, 0, 0, 1 };
            }
        };

        UNIFORM(YellowColor) {};
        MATERIAL(YellowColor)
        {
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                return{ 1, 1, 0, 1 };
            }
        };
    }
}

#undef UNIFORM
#undef MATERIAL
#undef DISPLACE
#undef SHADE