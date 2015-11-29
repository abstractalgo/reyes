#pragma once 

#include "vecmx.hpp"
#include <cstdint>

namespace reyes
{
    /* Image storage. */
    struct Image
    {
        Image(uint16_t _width, uint16_t _height) {}
        void rasterize(Grid<PosColor>& grid) {}
    };

    struct GBuffer : public Image
    {
        uint16_t width, height;
        struct GBufferPixel
        {
            color color;
            float z;
        } *data;

        GBuffer(uint8_t _width, uint8_t _height)
            : Image(_width, _height)
            , width(_width)
            , height(_height)
            , data(new GBufferPixel[width*height])
        {}

        void rasterize(Grid<PosColor>& grid) {}
    };
}