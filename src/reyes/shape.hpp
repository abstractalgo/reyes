#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "grid.hpp"
#include "misc.hpp"

namespace reyes
{
    /* Enum used to determine split direction of the Shape. */
    enum SplitDir { U, V };
    SplitDir split_dir(vec2 size)
    {
        return ((size.x > size.y)
            ? SplitDir::U
            : SplitDir::V);
    }

    /* Abstract class for shapes. */
    struct ShapeI
    {
        virtual void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack) = 0;
        virtual GridVertexTy<PosNormalUV>* dice(mem::ObjectStack<GridVertexTy<PosNormalUV>>& dicedGrids) = 0;
        virtual GridVertexTy<color>* shade(mem::ObjectStack<GridVertexTy<color>>& shadedGrids) = 0;
        virtual position P(uint16_t idx) = 0;
        virtual normal N(uint16_t idx) = 0;
        virtual uv UV(uint16_t idx) = 0;
    };

    template<class MaterialTy>
    /* Material-typed abstract class for shapes. */
    struct Shape : public ShapeI
    {
        mx4 transform;
        MaterialTy material;
    };

    template<class MaterialTy>
    /* Base class for all the parametric (UV) surfaces. */
    struct ParametricSurface : public Shape<MaterialTy>
    {
        float start_u, start_v, end_u, end_v;
        ParametricSurface(float su = 0.0f, float sv = 0.0f, float eu = 1.0f, float ev = 1.0f)
            : start_u(su)
            , start_v(sv)
            , end_u(eu)
            , end_v(ev)
        {}
        void uv_split(SplitDir direction, ParametricSurface& a, ParametricSurface& b)
        {
            if (U == direction)
            {
                a.start_u = start_u;
                a.end_u = end_u;
                b.start_u = start_u;
                b.end_u = end_u;

                a.start_v = start_v;
                a.end_v = start_v + (end_v - start_v) / 2.0f;
                b.start_v = start_v + (end_v - start_v) / 2.0f;
                b.end_v = end_v;
            }
            else if (V == direction)
            {
                a.start_u = start_u;
                a.end_u = start_u + (end_u - start_u) / 2.0f;
                b.start_u = start_u + (end_u - start_u) / 2.0f;
                b.end_u = end_u;

                a.start_v = start_v;
                a.end_v = end_v;
                b.start_v = start_v;
                b.end_v = end_v;
            }
        }
    };

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

    template<class MaterialTy>
    /* QUadrilateral shape inherited from UVSurface */
    struct Quadriliteral : public ParametricSurface<MaterialTy>
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

        Quadriliteral(position _a = { -1, 1, 0 }, position _b = { 1, 1, 0 }, position _c = { 1, -1, 0 }, position _d = { -1, -1, 0 })
            : a(_a)
            , b(_b)
            , c(_c)
            , d(_d)
        {}

        void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack)
        {
            Quadriliteral& one = *(Quadriliteral*)stack.alloc(sizeof(Quadriliteral));
            Quadriliteral& two = *(Quadriliteral*)stack.alloc(sizeof(Quadriliteral));
            if (U == direction)
            {
                one.a = a; one.b = b; one.c = 0.5f*(b + c); one.d = 0.5f*(a + d);
                two.a = 0.5f*(a + d); two.b = 0.5f*(b + c); two.c = c; two.d = d;
            }
            else
            {
                one.a = a; one.b = 0.5f*(a + b); one.c = 0.5f*(c + d); one.d = d;
                two.a = 0.5f*(a + b); two.b = b; two.c = c; two.d = 0.5f*(c + d);
            }
            uv_split(direction, one, two);
        }

        GridVertexTy<PosNormalUV>* dice(mem::ObjectStack<GridVertexTy<PosNormalUV>>& dicedGrids)
        {
            // grid
            QuadGrid<PosNormalUV, 4, 4>& grid = *new(dicedGrids.alloc(sizeof(QuadGrid<PosNormalUV, 4, 4>))) QuadGrid<PosNormalUV, 4, 4>;
            // vertices
            for (uint16_t idx = 0; idx < 4; idx++)
            {
                grid.data[idx].p = P(idx);
                grid.data[idx].n = N(idx);
                grid.data[idx].uv = UV(idx);
                //grid.data[idx].p = material.pShdr(grid.data[idx]);
            }
            // indices
            grid.indices[0] = 0;
            grid.indices[1] = 1;
            grid.indices[2] = 2;
            grid.indices[3] = 2;

            return &grid;
        }

        GridVertexTy<color>* shade(mem::ObjectStack<GridVertexTy<color>>& shadedGrids)
        {
            // dice
            QuadGrid<PosNormalUV, 4, 4>& grid = *new(dicedGrids.alloc(sizeof(QuadGrid<PosNormalUV, 4, 4>))) QuadGrid<PosNormalUV, 4, 4>;
            // vertices
            for (uint16_t idx = 0; idx < 4; idx++)
            {
                grid.data[idx].p = P(idx);
                grid.data[idx].n = N(idx);
                grid.data[idx].uv = UV(idx);
                //grid.data[idx].p = material.pShdr(grid.data[idx]);
            }
            // indices
            grid.indices[0] = 0;
            grid.indices[1] = 1;
            grid.indices[2] = 2;
            grid.indices[3] = 2;

            // color grid
            QuadGrid<color, 4, 4>& color_grid = *new(shadedGrids.alloc(sizeof(QuadGrid<color, 4, 4>))) QuadGrid<color, 4, 4>;
            // data
            for (uint8_t i = 0; i < 4; i++)
                color_grid.data[i] = material.cShdr(grid.data[i]);
            // indices
            memcpy(color_grid.indices, grid.indices, sizeof(grid.indices));
            return &color_grid;
        }

        position P(uint16_t idx) { return{ 0, 0, 1 }; /*diced grid entry*/ }
        normal N(uint16_t idx) { return{ 0, 0, 1 }; }
        uv UV(uint16_t idx) { return{ 0, 0 }; /*calc xy-coord and return normalized uv*/ }
    };

    // TODO template grid size
}