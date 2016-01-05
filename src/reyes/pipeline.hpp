#pragma once

#include "mem.hpp"
#include "backend.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "settings.hpp"

namespace reyes
{
    void render(Scene& scene, Camera& camera)
    {
        Microgrid grid;

        while (scene)
        {
            mem::blk shp_blk = scene.pop();
            Shape* shape = static_cast<Shape*>(shp_blk.ptr);

            shape->dice(grid);                                                  // DICE

            AABB2 bb = grid.aabb();                                             // BOUND
            if (!isInFrustum(bb))                                               // | try to cull
                continue;                                                       // |

            vec2 rasSz = camera.film->estimate(bb.max - bb.min);                // SPLIT - estimate grid's raster size
            if (requiresSplit(rasSz, RASTER_THRESHOLD))                         // |
            {                                                                   // |
                shape->split(scene);                                            // | do split
            }                                                                   // |
            else                                                                // | don't split, so continue to raster
            {
                shape->shade(grid);                                             // SHADE

                camera.film->rasterize(grid);                                   // SAMPLE
            }
        }
    }
}