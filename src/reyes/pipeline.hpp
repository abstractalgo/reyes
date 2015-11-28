#pragma once

#include "vecmx.hpp"
#include "shape.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "image.hpp"



namespace reyes
{
    template<class ImageTy>
    /* Main rendering functions that goes through all the rendering stages:
    1. bound-split
    2. dice
    3. shade
    4. sample
    */
    void render(Shape* scene, camera camera, ImageTy& image)
    {
        // bound-split
        // dice
        // shade
        // sample
    }
}