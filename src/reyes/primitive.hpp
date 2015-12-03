#pragma once

#include <cstdint>
#include "misc.hpp"
//#include "vecmx.hpp"

namespace reyes
{
    namespace primitive
    {
        // --- primitives ----------------------------------------------------------
        inline float v2x(vec3 a, vec3 b)
        {
            return (a.x*b.y - a.y*b.x);
        }
        inline bool on_side(vec3 test, vec3 v1, vec3 v2)
        {
            //return v2x(v2 - v1, test - v1) >= 0.0f;
            return ((v2.x - v1.x)*(test.y - v1.y) - (v2.y - v1.y)*(test.x - v1.x)) >= 0.0;
        }

        template<class VertexTy>
        struct PrimitiveI
        {
            
        };

        template<class VertexTy>
        struct Quadrilateral : public PrimitiveI<VertexTy>
        {
            VertexTy a, b, c, d;
        };

        template<class VertexTy>
        struct Triangle : public PrimitiveI<VertexTy>
        {
            VertexTy a, b, c;
        };

        template<class VertexTy>
        struct Polygon : public PrimitiveI<VertexTy>
        {
            VertexTy* vertices;
            uint16_t count;
        };

        bool inQuad(Quadrilateral<PosColor> q, vec3 p)
        {
            bool isAB = on_side(p, q.a.p, q.b.p);
            bool isBC = on_side(p, q.b.p, q.c.p);
            bool isCD = on_side(p, q.c.p, q.d.p);
            bool isDA = on_side(p, q.d.p, q.a.p);
            return !isAB && !isBC && !isCD && !isDA;
        }

        vec2 getUV(Quadrilateral<PosColor> q, vec3 p)
        {
            vec3 e = q.b.p - q.a.p;
            vec3 f = q.d.p - q.a.p;
            vec3 g = q.a.p - q.b.p + q.c.p - q.d.p;
            vec3 h = p - q.a.p;

            float k2 = v2x(g, f);
            float k1 = v2x(e, f) + v2x(h, g);
            float k0 = v2x(h, e);

            float w = k1*k1 - 4.0*k0*k2;

            if (w<0.0) return vec2(0, 0);

            w = sqrt(w);

            float v1 = (-k1 - w) / (2.0*k2);
            float v2 = (-k1 + w) / (2.0*k2);
            float u1 = (h.x - f.x*v1) / (e.x + g.x*v1);
            float u2 = (h.x - f.x*v2) / (e.x + g.x*v2);
            bool  b1 = v1>0.0 && v1<1.0 && u1>0.0 && u1<1.0;
            bool  b2 = v2>0.0 && v2<1.0 && u2>0.0 && u2<1.0;

            vec2 res = vec2(0, 0);

            if (b1 && !b2) res = vec2(u1, v1);
            if (!b1 &&  b2) res = vec2(u2, v2);

            return res;
        }

        // --- bounding boxes ------------------------------------------------------

        /*template<>
        struct Triangle<PosNormalUV>
        {
            AABB2 aabb(void) const
            {
                AABB2 bb;
                bb.min = vec2(fminf(a.p.x, fminf(b.p.x, fminf(c.p.x, d.p.x))), fminf(a.p.y, fminf(b.p.y, fminf(c.p.y, d.p.y))));
                bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, fmaxf(c.p.x, d.p.x))), fmaxf(a.p.y, fmaxf(b.p.y, fmaxf(c.p.y, d.p.y))));
                return bb;
            }
        };

        template<>
        struct Quadrilateral<PosNormalUV>
        {
            AABB2 aabb(void) const
            {
                AABB2 bb;
                bb.min = vec2(fminf(a.p.x, fminf(b.p.x, c.p.x)), fminf(a.p.y, fminf(b.p.y, c.p.y)));
                bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, c.p.x)), fmaxf(a.p.y, fmaxf(b.p.y, c.p.y)));
                return bb;
            }
        };

        template<>
        struct Polygon<PosNormalUV>
        {
            AABB2 aabb(void) const
            {
                AABB2 bb;

                if (count == 0)
                    return bb;

                bb.min = vec2(vertices[0].p.x, vertices[0].p.y);
                bb.max = vec2(vertices[0].p.x, vertices[0].p.y);
                for (uint8_t i = 1; i < count; i++)
                {
                    bb.min = vec2(fminf(bb.min.x, vertices[i].p.x), fminf(bb.min.y, vertices[i].p.y));
                    bb.max = vec2(fmaxf(bb.max.x, vertices[i].p.x), fmaxf(bb.max.y, vertices[i].p.y));
                }
                return bb;
            }
        };*/
    }
}