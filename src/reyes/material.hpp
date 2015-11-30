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

    template<class AttributeTy, class UniformTy>
    struct Material
    {
        UniformTy uniform;
        virtual position pShdr(AttributeTy& a) = 0;
        virtual color cShdr(AttributeTy& a) = 0;
    };
    
}