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
        virtual GridI* dice() = 0;
        virtual GridI* shade(void) = 0;
    };

    template<class VertexTy, class MaterialTy>
    /* Abstract class for shapes. */
    struct Shape : public ShapeI
    {
        mx4 transform;
        MaterialTy material;

        virtual void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack) = 0;
        virtual GridI* dice() = 0;
        virtual GridI* shade(void) = 0;
    };

    template<class VertexTy, class MaterialTy>
    /* Base class for all the parametric (UV) surfaces. */
    struct ParametricSurface : public Shape<VertexTy, MaterialTy>
    {
        float start_u, start_v, end_u, end_v;
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

        virtual void split(SplitDir direction, mem::ObjectStack<ShapeI>& stack) = 0;
        virtual GridI* dice() = 0;
        virtual GridI* shade(void) = 0;
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
            Rectangle& child_a = *(Rectangle<VertexTy, MaterialTy>*)stack.alloc(sizeof(Rectangle<VertexTy, MaterialTy>));
            Rectangle& child_b = *(Rectangle<VertexTy, MaterialTy>*)stack.alloc(sizeof(Rectangle<VertexTy, MaterialTy>));
            child_a.center = center;
            child_a.size = size;
            child_b.center = center;
            child_b.size = size;
            uv_split(direction, child_a, child_b);
        }

        GridI* dice()
        {
            return 0;
            //QuadGrid<Pos, 4, 4, 1> grid;

        }

        GridI* shade(void)
        {
            QuadGrid<color, 4, 4, 1>& color_grid = *new QuadGrid<color, 4, 4, 1>;
            QuadGrid<VertexTy, 4, 4, 1> grid;
            this->material.pShdr(grid.data[0]);
            return 0;
            //;
        }
    };
}