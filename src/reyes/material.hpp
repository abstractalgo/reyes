#pragma once

#include "misc.hpp"

namespace reyes
{
    /*struct MaterialAttribute
    {};

    struct MaterialI
    {
        virtual position pShdr(void) = 0;
        virtual color cShdr(void) = 0;
    };*/

    template<class UniformBlockTy>
    struct Material
    {
        UniformBlockTy uniform;
        virtual position pShdr(PosNormalUV& a) = 0;
        virtual color cShdr(PosNormalUV& a) = 0;
    };
    
}