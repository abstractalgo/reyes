#pragma once

#include "mem.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "image.hpp"

namespace reyes
{
    const vec2 RASTER_THRESHOLD = { 16, 16 };

    /* Main rendering functions that goes through all the rendering stages:
    1. bound-split
    2. dice
    3. shade
    4. sample
    */
    template<class FilmTy, uint16_t width, uint16_t height>
    void render(Scene& scene, Camera<FilmTy, width, height>& camera)
    {
        mem::mAllocator alok;
        MicrogridI<PosColor>* shadedGrid;
        MicrogridI<PosNormalUV>* dicedGrid;

        // BOUND-SPLIT
        while (scene)
        {
            mem::blk shp_blk = scene.pop();
            ShapeI* shape = static_cast<ShapeI*>(shp_blk.ptr);
            
            mem::blk dg_blk = shape->dice(&alok);                               // DICE
            dicedGrid = static_cast<MicrogridI<PosNormalUV>*>(dg_blk.ptr);      // |
            dicedGrid->transform(shape->transform);                             // | transform diced grid
            dicedGrid->transform(camera.view);                                  // |
            dicedGrid->transform(camera.projection);                            // |

            AABB2 bb = dicedGrid->aabb();                                       // BOUND
            vec2 rasSz = camera.image.estimate(bb.max - bb.min);                // | estimate diced grid's raster size

            SplitDir dir = split_dir(rasSz, RASTER_THRESHOLD);                  // SPLIT
            if (SplitDir::NoSplit != dir)                                       // |
            {                                                                   // |
                shape->split(dir, scene);                                       // |
            }                                                                   // |
            else
            {
                mem::blk sg_blk = shape->shade(dicedGrid, &alok);               // SHADE
                shadedGrid = static_cast<MicrogridI<PosColor>*>(sg_blk.ptr);    // |

                camera.capture(shadedGrid);                                     // SAMPLE
                alok.free(sg_blk);
            }
            alok.free(dg_blk);
            scene.free(shp_blk);
        }
    }
}