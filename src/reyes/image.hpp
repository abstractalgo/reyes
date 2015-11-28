#pragma once 

#include "vecmx.hpp"
#include <cstdint>

namespace reyes
{
    template<class PixelInfo>
    /* Image storage. */
    struct Image
    {
        Image(uint16_t _width, uint16_t _height);
        void inject(vec4 location, PixelInfo pixel);
    };

    struct GBufferPixelInfo
    {
        color color;
        double z;
    };

    template<>
    struct Image<GBufferPixelInfo>
    {
        uint16_t width, height;

        Image(uint8_t _width, uint8_t _height)
            : width(_width)
            , height(_height)
        {}
    };
    typedef Image<GBufferPixelInfo> GBuffer;
}