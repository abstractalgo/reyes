#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        struct BMPSampler : public Sampler
        {
            uint16_t width, height;
            unsigned char* data;

            BMPSampler(uint16_t w, uint16_t h, const char* filename)
                : width(w)
                , height(h)
            {
                unsigned char header[54];
                size_t imageSize;
                data = 0;

                // open the file
                FILE * file = fopen(filename, "rb");
                if (!file)
                    return;
                if (fread(header, 1, 54, file) != 54)
                    return;
                if (header[0] != 'B' || header[1] != 'M')
                    return;

                // allocate data
                imageSize = this->width * this->height * 3;
                data = new unsigned char[imageSize];
                if (!data)
                    return;

                // read data
                fread(data, 1, imageSize, file);

                // close file
                fclose(file);

                // invert BGR to RGB
                for (size_t i = 0; i < this->width * this->height; i++)
                {
                    unsigned char t = data[3 * i + 0];
                    data[3 * i + 0] = data[3 * i + 2];
                    data[3 * i + 2] = t;
                }
            }

            color sample(uv uv)
            {
                // repeat
                //while (uv.x > 1.0f) uv.x -= 1.0f;
                //while (uv.y > 1.0f) uv.y -= 1.0f;

                // calc offset
                size_t x = (size_t)(uv.x*width);
                size_t y = (size_t)((1.0f - uv.y)*height);
                size_t offset = y*width + x;

                // sample
                unsigned char R = data[3 * offset + 0];
                unsigned char G = data[3 * offset + 1];
                unsigned char B = data[3 * offset + 2];
                return{ R / 255.0f, G / 255.0f, B / 255.0f, 1.0f };
            }
        };
    }
}