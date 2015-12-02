#pragma once

#include "mem.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "image.hpp"

namespace reyes
{
    //const vec2 RASTER_THRESHOLD = { 0, 0 };

    /* Main rendering functions that goes through all the rendering stages:
    1. bound-split
    2. dice
    3. shade
    4. sample
    */
    void render(mem::ObjectStack<ShapeI>& scene, camera camera, ImageI& image)
    {
        mem::ObjectStack<GridVertexTy<color>> shadedGrids;

        // BOUND-SPLIT
        // (pass-through)

        // DICE-SHADE-SAMPLE
        while (scene) image.rasterize(*scene.pop()->shade(shadedGrids));
    }
}

// ------- FOR LATER USE & NOTES -----------------------------------------------



// can combine dice and shade and sample



// optimize memory (don't use list, but rather use some custom stack)



//while (unsplit_shapes.size()>0)                                               // if there is any big shape
//{
//    Shape shape = unsplit_shapes.front();                                     // get next shape from list
//    unsplit_shapes.pop_front();
//    Grid<position> grid;
//    shape.dice(grid, 8, 8);                                                   // coarse uGrid
//    grid.project(camera);                                                     // project uGrid

//    if (grid.visible(camera))                                                 // try to cull completely
//    {
//        vec2 rasterSize = grid.rasterEstimate();                              // estimate rasterized size
//        if (rasterSize.x > RASTER_THRESHOLD.x
//          || rasterSize.y > RASTER_THRESHOLD.y)                               // 1. too big, needs splitting
//        {

//            SplitDir dir = split_dir(rasterSize);                             // determine bigger dimension
//            Shape halves[2];
//            shape.split(dir, halves+0, halves+1);                             // split
//            unsplit_shapes.push_back(halves[0]);
//            unsplit_shapes.push_back(halves[1]);
//        }
//        else                                                                  // 2. small enough
//        {
//            split_shapes.push_back(shape);
//        }
//    }
//}