#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "image.hpp"
#include <list>

namespace reyes
{
    //const vec2 RASTER_THRESHOLD = { 0, 0 };

    /* Main rendering functions that goes through all the rendering stages:
    1. bound-split
    2. dice
    3. shade
    4. sample
    */
    template<class ImageTy> void render(mem::ObjectStack<Shape>& scene, camera camera, ImageTy& image)
    {
        mem::ObjectStack<GridI<PosColor>> shadedGrids;

        // BOUND-SPLIT
        // (pass-through)

        // DICE-SHADE
        while (split_shapes)
        {
            Shape* shape = scene.pop();                                         // get shape
            GridI<PosNormalMat>* grid = shape.dice();                           // dice it (transform it in world space)
            grid->shade();
            uGrids.push_back(grid);                                             // push the data
        }
    }

    template<class ImageTy> void render_list(std::list<Shape> scene, camera camera, ImageTy& image)
    {
        std::list<Shape> unsplit_shapes = scene;
        std::list<Shape> split_shapes;
        std::list<Grid<PosNormalMat>> uGrids;
        std::list<Grid<PosColor>> shadedGrids;

        // BOUND-SPLIT
        // get all shapes under some limit size
        while (unsplit_shapes.size() > 0)
        {
            Shape shape = unsplit_shapes.front();
            unsplit_shapes.pop_front();
            split_shapes.push_back(shape);
        }

        // DICE
        // get all small shapes and dice them
        while (split_shapes.size() > 0)
        {
            Shape shape = split_shapes.front();                                 // get shape
            split_shapes.pop_front();
            Grid<PosNormalMat> grid;
            shape.dice(grid);                                                   // dice it (transform it in world space)
            uGrids.push_back(grid);                                             // push the data
        }

        // SHADE
        // run shader on grid points
        while (uGrids.size() > 0)
        {
            Grid<PosNormalMat> grid = uGrids.front();                           // get grid
            uGrids.pop_front();
            Grid<PosColor> shadedGrid = grid.shade();                           // shade points in it
            shadedGrids.push_back(shadedGrid);                                  // pust the data
        }

        // SAMPLE
        // sample shaded points and get samples to be injected into a color buffer
        while (shadedGrids.size() > 0)
        {
            Grid<PosColor> grid = shadedGrids.front();                          // get the shaded grid
            shadedGrids.pop_front();
            grid.project(camera);                                               // project it (transform it from world space to view space) 
            image.rasterize(grid);                                              // rasterize and inject into buffer
        }
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