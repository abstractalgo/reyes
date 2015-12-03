#pragma once 

#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"
#include <cstring>
#include <cstdint>

namespace reyes
{
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
            uint16_t p_cnt = grid.count();
            for (uint16_t pidx = 0; pidx < p_cnt; pidx++)
            {
                primitive::PrimitiveI<PosColor>* p = grid.at(pidx);
                primitive::Quadrilateral<PosColor>* _shiet = (primitive::Quadrilateral<PosColor>*)p;
                int i = 0;
            }
        }

        char* getRGB(void)
        {
            return 0;
        }
    };
}