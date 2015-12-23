#pragma once

#include "shape.hpp"

namespace reyes {
    namespace lib {

        template<class MaterialTy>
        struct Disc : public Shape<MaterialTy>
        {
            float R;
            vec3 center;

            Disc(vec3 c = { 0, 0, 0 }, float r = 1.0f)
                : R(r)
                , center(c)
            {}

            normal N(uv uv)
            {
                return vec3(0, 0, 1);
            }
            position P(uv uv)
            {
                float angle = 2.0f*M_PI*(1.0f - uv.x);
                vec3 v = vec3(cos(angle), -sin(angle), 0).normalize();
                float l = R*uv.y;
                return v*l + center;
            }

            void split(SplitDir direction, Scene& scene)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Disc<MaterialTy>));
                    Disc<MaterialTy>* p = ::new(mblks[i].ptr) Disc<MaterialTy>;
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
}