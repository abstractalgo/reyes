#pragma once

#include "material.hpp"

//#define UNIFORM(matName) struct matName##UnifBlk
//#define MATERIAL(matName) struct matName : public Material<matName##UnifBlk>
//#define DISPLACE position pShdr(PosNormalUV& vertex)
//#define SHADE color cShdr(PosNormalUV& vertex)

namespace reyes
{
    namespace matlib
    {
        struct LambertUnifBlk
        {
            // no uniforms
        };
        struct Lambert : public Material<LambertUnifBlk>
        {
            position pShdr(PosNormalUV& vertex)
            {
                return{ 0, 0, 0};
            }

            color cShdr(PosNormalUV& vertex)
            {
                return{ 0, 0, 0, 1 };
            }
        };

        //UNIFORM(SolidColor)
        //{
        //    // no uniforms
        //};
        //MATERIAL(SolidColor)
        //{
        //    DISPLACE
        //    {
        //        return{ 0, 0, 0 };
        //    }

        //    SHADE
        //    {
        //        return{ 1, 0, 0, 1 };
        //    }
        //};
    }
}

//#undef UNIFORM
//#undef MATERIAL
//#undef DISPLACE
//#undef SHADE