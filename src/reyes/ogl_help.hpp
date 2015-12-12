#pragma once

#include "backend.hpp"

#define GLSL(version, shader) "#version " #version "\n" #shader

const char* fsquad_vs = GLSL(330,
    in vec2 iPos;
    out vec2 uv;

    void main()
    {
        gl_Position = vec4(iPos, 0.5, 1.0);
        uv = (iPos + vec2(1.0)) * 0.5;
        uv.y = 1.0 - uv.y;
    }
);

const char* fsquad_fs = GLSL(330,
    in vec2 uv;
    uniform sampler2D tex;
    out vec4 frag;

    void main()
    {
        frag = texture(tex, uv);
    }
);

void opengl_init(GLuint tex, GLuint program, GLuint quad, unsigned int w, unsigned int h, void* data)
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    // init texture
    {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w,h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
        glUseProgram(program);
        glUniform1i(glGetUniformLocation(program, "tex"), 0);
}

    // inti and draw quad
    {
        GLfloat verts[12] = { -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1 };
        glGenBuffers(1, &quad);
        glBindBuffer(GL_ARRAY_BUFFER, quad);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glEnableVertexAttribArray(glGetAttribLocation(program, "iPos"));
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void opengl_display(GLuint tex, unsigned int w, unsigned int h, void* data)
{
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    SwapBuffersBackend();
}

void opengl_cleanup(GLuint tex, GLuint program, GLuint quad)
{
    glUseProgram(0);
    glDeleteProgram(program);
    glDeleteBuffers(1, &quad);
    glDeleteTextures(1, &tex);
}
#undef GLSL