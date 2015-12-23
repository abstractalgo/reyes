#pragma once

#include "mem.hpp"
#include "shading.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include <cstdlib>
#include <ctime>

#define GR 8
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

    struct ShapeI
    {
        float start_u, end_u, start_v, end_v;
        mx4 transform;
        Microgrid* grid;

        ShapeI()
            : start_u(0)
            , end_u(1)
            , start_v(0)
            , end_v(1)
            , grid(0)
        {}

        ~ShapeI()
        {
            //delete grid;
        }

        virtual void split(SplitDir direction, Scene& scene) = 0;
        virtual void dice(CameraTransform* camera, mem::ObjectStack<Microgrid>& grids) = 0;
        virtual void shade(void) = 0;
        virtual position P(uv uv) = 0;
        virtual normal N(uv uv) = 0;
    };

    template<class MaterialTy>
    struct Shape : public ShapeI
    {
        MaterialTy material;
        void dice(CameraTransform* camera, mem::ObjectStack<Microgrid>& grids)
        {
            mem::blk m = grids.alloc(2048); // TODO
#ifdef GRID_TY_T
            grid = ::new(m.ptr) Microgrid(MicrogridType::TRIANGLE, (GR + 1)*(GR + 1), GR*GR * 2 * 3);
#endif
#ifdef GRID_TY_Q
            grid = ::new(m.ptr) Microgrid(MicrogridType::QUAD, (GR + 1)*(GR + 1), GR*GR * 4);
#endif
            float wu = (end_u - start_u) / (float)(GR);
            float wv = (end_v - start_v) / (float)(GR);
            // vertices
            for (uint16_t u = 0; u < GR+1; u++)
            for (uint16_t v = 0; v < GR+1; v++)
            {
                uv _uv(start_u + u*wu , start_v + v*wv );
                uint16_t idx = v * (GR+1) + u;
                grid->vertices[idx].uv = uv(start_u + u*wu, start_v + v*wv);
                grid->vertices[idx].p = P(_uv);
                grid->vertices[idx].n = N(_uv);
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
            {
                grid->vertices[i].c = rnd; // material.cShdr(grid->vertices[i]);
            }
                
        }
    };
}