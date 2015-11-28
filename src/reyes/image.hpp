#pragma once 

#include "vecmx.hpp"
#include <cstdint>

namespace reyes
{
    /* Image storage. */
    struct Image
    {
        Image(uint16_t _width, uint16_t _height) {}
        template<class GridTy>
        void rasterize(Grid<GridTy>& grid) {}
    };

    struct GBufferPixel
    {
        color color;
        float z;
    };

    struct GBuffer : public Image
    {
        uint16_t width, height;
        GBufferPixel* data;

        GBuffer(uint8_t _width, uint8_t _height)
            : Image(_width, _height)
            , width(_width)
            , height(_height)
            , data(new GBufferPixel[width*height])
        {}

        template<class GridTy>
        void rasterize(Grid<GridTy>& grid);
    };
}