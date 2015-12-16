#pragma once

#include "parametric.hpp"

namespace reyes
{
    template<class MaterialTy>
    struct Klein : public Surface<MaterialTy>
    {
        Klein()
        {}

        normal N(uv uv)
        {
            return vec3(0, 0, 1);
        }
        position P(uv uv)
        {
            float v = uv.y;
            float u = uv.x * 2.0f;
            float x, y, z;
            if (u < M_PI)
            {

                x = 3.0f * cos(u) * (1.0f + sin(u)) + (2.0f * (1.0f - cos(u) / 2.0f)) * cos(u) * cos(v);
                z = -8.0f * sin(u) - 2.0f * (1.0f - cos(u) / 2.0f) * sin(u) * cos(v);
            }
            else
            {
                x = 3.0f * cos(u) * (1.0f + sin(u)) + (2.0f * (1.0f - cos(u) / 2.0f)) * cos(v + M_PI);
                z = -8.0f * sin(u);
            }
            y = -2.0f * (1.0f - cos(u) / 2.0f) * sin(v);
            position p(x, y, z);
            return p*0.1f;
        }

        void split(SplitDir direction, Scene& scene)
        {
            mem::blk one_blk = scene.alloc(sizeof(Klein<MaterialTy>));
            mem::blk two_blk = scene.alloc(sizeof(Klein<MaterialTy>));
            Klein<MaterialTy>& one = *(::new(one_blk.ptr) Klein<MaterialTy>);
            Klein<MaterialTy>& two = *(::new(two_blk.ptr) Klein<MaterialTy>);

            splitUV(direction, one, two);
        }
    };
}