#pragma once

#include "parametric.hpp"

namespace reyes
{
    template<class MaterialTy>
    struct Sphere : public ParametricSurface<MaterialTy>
    {
        float R;
        vec3 center;

        Sphere(vec3 c = { 0, 0, 0 }, float r = 1.0f)
            : R(r)
            , center(c)
        {}

        normal N(uv uv)
        {
            float vang = asin(uv.y * 2 - 1);
            float uang = uv.x*M_PI * 2;

            return vec3(sin(uang)*cos(vang),
                -(uv.y * 2 - 1),
                cos(uang)*cos(vang)
                ).normalize();
        }
        position P(uv uv)
        {
            float vang = asin(uv.y * 2 - 1);
            float uang = (uv.x*M_PI * 2);

            return vec3(sin(uang)*cos(vang),
                -(uv.y * 2 - 1),
                cos(uang)*cos(vang)
                ) * R + center;
        }

        void split(SplitDir direction, Scene& scene)
        {
            mem::blk one_blk = scene.alloc(sizeof(Sphere<MaterialTy>));
            mem::blk two_blk = scene.alloc(sizeof(Sphere<MaterialTy>));
            Sphere<MaterialTy>& one = *(::new(one_blk.ptr) Sphere<MaterialTy>);
            Sphere<MaterialTy>& two = *(::new(two_blk.ptr) Sphere<MaterialTy>);

            one.center = center;
            two.center = center;
            one.R = R;
            two.R = R;

            splitUV(direction, one, two);
        }
    };
}