#pragma once

#include "parametric.hpp"

namespace reyes
{
    template<class MaterialTy>
    struct Disc : public Surface<MaterialTy>
    {
        float R;
        vec3 center;

        Disc(vec3 c = { 0, 0, 0 }, float r = 1.0f)
            : R(r)
            , center(c)
        {}

        normal N(uv uv)
        {
            return vec3(0, 0, 1);
        }
        position P(uv uv)
        {
            float angle = 2.0f*M_PI*(1.0f-uv.x);
            vec3 v = vec3(cos(angle), -sin(angle), 0).normalize();
            float l = R*uv.y;
            return v*l;
        }

        void split(SplitDir direction, Scene& scene)
        {
            mem::blk one_blk = scene.alloc(sizeof(Disc<MaterialTy>));
            mem::blk two_blk = scene.alloc(sizeof(Disc<MaterialTy>));
            Disc<MaterialTy>& one = *(::new(one_blk.ptr) Disc<MaterialTy>);
            Disc<MaterialTy>& two = *(::new(two_blk.ptr) Disc<MaterialTy>);

            one.center = center;
            two.center = center;
            one.R = R;
            two.R = R;

            splitUV(direction, one, two);
        }
    };
}