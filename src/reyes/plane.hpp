#pragma once

#include "shape.hpp"

namespace reyes
{
    namespace lib
    {
        struct Plane : public Shape
        {
            normal EvalN(uv uv)
            {
                return vec3(0, 0, 1);
            }
            position EvalP(uv uv)
            {
                return vec3(-0.5f + uv.x, 0.5f - uv.y, 0);
            }
            void split(Scene& scene, Shape** shapes=0)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Plane));
                    Plane* p = ::new(mblks[i].ptr) Plane;
                    Shape::splitData(p, i);
                    if (shapes)
                        shapes[i] = p;
                }
            }
        };
    }
}