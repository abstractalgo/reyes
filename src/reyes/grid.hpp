#pragma once

#include <cstdint>
#include <cmath>
#include "color.hpp"

namespace reyes
{
    enum MicrogridType { QUAD=0, TRIANGLE };
    struct Microgrid
    {
        Vertex* vertices;
        Index* indices;
        uint16_t vertCount;
        uint16_t indiCount;
        MicrogridType type;

        Microgrid(MicrogridType _ty, uint16_t vcnt, uint16_t icnt)
            : type(_ty)
            , vertCount(vcnt)
            , indiCount(icnt)
            , vertices(new Vertex[vertCount])
            , indices(new Index[indiCount])
        {}

        ~Microgrid()
        {
            delete[] vertices;
            delete[] indices;
            vertices = 0;
            indices = 0;
        }

        uint16_t primCount(void)
        {
            if (MicrogridType::TRIANGLE == type)
            {
                return indiCount / 3;
            }
            else if (MicrogridType::QUAD == type)
            {
                return indiCount / 4;
            }
        }

        /* Bounding box for entire grid. */
        AABB2 aabb(void)
        {
            AABB2 bb;
            bb.max = vec2(vertices[0].p.x, vertices[0].p.y);
            bb.min = bb.max;
            for (uint16_t i = 1; i < vertCount; i++)
            {
                vec3 p = vertices[i].p;
                bb.min = vec2(fminf(bb.min.x, p.x), fminf(bb.min.y, p.y));
                bb.max = vec2(fmaxf(bb.max.x, p.x), fmaxf(bb.max.y, p.y));
            }
            return bb;
        }

        void transform(mx4 m)
        {
            // TODO
        }
    };
}