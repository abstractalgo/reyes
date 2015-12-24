#pragma once

#include "shape.hpp"

namespace reyes
{
    namespace lib
    {
        template<class MaterialTy>
        struct Sphere : public Shape<MaterialTy>
        {
            position P(uv uv)
            {
                vec3 n = N(uv);
                return n;
            }
            normal N(uv uv)
            {
                float theta = uv.x*2.0f*M_PI;
                float phi = uv.y* M_PI;

                float z = cos(theta) * sin(phi);
                float x = sin(theta) * sin(phi);
                float y = cos(phi);

                vec3 n(x,y,z);
                n.normalize();

                return n;
            }
            void split(SplitDir direction, Scene& scene)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Sphere<MaterialTy>));
                    Sphere<MaterialTy>* p = ::new(mblks[i].ptr) Sphere<MaterialTy>;
                    Shape<MaterialTy>::splitData(p, i);
                }
            }
        };
    }
}