#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "grid.hpp"
#include "matlib.hpp"

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

    struct ShapeI
    {
        virtual void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack) = 0;
        virtual GridI* dice(mem::ObjectStack<GridI>& dicedGrids) = 0;
        virtual GridI* shade(mem::ObjectStack<GridVertexTy<color>>& shadedGrids) = 0;
    };

    template<class VertexTy, class MaterialTy>
    /* Abstract class for shapes. */
    struct Shape : public ShapeI
    {
        mx4 transform;
        MaterialTy material;
    };

    template<class VertexTy, class MaterialTy>
    /* Base class for all the parametric (UV) surfaces. */
    struct ParametricSurface : public Shape<VertexTy, MaterialTy>
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

    template<class VertexTy, class MaterialTy>
    /* Rectangular shape inherited from UVSurface */
    struct Rectangle : public ParametricSurface<VertexTy, MaterialTy>
    {
        position center;
        vec2 size;

        Rectangle(position _center = { 0, 0, 0 }, vec2 _size = { 0, 0 })
            : center(_center)
            , size(_size)
        {}

        void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack)
        {
            Rectangle& a = *(Rectangle*)stack.alloc(sizeof(Rectangle));
            Rectangle& b = *(Rectangle*)stack.alloc(sizeof(Rectangle));
            a.center = b.center = center;
            a.size = b.size = size;
            uv_split(direction, a, b);
        }

        GridI* dice(mem::ObjectStack<GridI>& dicedGrids)
        {
            // TODO: fill VertexTy data and get ready for shading

            // grid
            QuadGrid<VertexTy, 4, 4, 1>& grid = *new(dicedGrids.alloc(sizeof(QuadGrid<VertexTy, 4, 4, 1>))) QuadGrid<VertexTy, 4, 4, 1>;
            // vertices

            // indices
            grid.indices[0] = 0;
            grid.indices[1] = 1;
            grid.indices[2] = 2;
            grid.indices[3] = 3;

            return &grid;
        }

        GridI* shade(mem::ObjectStack<GridVertexTy<color>>& shadedGrids)
        {
            QuadGrid<VertexTy, 4, 4, 1> grid;
            

            QuadGrid<color, 4, 4, 1>& color_grid = *new(shadedGrids.alloc(sizeof(QuadGrid<color, 4, 4, 1>))) QuadGrid<color, 4, 4, 1>;
            // data
            for (uint8_t i = 0; i < 4; i++)
                color_grid.data[i] = material.cShdr(grid.data[i]);
            // indices
            memcpy(color_grid.indices, grid.indices, sizeof(grid.indices));
            return &color_grid;
        }
    };

    // TODO
    //template<class VertexTy, class MaterialTy>
    ///* Sphere shape inherited from UVSurface */
    //struct Sphere : public ParametricSurface<VertexTy, MaterialTy>
    //{
        
    //};

    //template<class VertexTy, class MaterialTy>
    ///* QUadrilateral shape inherited from UVSurface */
    //struct Quadriliteral : public ParametricSurface<VertexTy, MaterialTy>
    //{

    //};

    // TODO template grid size
}