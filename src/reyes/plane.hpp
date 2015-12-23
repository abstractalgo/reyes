#pragma once

#include "shape.hpp"

namespace reyes
{
    namespace lib
    {
        template<class MaterialTy>
        struct Plane : public Shape<MaterialTy>
        {
            vec2 dim;
            vec3 center;

            Plane(vec3 c = { 0, 0, 0 }, vec2 d = { 2, 2 })
                : dim(d)
                , center(c)
            {}

            normal N(uv uv)
            {
                return vec3(0, 0, 1);
            }
            position P(uv uv)
            {
                vec3 tl = center - vec3(dim.x*0.5f, -dim.y*0.5f, 0);
                return vec3(tl.x + dim.x*uv.x, tl.y - dim.y*uv.y, 0);
            }

            void split(SplitDir direction, Scene& scene)
            {
                mem::blk one_blk = scene.alloc(sizeof(Plane<MaterialTy>));
                mem::blk two_blk = scene.alloc(sizeof(Plane<MaterialTy>));
                Plane<MaterialTy>& one = *(::new(one_blk.ptr) Plane<MaterialTy>);
                Plane<MaterialTy>& two = *(::new(two_blk.ptr) Plane<MaterialTy>);

                one.center = center;
                two.center = center;
                one.dim = dim;
                two.dim = dim;

                splitUV(direction, one, two);
            }
        };
    }
}