#pragma once

#include "parametric.hpp"

namespace reyes
{
    template<class MaterialTy>
    struct Sphere : public Surface<MaterialTy>
    {
        float R;
        vec3 center;

        Sphere(vec3 c = { 0, 0, 0 }, float r = 1.0f)
            : R(r)
            , center(c)
        {}

        normal N(uv uv)
        {
            float vang = asin(uv.y * 2.0f - 1.0f);
            float uang = uv.x*M_PI * 2.0f;

            return vec3(sin(uang)*cos(vang),
                -(uv.y * 2.0f - 1.0f),
                cos(uang)*cos(vang)
                ).normalize();
        }
        position P(uv uv)
        {
            float vang = asin(uv.y * 2.0f - 1.0f);
            float uang = (uv.x*M_PI * 2.0f);

            return vec3(sin(uang)*cos(vang),
                -(uv.y * 2.0f - 1.0f),
                cos(uang)*cos(vang)
                ) * R + center;
        }

        void split(SplitDir direction, Scene& scene)
        {
            mem::blk mblks[4];
            for (char i = 0; i < 4; i++)
            {
                mblks[i] = scene.alloc(sizeof(Sphere<MaterialTy>));
                Sphere<MaterialTy>* p = ::new(mblks[i].ptr) Sphere<MaterialTy>;
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
            //splitUV(direction, one, two);
        }
    };
}