#pragma once

#include "mem.hpp"
#include "backend.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "settings.hpp"

namespace reyes
{
    template<class FilmTy, uint16_t width, uint16_t height>
    void render(Scene& scene, Camera<FilmTy, width, height>& camera)
    {
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        Microgrid& grid = *new Microgrid;

        while (scene)
        {
            mem::blk shp_blk = scene.pop();
            Shape* shape = static_cast<Shape*>(shp_blk.ptr);

            shape->dice(grid);                                                  // DICE

            AABB2 bb = grid.aabb();                                             // BOUND
            if (!isInFrustum(bb))                                               // | try to cull
                goto memoryCleanup;                                             // |

            vec2 rasSz = camera.film.estimate(bb.max - bb.min);                 // SPLIT - estimate grid's raster size
            if (requiresSplit(rasSz, RASTER_THRESHOLD))                         // |
            {                                                                   // |
                shape->split(scene);                                            // | do split
            }                                                                   // |
            else                                                                // | don't split, so continue to raster
            {
                shape->shade(grid);                                             // SHADE

                camera.film.rasterize(grid);                                    // SAMPLE
            }

        memoryCleanup:
            printf("\rSHAPES: %d    ", scene.cnt);
        }
        delete &grid;
        SwapBuffersBackend();
    }
}