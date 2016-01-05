#pragma once

#include "shape.hpp"

namespace reyes
{
    namespace lib
    {
        struct Sphere : public Shape
        {
            position EvalP(uv uv)
            {
                vec3 n = EvalN(uv);
                return n;
            }
            normal EvalN(uv uv)
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
            void split(Scene& scene, Shape** shapes = 0)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Sphere));
                    Sphere* p = ::new(mblks[i].ptr) Sphere;
                    Shape::splitData(p, i);
                    if (shapes)
                        shapes[i] = p;
                }
            }
        };
    }
}