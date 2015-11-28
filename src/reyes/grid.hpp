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

    typedef vec3 position;
    typedef vec3 normal;
    struct PosNormalMat
    {
        position position;
        normal normal;
        uint8_t material;
    };
    struct PosColor
    {
        position position;
        color color;
    };

    template<>
    class Grid<PosNormalMat>
    {
        void project(camera& camera)
        {
            // TODO
        }

        Grid<PosColor> shade(void)
        {
            // TODO
        }
    };
}