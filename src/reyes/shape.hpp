#pragma once

#include "mem.hpp"
#include "vecmx.hpp"
#include "grid.hpp"

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
    struct Shape
    {
        mx4 transform;

        virtual void split(SplitDir direction, mem::ObjectStack<Shape>& stack) {}
        virtual GridI<PosNormalMat>* dice(mem::ObjectStack<GridI<PosNormalMat>>& grids) {}

        virtual vec3 N(float u, float v) { return{ 0, 0, 0 }; }
        virtual vec3 P(float u, float v) { return{ 0, 0, 0 }; }
    };

    /* Base class for all the parametric (UV) surfaces. */
    struct ParametricSurface : public Shape
    {
        float start_u, start_v, end_u, end_v;

        // TODO move part of the split() and dice() here
    };

    /* Rectangular shape inherited from UVSurface */
    struct Rectangle : public ParametricSurface
    {
        position center;
        vec2 size;

        Rectangle(position _center = { 0, 0, 0 }, vec2 _size = { 0, 0 })
            : center(_center)
            , size(_size)
        {}

        void split(SplitDir direction, mem::ObjectStack<Shape>& stack)
        {
            Rectangle& child_a = *(Rectangle*)stack.alloc(sizeof(Rectangle));
            Rectangle& child_b = *(Rectangle*)stack.alloc(sizeof(Rectangle));
            child_a.center = center;
            child_a.size = size;
            child_b.center = center;
            child_b.size = size;
            if (U == direction)
            {
                child_a.start_u = start_u;
                child_a.end_u = end_u;
                child_b.start_u = start_u;
                child_b.end_u = end_u;

                child_a.start_v = start_v;
                child_a.end_v = start_v + (end_v - start_v) / 2.0f;
                child_b.start_v = start_v + (end_v - start_v) / 2.0f;
                child_b.end_v = end_v;
            }
            else if (V == direction)
            {
                child_a.start_u = start_u;
                child_a.end_u = start_u + (end_u - start_u) / 2.0f;
                child_b.start_u = start_u + (end_u - start_u) / 2.0f;
                child_b.end_u = end_u;

                child_a.start_v = start_v;
                child_a.end_v = end_v;
                child_b.start_v = start_v;
                child_b.end_v = end_v;
            }
        }

        
        GridI<PosNormalMat>* dice(mem::ObjectStack<GridI<PosNormalMat>>& grids)
        {
            size_t size = sizeof(TriGrid<PosNormalMat>);
            void* mem = grids.alloc(size);
            TriGrid<PosNormalMat>* grid = new(mem) TriGrid<PosNormalMat>;
            return (GridI<PosNormalMat>*)grid;
        }
    };
}