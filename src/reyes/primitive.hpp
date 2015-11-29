#pragma once

#include <cstdint>
#include "vecmx.hpp"
#include "grid.hpp"

namespace reyes
{
    struct PrimitiveI
    {
        virtual color get(position position) const = 0;
        virtual AABB2 aabb(void) const = 0;
    };

    struct Quadrilateral : public PrimitiveI
    {
        PosColor a, b, c, d;

        color get(position position) const
        {
            color col;
            // TODO: calc relative position and interpolate
            return col;
        }

        AABB2 aabb(void) const
        {
            AABB2 bb;
            bb.min = vec2(fminf(a.position.x, fminf(b.position.x, fminf(c.position.x, d.position.x))), fminf(a.position.y, fminf(b.position.y, fminf(c.position.y, d.position.y))));
            bb.max = vec2(fmaxf(a.position.x, fmaxf(b.position.x, fmaxf(c.position.x, d.position.x))), fmaxf(a.position.y, fmaxf(b.position.y, fmaxf(c.position.y, d.position.y))));
            return bb;
        }
    };

    struct Triangle : public PrimitiveI
    {
        PosColor a, b, c;

        color get(position position) const
        {
            color col;
            // TODO: calc relative position and interpolate
            return col;
        }

        AABB2 aabb(void) const
        {
            AABB2 bb;
            bb.min = vec2(fminf(a.position.x, fminf(b.position.x, c.position.x)), fminf(a.position.y, fminf(b.position.y, c.position.y)));
            bb.max = vec2(fmaxf(a.position.x, fmaxf(b.position.x, c.position.x)), fmaxf(a.position.y, fmaxf(b.position.y, c.position.y)));
            return bb;
        }
    };

    struct Polygon : public PrimitiveI
    {
        PosColor* vertices;
        uint8_t count;

        color get(position position) const
        {
            color col;
            // TODO: calc relative position and interpolate
            return col;
        }

        AABB2 aabb(void) const
        {
            AABB2 bb;

            if (count == 0)
                return bb;

            bb.min = vec2(vertices[0].position.x, vertices[0].position.y);
            bb.max = vec2(vertices[0].position.x, vertices[0].position.y);
            for (uint8_t i = 1; i < count; i++)
            {
                bb.min = vec2(fminf(bb.min.x, vertices[i].position.x), fminf(bb.min.y, vertices[i].position.y));
                bb.max = vec2(fmaxf(bb.max.x, vertices[i].position.x), fmaxf(bb.max.y, vertices[i].position.y));
            }
            return bb;
        }
    };
}