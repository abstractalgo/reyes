#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include <cstdlib>
#include <ctime>

#define GR 15
#define GRID_TY_T
//#define GRID_TY_Q

namespace reyes
{
    /* Enum used to determine split direction. */
    enum SplitDir { U, V, NoSplit };
    SplitDir split_dir(vec2 size, vec2 threshold)
    {
        if (size.x <= threshold.x && size.y <= threshold.y)
            return SplitDir::NoSplit;
        return ((size.x > size.y)
            ? SplitDir::V
            : SplitDir::U);
    }

    struct SurfaceI
    {
        float start_u, end_u, start_v, end_v;
        mx4 transform;
        Microgrid* grid;

        SurfaceI()
            : start_u(0)
            , end_u(1)
            , start_v(0)
            , end_v(1)
#ifdef GRID_TY_T
            , grid(new Microgrid(MicrogridType::TRIANGLE, (GR+1)*(GR+1), GR*GR*2*3))
#endif
#ifdef GRID_TY_Q
            , grid(new Microgrid(MicrogridType::QUAD, (GR+1)*(GR+1), GR*GR**4))
#endif
        {}

        ~SurfaceI()
        {
            delete grid;
        }

        void splitUV(SplitDir direction, SurfaceI& one, SurfaceI& two)
        {
            if (U == direction)
            {
                one.start_u = start_u;
                one.end_u = end_u;
                one.start_v = start_v;
                one.end_v = (start_v + end_v)*0.5f;

                two.start_u = start_u;
                two.end_u = end_u;
                two.start_v = (start_v + end_v)*0.5f;
                two.end_v = end_v;
            }
            else
            {
                one.start_u = start_u;
                one.end_u = (start_u + end_u)*0.5f;
                one.start_v = start_v;
                one.end_v = end_v;

                two.start_u = (start_u + end_u)*0.5f;
                two.end_u = end_u;
                two.start_v = start_v;
                two.end_v = end_v;
            }
        }

        virtual void split(SplitDir direction, Scene& scene) = 0;
        virtual void dice(CameraTransform* camera) = 0;
        virtual void shade(void) = 0;
        virtual position P(uv uv) = 0;
        virtual normal N(uv uv) = 0;
    };

    template<class MaterialTy>
    struct Surface : public SurfaceI
    {
        MaterialTy material;
        void dice(CameraTransform* camera)
        {
            // vertices
            for (uint16_t u = 0; u < GR+1; u++)
            for (uint16_t v = 0; v < GR+1; v++)
            {
                float w = 1.0f/(float)(GR);
                uv _uv = uv(start_u + u*w * (end_u - start_u), start_v + v*w * (end_v - start_v));
                uint16_t idx = v * (GR+1) + u;
                grid->vertices[idx].p = P(_uv);
                grid->vertices[idx].n = N(_uv);
                grid->vertices[idx].uv = _uv;
                grid->vertices[idx].p = material.pShdr(grid->vertices[idx]);
                // TODO transformations: model, view, projection
            }

            // indices
            for (uint16_t v = 0; v < GR; v++)
            for (uint16_t u = 0; u < GR; u++)
            {
                // A: v * 9 + u
                // B: v * 9 + u + 1
                // C: v * 9 + 9 + u + 1
                // D: v * 9 + 9 + u
#ifdef GRID_TY_T
                // ADB
                grid->indices[v * (GR * 6) + u * 6 + 0] = (v * (GR+1)) + (u);
                grid->indices[v * (GR * 6) + u * 6 + 1] = ((v + 1) * (GR + 1)) + (u);
                grid->indices[v * (GR * 6) + u * 6 + 2] = (v * (GR + 1)) + (u + 1);

                // BDC
                grid->indices[v * (GR * 6) + u * 6 + 3] = (v * (GR + 1)) + (u + 1);
                grid->indices[v * (GR * 6) + u * 6 + 4] = ((v + 1) * (GR + 1)) + (u);
                grid->indices[v * (GR * 6) + u * 6 + 5] = ((v + 1) * (GR + 1)) + (u + 1);
#endif
#ifdef GRID_TY_Q
                grid->indices[v * (GR*4) + u * 4 + 0] = (v * (GR+1)) + (u);
                grid->indices[v * (GR*4) + u * 4 + 1] = (v * (GR+1)) + (u + 1);
                grid->indices[v * (GR*4) + u * 4 + 2] = ((v + 1) * (GR+1)) + (u + 1);
                grid->indices[v * (GR*4) + u * 4 + 3] = ((v + 1) * (GR+1)) + (u);
#endif
            }
        }

        void shade(void)
        {
            color rnd = { rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1 };
            for (uint16_t i = 0; i < (GR + 1)*((GR + 1)); i++)
                grid->vertices[i].c = rnd;// material.cShdr(grid->vertices[i]);
        }

        void renderOGL(void)
        {
#ifdef GRID_TY_T
            glColor3f(1, 1, 1);
            glBegin(GL_TRIANGLES);
            for (uint16_t i = 0; i < grid->primCount(); i++)
            {
                Vertex a = grid->vertices[grid->indices[3 * i + 0]];
                Vertex b = grid->vertices[grid->indices[3 * i + 1]];
                Vertex c = grid->vertices[grid->indices[3 * i + 2]];
                glVertex3f(a.p.x, a.p.y, a.p.z);
                glVertex3f(b.p.x, b.p.y, b.p.z);
                glVertex3f(c.p.x, c.p.y, c.p.z);
            }
            glEnd();

            glColor3f(1, 0, 0);
            glBegin(GL_LINES);
            for (uint16_t i = 0; i < grid->primCount(); i++)
            {
                Vertex a = grid->vertices[grid->indices[3 * i + 0]];
                Vertex b = grid->vertices[grid->indices[3 * i + 1]];
                Vertex c = grid->vertices[grid->indices[3 * i + 2]];
                glVertex3f(a.p.x, a.p.y, a.p.z);
                glVertex3f(b.p.x, b.p.y, b.p.z);

                glVertex3f(b.p.x, b.p.y, b.p.z);
                glVertex3f(c.p.x, c.p.y, c.p.z);

                glVertex3f(c.p.x, c.p.y, c.p.z);
                glVertex3f(a.p.x, a.p.y, a.p.z);
            }
            glEnd();
#endif
#ifdef GRID_TY_Q
            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            for (uint16_t i = 0; i < grid->primCount(); i++)
            {
                Vertex a = grid->vertices[grid->indices[4 * i + 0]];
                Vertex b = grid->vertices[grid->indices[4 * i + 1]];
                Vertex c = grid->vertices[grid->indices[4 * i + 2]];
                Vertex d = grid->vertices[grid->indices[4 * i + 3]];
                glVertex3f(a.p.x, a.p.y, a.p.z);
                glVertex3f(b.p.x, b.p.y, b.p.z);
                glVertex3f(c.p.x, c.p.y, c.p.z);
                glVertex3f(d.p.x, d.p.y, d.p.z);
            }
            glEnd();

            glColor3f(1, 0, 0);
            glBegin(GL_LINES);
            for (uint16_t i = 0; i < grid->primCount(); i++)
            {
                Vertex a = grid->vertices[grid->indices[4 * i + 0]];
                Vertex b = grid->vertices[grid->indices[4 * i + 1]];
                Vertex c = grid->vertices[grid->indices[4 * i + 2]];
                Vertex d = grid->vertices[grid->indices[4 * i + 3]];
                glVertex3f(a.p.x, a.p.y, a.p.z);
                glVertex3f(b.p.x, b.p.y, b.p.z);

                glVertex3f(b.p.x, b.p.y, b.p.z);
                glVertex3f(c.p.x, c.p.y, c.p.z);

                glVertex3f(c.p.x, c.p.y, c.p.z);
                glVertex3f(d.p.x, d.p.y, d.p.z);

                glVertex3f(d.p.x, d.p.y, d.p.z);
                glVertex3f(a.p.x, a.p.y, a.p.z);
            }
            glEnd();
#endif
        }
    };
}