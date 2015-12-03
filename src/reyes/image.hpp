#pragma once 

#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"
#include <cstring>
#include <cstdint>

namespace reyes
{
    // TODO: move Image to Camera

    /* Image interface. */
    struct ImageI
    {
        uint16_t width, height;
        float half_width, half_height;
        virtual char* getRGB(void) = 0;
        virtual void rasterize(GridVertexTy<PosColor>& grid) = 0;
        ImageI(uint16_t _width, uint16_t _height)
            : width(_width)
            , height(_height)
            , half_width(_width/2.0f)
            , half_height(_height/2.0f)
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
        inline vec2 estimate(vec2 dist)
        {
            return vec2(dist.x*half_width, dist.y*half_height);
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

        void rasterize(GridVertexTy<PosColor>& grid)
        {
            // TODO
        }

        char* getRGB(void)
        {
            // TODO
            return 0;
        }
    };

    /* Primitive RGB rasterizer. */
    struct RGB8Image : public ImageI
    {
        struct RGB8Pixel
        {
            char r, g, b;
        } *data;

        RGB8Image(uint16_t _width, uint16_t _height)
            : ImageI(_width, _height)
            , data(new RGB8Pixel[_width*_height])
        {
            for (uint16_t x = 0; x < height; x++)
                for (uint16_t y = 0; y < width; y++)
                        data[x*width + y] = { 255, 0, 255 };
        }

        void rasterize(GridVertexTy<PosColor>& grid)
        {
            uint16_t p_cnt = grid.count();
            for (uint16_t pidx = 0; pidx < p_cnt; pidx++)
            {
                primitive::Quadrilateral<PosColor> * q = (primitive::Quadrilateral<PosColor>*)grid.at(pidx);
                for (uint16_t x = 0; x < height; x++)
                {
                    for (uint16_t y = 0; y < width; y++)
                    {
                        if (primitive::inQuad(*q, vec3(x, y, 0)))
                        {
                            data[x*width + y] = { 0, 0, 255 };
                        }
                    }
                }
            }
        }

        char* getRGB(void)
        {
            return (char*)data;
        }
    };
}