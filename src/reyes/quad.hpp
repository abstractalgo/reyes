#include "shape.hpp"

namespace reyes
{
    template<class MaterialTy>
    /* Quadrilateral shape. */
    struct Quadrilateral : public Shape<MaterialTy>
    {
        /*

        A         B
        #---------#
        |          \
        |           \
        #------------#
        D           C

        */
        position a, b, c, d;

        Quadrilateral(position _a = { -1, 1, 0 }, position _b = { 1, 1, 0 }, position _c = { 1, -1, 0 }, position _d = { -1, -1, 0 })
            : a(_a)
            , b(_b)
            , c(_c)
            , d(_d)
        {}

        void split(SplitDir direction, Scene& scene)
        {
            mem::blk one_blk = scene.alloc(sizeof(Quadrilateral<MaterialTy>));
            mem::blk two_blk = scene.alloc(sizeof(Quadrilateral<MaterialTy>));
            Quadrilateral<MaterialTy>& one = *(::new(one_blk.ptr) Quadrilateral<MaterialTy>);
            Quadrilateral<MaterialTy>& two = *(::new(two_blk.ptr) Quadrilateral<MaterialTy>);

            if (U == direction)
            {
                two.a = (a + d)*0.5f;
                two.b = (b + c)*0.5f;
                two.c = c;
                two.d = d;

                one.a = a;
                one.b = b;
                one.c = (b + c)*0.5f;
                one.d = (a + d)*0.5f;
            }
            else
            {
                two.a = a;
                two.b = (a + b)*0.5f;
                two.c = (c + d)*0.5f;
                two.d = d;

                one.a = (a + b)*0.5f;
                one.b = b;
                one.c = c;
                one.d = (c + d)*0.5f;
            }
            // copy transform
        }
        mem::blk dice(CameraTransform* camera, mem::AllocatorI* alloc)
        {
            // grid
            mem::blk grid_blk = alloc->alloc(sizeof(GQuadGrid<4, 4>));
            GQuadGrid<4, 4>& grid = *(::new(grid_blk.ptr) GQuadGrid<4, 4>);
            // vertices
            for (uint16_t idx = 0; idx < 4; idx++)
            {
                grid.data[idx].p = P(idx);
                grid.data[idx].n = N(idx);
                grid.data[idx].uv = UV(idx);
                grid.data[idx].p = material.pShdr(grid.data[idx]);
                // TODO transformations: model, view, projection
            }
            // indices
            grid.indices[0] = 0;
            grid.indices[1] = 1;
            grid.indices[2] = 3;
            grid.indices[3] = 2;

            // transform grid
            //grid.transform(transform);

            return grid_blk;
        }

        mem::blk shade(MicrogridI<PosNormalUV>* dgrid, mem::AllocatorI* alloc)
        {
            GQuadGrid<4, 4>& grid = *(GQuadGrid<4, 4>*)dgrid;

            // color grid
            mem::blk colg_blk = alloc->alloc(sizeof(SQuadGrid<4, 4>));
            SQuadGrid<4, 4>& color_grid = *(::new(colg_blk.ptr) SQuadGrid<4, 4>);
            
            // data
            for (uint16_t i = 0; i < 4; i++)
            {
                color_grid.data[i].col  = material.cShdr(grid.data[i]);
                color_grid.data[i].p    = grid.data[i].p;
            }
            // indices
            color_grid.indices[0] = 0;
            color_grid.indices[1] = 1;
            color_grid.indices[2] = 3;
            color_grid.indices[3] = 2;
            return colg_blk;
        }

        position P(uint16_t idx) {
            
            uv uv;
            uint16_t w = 2, h = 2;
            uv.x = (float)(idx%w)/(w-1);
            uv.y = (float)(idx/2)/(h-1);
            return ((a*(1.0f - uv.x) + b*uv.x)*(1.0f - uv.y) + (d*(1.0f - uv.x) + c*uv.x)*uv.y)*0.5f;
        }
        normal N(uint16_t idx)
        {
            // TODO
            vec3 q = c-a;
            vec3 w = b-d;
            return vec3(q.y*w.z - q.z*w.y, q.z*w.x - q.x*w.z, q.x*w.y - q.y*w.x).normalize();
        }
        uv UV(uint16_t idx)
        {
            uv uv;
            uv.x = (float)(idx % 2);
            uv.y = (float)(idx / 2);
            return uv;
        }
    };
}