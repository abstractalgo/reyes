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

    // DYNAMIC VERSION
    /*template<class InputTy>
    struct Material : public MaterialI
    {
        InputTy vertex;
        virtual position pShdr(void) = 0;
        virtual color cShdr(void) = 0;
    };*/

    // STATIC VERSION (tru later)
    template<class AttributeTy, class UniformTy>
    struct Material : public MaterialI
    {
        AttributeTy uniform;
        virtual position pShdr() = 0;
        virtual color cShdr() = 0;
    };

    struct LambertBlock
    {};
    
    struct Lambert : public Material<PosNormal, LambertBlock>
    {
        position pShdr()
        {
            return{ 0, 0, 0 };
        }

        color cShdr()
        {
            return{ 0, 0, 0, 1 };
        }
    };

    template<class VertexTy, class MaterialTy>
    struct Grid
    {
        VertexTy data[2];
        MaterialTy material;
    };
}