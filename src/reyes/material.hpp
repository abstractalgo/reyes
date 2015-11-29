#pragma once

#include <cstdint>
#include "vecmx.hpp"

namespace reyes
{
    /* RGBA32F color struct. */
    struct color
    {
        float r, g, b, a;
        color(float _r = 0, float _g = 0, float _b = 0, float _a = 0)
            : r(_r)
            , g(_g)
            , b(_b)
            , a(_a)
        {}
    };

    template<class... InputTys>
    struct shader
    {
        color shade(InputTys... inputs);
    };

    template<class... InputTys>
    struct SolidColorShader : public shader<>
    {
        color shade()
        {
            return{ 1, 0, 0, 1 };
        }
    };

    template<class... InputTys>
    struct LambertianShader : public shader<position, normal>
    {
        color shade(position p, normal n)
        {
            return{ 0, 0, 0, 1 };
        }
    };

    struct material_factory
    {
        template<class ShaderTy>
        static void make(ShaderTy*& shdr)
        {
            shdr = new ShaderTy;
        }
    };
    typedef shader<> material;
}
// material descriptor + color(p,n)