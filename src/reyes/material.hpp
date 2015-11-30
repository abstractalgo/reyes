#pragma once

#include <cstdint>
#include "misc.hpp"

namespace reyes
{
    struct MaterialI
    {
        virtual position pShdr(void) = 0;
        virtual color cShdr(void) = 0;
    };

    template<class AttributeTy, class UniformTy>
    struct Material : public MaterialI
    {
        UniformTy uniform;
        virtual position pShdr(AttributeTy& input) = 0;
        virtual color cShdr(AttributeTy& input) = 0;
    };
}