#pragma once

#include "shape.hpp"

namespace reyes {
    namespace lib {
        struct Bezier16 : public Shape
        {
            vec3 points[16];
            Bezier16(const vec3* _p=0)
            {
                if (_p)
                {
                    for (char i = 0; i < 16; i++)
                    {
                        points[i].x = _p[i].x;
                        points[i].y = _p[i].y;
                        points[i].z = _p[i].z;
                    }
                }
            }

            vec3 evalBezierCurve4(const vec3* P, float t)
            {
                float b0 = (1 - t) * (1 - t) * (1 - t);
                float b1 = 3 * t * (1 - t) * (1 - t);
                float b2 = 3 * t * t * (1 - t);
                float b3 = t * t * t;
                return P[0] * b0 + P[1] * b1 + P[2] * b2 + P[3] * b3;
            }

            normal N(uv _uv)
            {
                float epsi = 0.01f;
                
                vec3 pa = P(_uv);
                vec3 pb = P(uv(_uv.x + epsi, _uv.y));
                vec3 pc = P(uv(_uv.x, _uv.y + epsi));

                vec3 b(pb.x - pa.x, pb.y - pa.y, pb.z - pa.z);
                vec3 a(pc.x - pa.x, pc.y - pa.y, pc.z - pa.z);

                vec3 n(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
                /*if ((_uv.x + epsi > 1.0f) || (_uv.y + epsi > 1.0f))
                    n = vec3(-n.x, -n.y, -n.z);*/
                return n.normalize();
            }
            position P(uv uv)
            {
                vec3 uCurve[4];
                for (int i = 0; i < 4; ++i)
                {
                    uCurve[i] = evalBezierCurve4(points + 4 * i, uv.x);
                }
                return evalBezierCurve4(uCurve, uv.y);
            }

            void split(Scene& scene)
            {
                mem::blk mblks[4];
                for (char i = 0; i < 4; i++)
                {
                    mblks[i] = scene.alloc(sizeof(Bezier16));
                    Bezier16* p = ::new(mblks[i].ptr) Bezier16;
                    for (char i = 0; i < 16; i++)
                    {
                        p->points[i].x = points[i].x;
                        p->points[i].y = points[i].y;
                        p->points[i].z = points[i].z;
                    }
                    Shape::splitData(p, i);
                }
            }
        };
    }
}