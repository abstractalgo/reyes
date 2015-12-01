#pragma once

#include "material.hpp"

#define usemat(matName) matName##Att,matName

#define MATERIAL(matName) struct matName : public Material<matName##Att, matName##Unif>
#define DISPLACE(name) position pShdr(name##Att& vertex)
#define SHADE(name) color cShdr(name##Att& vertex)

namespace reyes
{
    namespace matlib
    {
        struct EmptyMaterial {};
        struct LambertAtt
        {
            position p;
            normal n;
        };
        struct LambertUnif {};
        MATERIAL(Lambert)
        {
            DISPLACE(Lambert)
            {
                return{ 0, 0, 0};
            }

            SHADE(Lambert)
            {
                return{ 0, 0, 0, 1 };
            }
        };

        typedef Pos SolidColorAtt;
        struct SolidColorUnif {};
        MATERIAL(SolidColor)
        {
            DISPLACE(SolidColor)
            {
                return{ 0, 0, 0 };
            }

            SHADE(SolidColor)
            {
                return{ 1, 0, 0, 1 };
            }
        };
    }
}

#undef MATERIAL
#undef DISPLACE
#undef SHADE