#pragma once

#include "shape.hpp"

namespace reyes {
    namespace lib {

        template<class MaterialTy>
        struct Disc : public Shape<MaterialTy>
        {
            Disc()
            {}

            normal N(uv uv)
            {
                return vec3(0, 0, 1);
            }
            position P(uv uv)
            {
                float angle = 2.0f*M_PI*(1.0f - uv.x);
                vec3 v = vec3(cos(angle), -sin(angle), 0).normalize();
                float l = uv.y;
                return v*l;
            }

            void split(SplitDir direction, Scene& scene)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Disc<MaterialTy>));
                    Disc<MaterialTy>* p = ::new(mblks[i].ptr) Disc<MaterialTy>;
                    Shape<MaterialTy>::splitData(p, i);
                }
                
            }
        };
    }
}