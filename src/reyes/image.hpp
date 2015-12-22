#pragma once 

#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"
#include <cstring>
#include <cstdint>
#include "ogl_help.hpp"

namespace reyes
{
    /* Image interface. */
    struct ImageI
    {
        uint16_t width, height;
        float half_width, half_height;
        virtual char* getRGB(void) = 0;
        virtual void rasterize(Microgrid& grid) = 0;
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
    
        GBuffer(uint16_t _width, uint16_t _height)
            : ImageI(_width, _height)
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
                uint16_t start_x=0, end_x=width, start_y=0, end_y=height;
                // calc tight rasterization box
                for (start_x = 0; -1.0f + (2 * start_x + 1)*halfpx_x < bb.min.x && start_x<width; start_x++);
                for (end_x = start_x; -1.0f + (2 * end_x + 1)*halfpx_x < bb.max.x && end_x<width; end_x++);
                for (start_y = 0; 1.0f - (2 * start_y + 1)*halfpx_y > bb.max.y && start_y<height; start_y++);
                for (end_y = start_y; 1.0f - (2 * end_y + 1)*halfpx_y >= bb.min.y && end_y<height; end_y++);
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
            opengl_display(width, height, getRGB());
        }
    
        char* getRGB(void)
        {
            return (char*)rgb_data;
        }
    };

    struct OGLImage : public ImageI
    {
        GLuint fbo, rb, tex;
        char* data;
        OGLImage(uint16_t _width, uint16_t _height)
            : ImageI(_width, _height)
            , data(0)
        {
            /*glGenRenderbuffers(1, &rb);
            glBindRenderbuffer(GL_RENDERBUFFER, rb);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);*/
        }

        char* getRGB(void)
        {
            /*data = new char[3 * width*height];
            glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
            return data;*/
            return 0;
        }
        void rasterize(Microgrid& grid)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            //glDepthFunc(GL_GEQUAL);
            switch (grid.type)
            {
            case MicrogridType::TRIANGLE:
                glBegin(GL_TRIANGLES);
                for (uint16_t i = 0; i < grid.primCount(); i++)
                {
                    Vertex a = grid.vertices[grid.indices[3 * i + 0]];
                    Vertex b = grid.vertices[grid.indices[3 * i + 1]];
                    Vertex c = grid.vertices[grid.indices[3 * i + 2]];
                    glColor3f(a.c.r, a.c.g, a.c.b);
                    glVertex3f(a.p.x, a.p.y, a.p.z);

                    glColor3f(b.c.r, b.c.g, b.c.b);
                    glVertex3f(b.p.x, b.p.y, b.p.z);

                    glColor3f(c.c.r, c.c.g, c.c.b);
                    glVertex3f(c.p.x, c.p.y, c.p.z);
                }
                glEnd();
                break;
            case MicrogridType::QUAD:
                glBegin(GL_QUADS);
                for (uint16_t i = 0; i < grid.primCount(); i++)
                {
                    Vertex a = grid.vertices[grid.indices[4 * i + 0]];
                    Vertex b = grid.vertices[grid.indices[4 * i + 1]];
                    Vertex c = grid.vertices[grid.indices[4 * i + 2]];
                    Vertex d = grid.vertices[grid.indices[4 * i + 3]];
                    glColor3f(a.c.r, a.c.g, a.c.b);
                    glVertex3f(a.p.x, a.p.y, a.p.z);

                    glColor3f(b.c.r, b.c.g, b.c.b);
                    glVertex3f(b.p.x, b.p.y, b.p.z);

                    glColor3f(c.c.r, c.c.g, c.c.b);
                    glVertex3f(c.p.x, c.p.y, c.p.z);

                    glColor3f(d.c.r, d.c.g, d.c.b);
                    glVertex3f(d.p.x, d.p.y, d.p.z);
                }
                glEnd();
                break;
            }
        }

        ~OGLImage()
        {
            /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteRenderbuffers(1, &rb);
            glDeleteTextures(1, &tex);
            glDeleteFramebuffers(1, &fbo);*/
            fbo = 0;
            tex = 0;
            rb = 0;
            delete[] data;
        }
    };
}