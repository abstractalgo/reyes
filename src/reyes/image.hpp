#pragma once 

#include "vecmx.hpp"
#include "grid.hpp"
#include <cstring>
#include <cstdint>

namespace reyes
{
    /* Image interface. */
    struct ImageI
    {
        uint16_t width, height;
        virtual char* getRGB(void) = 0;
        virtual void rasterize(GridVertexTy<color>& grid) = 0;
        ImageI(uint16_t _width, uint16_t _height)
            : width(_width)
            , height(_height)
        {}
        void writePPM(const char* filename)
        {

            FILE* ppm_file = fopen(filename, "wb");
            if (!ppm_file)
                return;

            fprintf(ppm_file, "P6\n%d %d\n%d\n", width, height, 255);
            size_t size = width * height * 3;
            char* rgbdata = getRGB();
            size_t cnt = fwrite(rgbdata, sizeof(char), size, ppm_file);
            fclose(ppm_file);
        }
    };

    /* G-Buffer storage. */
    struct GBuffer : public ImageI
    {
        struct GBufferPixel
        {
            color color;
            float z;
        } *data;

        GBuffer(uint16_t _width, uint16_t _height)
            : ImageI(_width, _height)
            , data(new GBufferPixel[_width*_height])
        {}

        void rasterize(GridVertexTy<color>& grid)
        {

        }

        char* getRGB(void)
        {
            return 0;
        }
    };
}