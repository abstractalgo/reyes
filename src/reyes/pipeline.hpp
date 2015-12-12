#pragma once

#include "mem.hpp"
#include "backend.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "image.hpp"

namespace reyes
{
    template<class FilmTy, uint16_t width, uint16_t height>
    void render(Scene& scene, Camera<FilmTy, width, height>& camera)
    {
        static const vec2 RASTER_THRESHOLD = { 32, 32 };

        mem::mAllocator alok;
        MicrogridI<PosColor>* shadedGrid;
        MicrogridI<PosNormalUV>* dicedGrid;

        PerfMarker("REYES render");

        while (scene)
        {
            mem::blk shp_blk = scene.pop();
            ShapeI* shape = static_cast<ShapeI*>(shp_blk.ptr);
            
            mem::blk dg_blk = shape->dice(&camera, &alok);                      // DICE
            dicedGrid = static_cast<MicrogridI<PosNormalUV>*>(dg_blk.ptr);      // |

            AABB2 bb = dicedGrid->aabb();                                       // BOUND
            if (bb.max.x <= -1.0f || bb.min.x >= 1.0f ||                        // | try to cull
                bb.min.y >= 1.0f || bb.max.y <= -1.0f)                          // |
                goto memoryCleanup;                                             // |

            vec2 rasSz = camera.image.estimate(bb.max - bb.min);                // SPLIT - estimate grid's raster size
            SplitDir dir = split_dir(rasSz, RASTER_THRESHOLD);                  // | determine if to split and how
            if (SplitDir::NoSplit != dir)                                       // |
            {                                                                   // |
                shape->split(dir, scene);                                       // | do split
            }                                                                   // |
            else                                                                // | don't split, so continue to raster
            {
                mem::blk sg_blk = shape->shade(dicedGrid, &alok);               // SHADE
                shadedGrid = static_cast<MicrogridI<PosColor>*>(sg_blk.ptr);    // |

                camera.capture(shadedGrid);                                     // SAMPLE
                alok.free(sg_blk);
            }

        memoryCleanup:
            alok.free(dg_blk);
            scene.free(shp_blk);
        }
    }
}