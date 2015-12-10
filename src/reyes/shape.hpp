#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"

namespace reyes
{
    struct Scene
    {
        mem::Stack<mem::blk, 1024> allocations;
        mem::mAllocator memory;

        mem::blk alloc(size_t size)
        {
            mem::blk block = memory.alloc(size);
            allocations.push(block);
            return block;
        }

        mem::blk pop()
        {
            return allocations.pop();
        }

        void free(mem::blk block)
        {
            memory.free(block);
        }

        operator bool()
        {
            return allocations.top > 0;
        }
    };

    /* Enum used to determine split direction of the Shape. U - vertical cut, V - hozirontal cut*/
    enum SplitDir { U, V, NoSplit };
    SplitDir split_dir(vec2 size, vec2 threshold)
    {
        if (size.x <= threshold.x && size.y <= threshold.y)
            return SplitDir::NoSplit;
        return ((size.x > size.y)
            ? SplitDir::V
            : SplitDir::U);
    }

    /* Abstract class for shapes. */
    struct ShapeI
    {
        mx4 transform;
        virtual void split(SplitDir direction, Scene& scene) = 0;
        virtual mem::blk dice(mem::AllocatorI* alloc) = 0;
        virtual mem::blk shade(MicrogridI<PosNormalUV>* dgrid, mem::AllocatorI* alloc) = 0;
        virtual position P(uint16_t idx) = 0;
        virtual normal N(uint16_t idx) = 0;
        virtual uv UV(uint16_t idx) = 0;
    };

    template<class MaterialTy>
    /* Material-typed abstract class for shapes. */
    struct Shape : public ShapeI
    {
        MaterialTy material;
    };

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
        }
        mem::blk dice(mem::AllocatorI* alloc)
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
            }
            // indices
            grid.indices[0] = 0;
            grid.indices[1] = 1;
            grid.indices[2] = 3;
            grid.indices[3] = 2;

            // transform grid
            grid.transform(transform);

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

//template<class MaterialTy>
///* Base class for all the parametric (UV) surfaces. */
//struct ParametricSurface : public Shape<MaterialTy>
//{
//    float start_u, start_v, end_u, end_v;
//    ParametricSurface(float su = 0.0f, float sv = 0.0f, float eu = 1.0f, float ev = 1.0f)
//        : start_u(su)
//        , start_v(sv)
//        , end_u(eu)
//        , end_v(ev)
//    {}
//    void uv_split(SplitDir direction, ParametricSurface& a, ParametricSurface& b)
//    {
//        if (U == direction)
//        {
//            a.start_u = start_u;
//            a.end_u = end_u;
//            b.start_u = start_u;
//            b.end_u = end_u;
//
//            a.start_v = start_v;
//            a.end_v = start_v + (end_v - start_v) / 2.0f;
//            b.start_v = start_v + (end_v - start_v) / 2.0f;
//            b.end_v = end_v;
//        }
//        else if (V == direction)
//        {
//            a.start_u = start_u;
//            a.end_u = start_u + (end_u - start_u) / 2.0f;
//            b.start_u = start_u + (end_u - start_u) / 2.0f;
//            b.end_u = end_u;
//
//            a.start_v = start_v;
//            a.end_v = end_v;
//            b.start_v = start_v;
//            b.end_v = end_v;
//        }
//    }
//};

//template<class MaterialTy>
///* Rectangular shape inherited from UVSurface */
//struct Rectangle : public ParametricSurface<MaterialTy>
//{
//    position center;
//    vec2 size;

//    Rectangle(position _center = { 0, 0, 0 }, vec2 _size = { 0, 0 })
//        : center(_center)
//        , size(_size)
//    {}

//    void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack)
//    {
//        Rectangle& a = *(Rectangle*)stack.alloc(sizeof(Rectangle));
//        Rectangle& b = *(Rectangle*)stack.alloc(sizeof(Rectangle));
//        a.center = b.center = center;
//        a.size = b.size = size;
//        uv_split(direction, a, b);
//    }

//    GridI* dice(mem::ObjectStack<GridI>& dicedGrids)
//    {
//        // TODO: fill VertexTy data and get ready for shading

//        // grid
//        QuadGrid<PosNormalUV, 4, 4, 1>& grid = *new(dicedGrids.alloc(sizeof(QuadGrid<PosNormalUV, 4, 4, 1>))) QuadGrid<PosNormalUV, 4, 4, 1>;
//        // vertices

//        // indices
//        grid.indices[0] = 0;
//        grid.indices[1] = 1;
//        grid.indices[2] = 2;
//        grid.indices[3] = 3;

//        return &grid;
//    }

//    GridI* shade(mem::ObjectStack<GridVertexTy<color>>& shadedGrids)
//    {
//        QuadGrid<PosNormalUV, 4, 4, 1> grid;
//        

//        QuadGrid<color, 4, 4, 1>& color_grid = *new(shadedGrids.alloc(sizeof(QuadGrid<color, 4, 4, 1>))) QuadGrid<color, 4, 4, 1>;
//        // data
//        for (uint8_t i = 0; i < 4; i++)
//            color_grid.data[i] = material.cShdr(grid.data[i]);
//        // indices
//        memcpy(color_grid.indices, grid.indices, sizeof(grid.indices));
//        return &color_grid;
//    }

//    position P(uint16_t idx) { return{ 0, 0, 1 }; }
//    norm
//};

// TODO
//template<class MaterialTy>
///* Sphere shape inherited from UVSurface */
//struct Sphere : public ParametricSurface<MaterialTy>
//{
//    position center;
//    float R;

//    Sphere(position _c = { 0, 0, 0 }, float _R = 1.0f)
//        : center(_c)
//        , R(_R)
//    {}

//    void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack)
//    {
//        Sphere& a = *(Sphere*)stack.alloc(sizeof(Sphere));
//        Sphere& b = *(Sphere*)stack.alloc(sizeof(Sphere));
//        a.center = b.center = center;
//        a.R = b.R = R;
//        uv_split(direction, a, b);
//    }

//    GridVertexTy<PosNormalUV>* dice(mem::ObjectStack<GridVertexTy<PosNormalUV>>& dicedGrids)
//    {
//        // grid
//        TriGrid<PosNormalUV, 3, 3, 1>& grid = *new(dicedGrids.alloc(sizeof(TriGrid<PosNormalUV, 3, 3, 1>))) TriGrid<PosNormalUV, 3, 3, 1>;
//        // vertices
//        for (uint16_t idx = 0; idx < 3; idx++)
//        {
//            grid.data[idx].p = P(idx);
//            grid.data[idx].n = N(idx);
//            grid.data[idx].uv = UV(idx);
//            grid.data[idx].p = material.pShdr(grid.data[idx]);
//        }
//        // indices
//        grid.indices[0] = 0;
//        grid.indices[1] = 1;
//        grid.indices[2] = 2;

//        return &grid;
//    }

//    GridVertexTy<color>* shade(mem::ObjectStack<GridVertexTy<color>>& shadedGrids)
//    {
//        // dice grid
//        TriGrid<PosNormalUV, 3, 3, 1> grid;
//        // vertices
//        for (uint16_t idx = 0; idx < 3; idx++)
//        {
//            grid.data[idx].p = P(idx);
//            grid.data[idx].n = N(idx);
//            grid.data[idx].uv = UV(idx);
//            grid.data[idx].p = material.pShdr(grid.data[idx]);
//        }
//        // indices
//        grid.indices[0] = 0;
//        grid.indices[1] = 1;
//        grid.indices[2] = 2;

//        // color grid
//        TriGrid<color, 3, 3, 1>& color_grid = *new(shadedGrids.alloc(sizeof(TriGrid<color, 3, 3, 1>))) TriGrid<color, 3, 3, 1>;
//        // data
//        for (uint8_t i = 0; i < 3; i++)
//            color_grid.data[i] = material.cShdr(grid.data[i]);
//        // indices
//        memcpy(color_grid.indices, grid.indices, sizeof(grid.indices));
//        return &color_grid;
//    }

//    position P(uint16_t idx) { return{ 0, 0, 1 }; }
//    normal N(uint16_t idx) { return{ 0, 0, 1 }; }
//    uv UV(uint16_t idx) { return{ 0, 0 }; }
//};