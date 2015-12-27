#pragma once

#include <cstdint>
#include <cmath>
#include "color.hpp"
#include "settings.hpp"

namespace reyes
{
    struct Microgrid
    {
        Vertex vertices[GRID_SIZE];
        Index indices[GRID_IDX_SIZE];

        Microgrid()
        {
            for (uint16_t v = 0; v < GRID_DIM; v++)
            for (uint16_t u = 0; u < GRID_DIM; u++)
            {
#if GRID_TYPE==TRIANGLE_GRID
                // ADB
                indices[v * (GRID_DIM * 6) + u * 6 + 0] = (v * (GRID_DIM + 1)) + (u);
                indices[v * (GRID_DIM * 6) + u * 6 + 1] = ((v + 1) * (GRID_DIM + 1)) + (u);
                indices[v * (GRID_DIM * 6) + u * 6 + 2] = (v * (GRID_DIM + 1)) + (u + 1);

                // BDC
                indices[v * (GRID_DIM * 6) + u * 6 + 3] = (v * (GRID_DIM + 1)) + (u + 1);
                indices[v * (GRID_DIM * 6) + u * 6 + 4] = ((v + 1) * (GRID_DIM + 1)) + (u);
                indices[v * (GRID_DIM * 6) + u * 6 + 5] = ((v + 1) * (GRID_DIM + 1)) + (u + 1);
#else
                indices[v * (GRID_DIM * 4) + u * 4 + 0] = (v * (GRID_DIM + 1)) + (u);
                indices[v * (GRID_DIM * 4) + u * 4 + 1] = ((v + 1) * (GRID_DIM + 1)) + (u);
                indices[v * (GRID_DIM * 4) + u * 4 + 2] = ((v + 1) * (GRID_DIM + 1)) + (u + 1);
                indices[v * (GRID_DIM * 4) + u * 4 + 3] = (v * (GRID_DIM + 1)) + (u + 1);
#endif
            }
        }

        /* Bounding box for entire grid. */
        AABB2 aabb(void)
        {
            AABB2 bb;
            bb.max = vec2(vertices[0].p.x, vertices[0].p.y);
            bb.min = bb.max;
            for (uint16_t i = 1; i < GRID_SIZE; i++)
            {
                vec3 p = vertices[i].p;
                bb.min = vec2(fminf(bb.min.x, p.x), fminf(bb.min.y, p.y));
                bb.max = vec2(fmaxf(bb.max.x, p.x), fmaxf(bb.max.y, p.y));
            }
            return bb;
        }
    };
}