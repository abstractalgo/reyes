#pragma once

#include <cstdint>
#include "camera.hpp"

namespace reyes
{
    template<class T>
    /* Template struct for grid data. */
    struct Grid
    {
        T* data;
        uint8_t width, height;
    };

    template<>
    struct Grid<position>
    {
        void project(camera& camera)
        {
            // TODO
        }

        bool visible(camera& camera)
        {
            // TODO
            return false;
        }

        vec2 rasterEstimate()
        {
            // TODO
            return{ 0, 0 };
        }
    };

    template<>
    struct Grid<PosNormalMat>
    {
        void project(camera& camera)
        {
            // TODO
        }

        Grid<PosColor> shade(void)
        {
            // TODO
            return Grid<PosColor>{};
        }
    };
}