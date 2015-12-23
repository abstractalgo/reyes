#pragma once

#include "film.hpp"

namespace reyes
{
    /* Film that rasterizes image on the CPU. */
    struct CPUFilm : public FilmI
    {
        struct RGBpixel
        {
            char r, g, b;
        } *rgb_data;
        struct Apixel
        {
            float a;
        } *a_data;
        struct Zpixel
        {
            float z;
        } *z_data;

        CPUFilm(uint16_t _width, uint16_t _height)
            : FilmI(_width, _height)
            , rgb_data(new RGBpixel[_width*_height])
            , a_data(new Apixel[_width*_height])
            , z_data(new Zpixel[_width*_height])
        {
            for (uint16_t x = 0; x < width; x++)
            for (uint16_t y = 0; y < height; y++)
            {
                rgb_data[y*width + x] = { 0, 0, 0 };
                a_data[y*width + x] = { 1.0f };
                z_data[y*width + x] = { -1.0f };
            }
        }

        void rasterize(Microgrid& grid)
        {
            uint16_t p_cnt = grid.primCount();
            float halfpx_x = 1.0f / width;
            float halfpx_y = 1.0f / height;
            for (uint16_t pidx = 0; pidx < p_cnt; pidx++)
            {
                primitive::PrimitiveI* prim = grid[pidx];
                // get bounding box
                AABB2 bb = prim->aabb();
                uint16_t start_x = 0, end_x = width, start_y = 0, end_y = height;
                // calc tight rasterization box
                for (start_x = 0; -1.0f + (2 * start_x + 1)*halfpx_x < bb.min.x && start_x < width; start_x++);
                for (end_x = start_x; -1.0f + (2 * end_x + 1)*halfpx_x < bb.max.x && end_x<width; end_x++);
                for (start_y = 0; 1.0f - (2 * start_y + 1)*halfpx_y > bb.max.y && start_y < height; start_y++);
                for (end_y = start_y; 1.0f - (2 * end_y + 1)*halfpx_y >= bb.min.y && end_y < height; end_y++);
                // rasterize
                for (uint16_t x = start_x; x < end_x; x++)
                {
                    for (uint16_t y = start_y; y < end_y; y++)
                    {
                        // construct pixel location
                        float px = -1.0f + (2 * x + 1)*halfpx_x;
                        float py = 1.0f - (2 * y + 1)*halfpx_y;
                        vec3 p(px, py, 0);

                        // test
                        if (prim->in(p))
                        {
                            Vertex r = prim->interpolate(p);
                            Zpixel px_z = z_data[y*width + x];

                            // rasterized pixel should overwrite information
                            if (r.p.z > px_z.z)
                            {
                                // TODO depth test settings
                                // TODO blending
                                px_z.z = r.p.z;
                                RGBpixel px_rgb = { r.c.r*255.0f, r.c.g*255.0f, r.c.b*255.0f };
                                rgb_data[y*width + x] = px_rgb;
                                z_data[y*width + x] = px_z;
                            }
                        }
                    }
                }
            }
        }

        char* getRGB(void)
        {
            return (char*)rgb_data;
        }

        void display()
        {
            // TODO
        }
    };
}