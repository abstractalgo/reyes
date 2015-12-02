#pragma once

#include "material.hpp"

#define usemat(matName) matName##Att,matName

#define MATERIAL(matName) struct matName : public Material<matName##UnifBlk>
#define DISPLACE position pShdr(PosNormalUV& vertex)
#define SHADE color cShdr(PosNormalUV& vertex)

namespace reyes
{
    namespace matlib
    {
        struct LambertUnifBlk {};
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

        struct SolidColorUnifBlk {};
        MATERIAL(SolidColor)
        {
            DISPLACE
            {
                return{ 0, 0, 0 };
            }

            SHADE
            {
                return{ 1, 0, 0, 1 };
            }
        };
    }
}

#undef MATERIAL
#undef DISPLACE
#undef SHADE