#pragma once

#include "shape.hpp"

namespace reyes
{
    namespace lib
    {

        template<class MaterialTy>
        struct Sphere : public Shape<MaterialTy>
        {
            float R;
            vec3 center;

            Sphere(vec3 c = { 0, 0, 0 }, float r = 1.0f)
                : R(r)
                , center(c)
            {}

            position P(uv uv)
            {
                vec3 n = N(uv);

                return n*R + center;
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
                    p->material.uniform = material.uniform;     // memory garbage
                    p->center = center;
                    p->R = R;
                    if (0 == i)
                    {
                        p->start_u = start_u;
                        p->end_u = (start_u + end_u)*0.5f;
                        p->start_v = start_v;
                        p->end_v = (start_v + end_v)*0.5f;
                    }
                    else if (1 == i)
                    {
                        p->start_u = (start_u + end_u)*0.5f;
                        p->end_u = end_u;
                        p->start_v = start_v;
                        p->end_v = (start_v + end_v)*0.5f;
                    }
                    else if (2 == i)
                    {
                        p->start_u = start_u;
                        p->end_u = (start_u + end_u)*0.5f;
                        p->start_v = (start_v + end_v)*0.5f;
                        p->end_v = end_v;
                    }
                    else if (3 == i)
                    {
                        p->start_u = (start_u + end_u)*0.5f;
                        p->end_u = end_u;
                        p->start_v = (start_v + end_v)*0.5f;
                        p->end_v = end_v;
                    }
                }
            }
        };
    }
}