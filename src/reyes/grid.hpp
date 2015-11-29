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
        uint16_t count;

        // used for other primitives (TODO later)
        uint16_t* indices;
        uint8_t size;

        void project(camera& camera)
        {
            // TODO
        }
    };

    template<>
    struct Grid<position>
    {
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
        Grid<PosColor> shade(void)
        {
            // TODO
            return Grid<PosColor>{};
        }
    };
}