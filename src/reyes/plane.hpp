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
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Plane<MaterialTy>));
                    Plane<MaterialTy>* p = ::new(mblks[i].ptr) Plane<MaterialTy>;
                    p->material.uniform = material.uniform;
                    p->center = center;
                    p->dim = dim;
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