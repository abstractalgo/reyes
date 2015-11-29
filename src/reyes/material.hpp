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

    /*template<class... InputTys>
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

    template<class ShaderTy>
    static void make(shader<>*& shdr)
    {
        shdr = (shader<>*)(new ShaderTy);
    }
    typedef shader<> material;*/

    template<class ShaderInputTy>
    struct shader
    {
        ShaderInputTy input;
        virtual color shade(ShaderInputTy) = 0;
    };

    struct PN_inp
    {
        position p;
        normal n;
    };

    struct SolidColorShader : public shader<void>
    {
        color shade() { return{ 0, 0, 0, 1 }; }
    };
    struct LambertShader : public shader<PN_inp>
    {
        color shade(PN_inp) { return{ 1, 0, 0, 1 }; }
    };

}
// material descriptor + color(p,n)