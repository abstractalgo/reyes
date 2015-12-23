#pragma once 

#include "vecmx.hpp"
#include "grid.hpp"
#include "color.hpp"
#include <cstring>
#include <cstdint>
#include "backend.hpp"

#define GLSL(version, shader) "#version " #version "\n" #shader

static const char* fsquad_vs = GLSL(330,
    in vec2 iPos;
    out vec2 uv;

    void main()
    {
        gl_Position = vec4(iPos, 0.5, 1.0);
        uv = (iPos + vec2(1.0)) * 0.5;
        uv.y = 1.0 - uv.y;
    }
);

static const char* fsquad_fs = GLSL(330,
    in vec2 uv;
    uniform sampler2D tex;
    out vec4 frag;

    void main()
    {
        frag = texture(tex, uv);
    }
);

namespace reyes
{
    /* Image interface. */
    struct FilmI
    {
        uint16_t width, height;
        GLuint tex, program, quad;

        virtual char* getRGB(void) = 0;
        virtual void rasterize(Microgrid& grid) = 0;
        virtual void display(GLuint tt=0)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glUseProgram(program);
            
            if (tt)
            {
                glBindTexture(GL_TEXTURE_2D, tt);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, getRGB());
            }
            glUniform1i(glGetUniformLocation(program, "tex"), 0);
            glBindBuffer(GL_ARRAY_BUFFER, quad);
            glEnableVertexAttribArray(glGetAttribLocation(program, "iPos"));
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        FilmI(uint16_t _width, uint16_t _height)
            : width(_width)
            , height(_height)
        {
            // init texture
            {
                glGenTextures(1, &tex);
                glBindTexture(GL_TEXTURE_2D, tex);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
            }

            // init program
            {
                GLuint vs, fs;
                program = glCreateProgram();
                vs = glCreateShader(GL_VERTEX_SHADER);
                fs = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(vs, 1, &fsquad_vs, NULL);
                glShaderSource(fs, 1, &fsquad_fs, NULL);
                glCompileShader(vs);
                glCompileShader(fs);
                glAttachShader(program, vs);
                glAttachShader(program, fs);
                glLinkProgram(program);
                glDeleteShader(vs);
                glDeleteShader(fs);
            }

            // inti and draw quad
            {
                GLfloat verts[12] = { -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1 };
                glGenBuffers(1, &quad);
                glBindBuffer(GL_ARRAY_BUFFER, quad);
                glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
            }
        }

        ~FilmI()
        {
            glUseProgram(0);
            glDeleteProgram(program);
            glDeleteBuffers(1, &quad);
            glDeleteTextures(1, &tex);
        }

        /* Writes RGB data to PPM file. */
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

        /* Estimate raster size of the vector in NDC space. */
        inline vec2 estimate(vec2 dist)
        {
            float half_width = width / 2.0f;
            float half_height = height / 2.0f;;
            return vec2(dist.x*half_width, dist.y*half_height);
        }
    };
}

#undef GLSL