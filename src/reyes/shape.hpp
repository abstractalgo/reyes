#pragma once

#include "mem.hpp"
#include "shading.hpp"
#include "grid.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include <cstdlib>
#include <ctime>

#define GR 8
//#define GRID_SHADE
#define GRID_TY_Tri

#ifdef GRID_TY_Tri
#define GRID_TY_T
#else
#define GRID_TY_Q
#endif

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
        struct transform_tag
        {
            mx4 model_matrix;
            vec3 T; // translate
            vec3 R; // rotate
            vec3 S; // scale
        } transform;

        Microgrid* grid;

        ShapeI()
            : start_u(0)
            , end_u(1)
            , start_v(0)
            , end_v(1)
            , grid(0)
        {
            transform.S = vec3(1, 1, 1);
            transform.R = vec3(0, 0, 0);
            transform.T = vec3(0, 0, 0);
        }

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
            Vertex vx;
            uint16_t idx;
            uv _uv;
            // vertices
            for (uint16_t u = 0; u < GR+1; u++)
            for (uint16_t v = 0; v < GR + 1; v++)
            {
                _uv = vec2(start_u + u*wu, start_v + v*wv);
                idx = v * (GR + 1) + u;

                // calculate model-world values
                vx.uv = uv(start_u + u*wu, start_v + v*wv);
                vx.p = P(_uv);
                vx.n = N(_uv);
                vx.p = material.pShdr(vx);

                // TODO transformations: model, view, projection

                // transform vertex
                // 1. scale
                vx.p.x *= transform.S.x;
                vx.p.y *= transform.S.y;
                vx.p.z *= transform.S.z;
                // 2. rotate (XYZ order)
                // 1 0  0
                // 0 c -s
                // 0 s  c
                float Rx = transform.R.x;
                vx.p.y = vx.p.y*cos(Rx) + sin(Rx)*vx.p.z;
                vx.p.z = -vx.p.y*sin(Rx) + cos(Rx)*vx.p.z;
                //  c 0 s
                //  0 1 0
                // -s 0 c
                float Ry = transform.R.y;
                vx.p.x = vx.p.x*cos(Ry) - sin(Ry)*vx.p.z;
                vx.p.z = vx.p.x*sin(Ry) + cos(Ry)*vx.p.z;
                // c -s 0
                // s  c 0
                // 0  0 1
                float Rz = transform.R.z;
                vx.p.x = vx.p.x*cos(Rz) + sin(Rz)*vx.p.y;
                vx.p.y = -vx.p.x*sin(Rz) + cos(Rz)*vx.p.y;
                // 3. translate
                vx.p.x += transform.T.x;
                vx.p.y += transform.T.y;
                vx.p.z += transform.T.z;

                // transform normal
                vx.n.y = vx.n.y*cos(Rx) + sin(Rx)*vx.n.z;
                vx.n.z = -vx.n.y*sin(Rx) + cos(Rx)*vx.n.z;
                vx.n.x = vx.n.x*cos(Ry) - sin(Ry)*vx.n.z;
                vx.n.z = vx.n.x*sin(Ry) + cos(Ry)*vx.n.z;
                vx.n.x = vx.n.x*cos(Rz) + sin(Rz)*vx.n.y;
                vx.n.y = -vx.n.x*sin(Rz) + cos(Rz)*vx.n.y;

                vx.n.x *= transform.S.x;
                vx.n.y *= transform.S.y;
                vx.n.z *= transform.S.z;

                vx.n.normalize();

                
                grid->vertices[idx] = vx;
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
                grid->indices[v * (GR * 4) + u * 4 + 0] = (v * (GR + 1)) + (u);
                grid->indices[v * (GR * 4) + u * 4 + 1] = ((v + 1) * (GR + 1)) + (u);
                grid->indices[v * (GR * 4) + u * 4 + 2] = ((v + 1) * (GR + 1)) + (u + 1);
                grid->indices[v * (GR * 4) + u * 4 + 3] = (v * (GR + 1)) + (u + 1);
#endif
            }
        }

        void splitData(Shape<MaterialTy>* shp, char i)
        {
            shp->transform = transform; // TODO copy just matrix
            shp->material.uniform = material.uniform; // TODO gargbage, auto-copy
            if (0 == i)
            {
                shp->start_u = start_u;
                shp->end_u = (start_u + end_u)*0.5f;
                shp->start_v = start_v;
                shp->end_v = (start_v + end_v)*0.5f;
            }
            else if (1 == i)
            {
                shp->start_u = (start_u + end_u)*0.5f;
                shp->end_u = end_u;
                shp->start_v = start_v;
                shp->end_v = (start_v + end_v)*0.5f;
            }
            else if (2 == i)
            {
                shp->start_u = start_u;
                shp->end_u = (start_u + end_u)*0.5f;
                shp->start_v = (start_v + end_v)*0.5f;
                shp->end_v = end_v;
            }
            else if (3 == i)
            {
                shp->start_u = (start_u + end_u)*0.5f;
                shp->end_u = end_u;
                shp->start_v = (start_v + end_v)*0.5f;
                shp->end_v = end_v;
            }
        }

        void shade(void)
        {
            color rnd = { rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1 };
            for (uint16_t i = 0; i < (GR + 1)*((GR + 1)); i++)
            {
#ifdef GRID_SHADE
                grid->vertices[i].c = material.cShdr(grid->vertices[i]);
#else
                grid->vertices[i].c = rnd;
#endif
            }
        }
    };
}