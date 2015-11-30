#pragma once

#include <cstdint>
#include "misc.hpp"
#include "vecmx.hpp"
#include "grid.hpp"

namespace reyes
{
    template<class VertexTy>
    struct PrimitiveI
    {
        //virtual AABB2 aabb(void) const = 0;
    };

    template<class VertexTy>
    struct Quadrilateral : public PrimitiveI<VertexTy>
    {
        VertexTy a, b, c, d;

        /*AABB2 aabb(void) const
        {
            AABB2 bb;
            bb.min = vec2(fminf(a.p.x, fminf(b.p.x, fminf(c.p.x, d.p.x))), fminf(a.p.y, fminf(b.p.y, fminf(c.p.y, d.p.y))));
            bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, fmaxf(c.p.x, d.p.x))), fmaxf(a.p.y, fmaxf(b.p.y, fmaxf(c.p.y, d.p.y))));
            return bb;
        }*/
    };

    template<class VertexTy>
    struct Triangle : public PrimitiveI<VertexTy>
    {
        VertexTy a, b, c;

        /*AABB2 aabb(void) const
        {
            AABB2 bb;
            bb.min = vec2(fminf(a.p.x, fminf(b.p.x, c.p.x)), fminf(a.p.y, fminf(b.p.y, c.p.y)));
            bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, c.p.x)), fmaxf(a.p.y, fmaxf(b.p.y, c.p.y)));
            return bb;
        }*/
    };

    template<class VertexTy>
    struct Polygon : public PrimitiveI<VertexTy>
    {
        VertexTy* vertices;
        uint16_t count;

        /*AABB2 aabb(void) const
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
        }*/
    };
}