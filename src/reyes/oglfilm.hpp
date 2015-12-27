#pragma once

#include "film.hpp"
#include "backend.hpp"
#include <cassert>

namespace reyes
{
    struct OGLFilm : public FilmI
    {
        GLuint fbo, rb, tex;
        char* data;
        OGLFilm(uint16_t _width, uint16_t _height)
            : FilmI(_width, _height)
            , data(0)
        {
            glGenRenderbuffers(1, &rb);
            glBindRenderbuffer(GL_RENDERBUFFER, rb);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }

        char* getRGB(void)
        {
            data = new char[3 * width*height];
            glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
            return data;
        }
        void rasterize(Microgrid& grid)
        {
#if GRID_TYPE==TRIANGLE_GRID
                glBegin(GL_TRIANGLES);
                for (uint16_t i = 0; i < GRID_PRIM_SIZE; i++)
                {
                    Vertex a = grid.vertices[grid.indices[3 * i + 0]];
                    Vertex b = grid.vertices[grid.indices[3 * i + 1]];
                    Vertex c = grid.vertices[grid.indices[3 * i + 2]];
                    glColor3f(a.c.r, a.c.g, a.c.b);
                    glVertex3f(a.p.x, a.p.y, -a.p.z);

                    glColor3f(b.c.r, b.c.g, b.c.b);
                    glVertex3f(b.p.x, b.p.y, -b.p.z);

                    glColor3f(c.c.r, c.c.g, c.c.b);
                    glVertex3f(c.p.x, c.p.y, -c.p.z);
                }
                glEnd();
#else
                glBegin(GL_QUADS);
                for (uint16_t i = 0; i < GRID_PRIM_COUNT; i++)
                {
                    Vertex a = grid.vertices[grid.indices[4 * i + 0]];
                    Vertex b = grid.vertices[grid.indices[4 * i + 1]];
                    Vertex c = grid.vertices[grid.indices[4 * i + 2]];
                    Vertex d = grid.vertices[grid.indices[4 * i + 3]];
                    glColor3f(a.c.r, a.c.g, a.c.b);
                    glVertex3f(a.p.x, a.p.y, -a.p.z);

                    glColor3f(b.c.r, b.c.g, b.c.b);
                    glVertex3f(b.p.x, b.p.y, -b.p.z);

                    glColor3f(c.c.r, c.c.g, c.c.b);
                    glVertex3f(c.p.x, c.p.y, -c.p.z);

                    glColor3f(d.c.r, d.c.g, d.c.b);
                    glVertex3f(d.p.x, d.p.y, -d.p.z);
                }
                glEnd();
#endif
        }

        ~OGLFilm()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteRenderbuffers(1, &rb);
            glDeleteTextures(1, &tex);
            glDeleteFramebuffers(1, &fbo);
            fbo = 0;
            tex = 0;
            rb = 0;
            delete[] data;
        }

        void display(GLuint tt = 0)
        {
            FilmI::display(tex);
        }
    };
}