#pragma once

#include <cstdint>
#include <cmath>
#include "primitive.hpp"
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

        primitive::PrimitiveI* operator[](Index idx)
        {
            if (MicrogridType::TRIANGLE == type)
            {
                primitive::Triangle* t = new primitive::Triangle;
                t->a = vertices[indices[3 * idx + 0]];
                t->b = vertices[indices[3 * idx + 1]];
                t->c = vertices[indices[3 * idx + 2]];
                return t;
            }
            else if (MicrogridType::QUAD == type)
            {
                primitive::Quad* q = new primitive::Quad;
                q->a = vertices[indices[4 * idx + 0]];
                q->b = vertices[indices[4 * idx + 1]];
                q->c = vertices[indices[4 * idx + 2]];
                q->d = vertices[indices[4 * idx + 3]];
                return q;
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