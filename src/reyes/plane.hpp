#pragma once

#include "shape.hpp"

namespace reyes
{
    namespace lib
    {
        template<class MaterialTy>
        struct Plane : public Shape<MaterialTy>
        {
            normal N(uv uv)
            {
                return vec3(0, 0, 1);
            }
            position P(uv uv)
            {
                return vec3(-0.5f + uv.x, 0.5f - uv.y, 0);
            }
            void split(SplitDir direction, Scene& scene)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Plane<MaterialTy>));
                    Plane<MaterialTy>* p = ::new(mblks[i].ptr) Plane<MaterialTy>;
                    Shape<MaterialTy>::splitData(p, i);
                }
            }
        };
    }
}