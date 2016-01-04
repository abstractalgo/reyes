#pragma once

#include "shape.hpp"

namespace reyes {
    namespace lib {

        struct Disc : public Shape
        {
            normal EvalN(uv uv)
            {
                return vec3(0, 0, 1);
            }
            position EvalP(uv uv)
            {
                float angle = 2.0f*M_PI*(1.0f - uv.x);
                vec3 v = vec3(cos(angle), -sin(angle), 0).normalize();
                float l = uv.y;
                return v*l;
            }
            void split(Scene& scene)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Disc));
                    Disc* p = ::new(mblks[i].ptr) Disc;
                    Shape::splitData(p, i);
                }
                
            }
        };
    }
}