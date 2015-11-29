#pragma once

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

        virtual void split(SplitDir direction, Shape** child_a, Shape** child_b) {}
        virtual void dice(Grid<PosNormalMat>& grid) {}

        virtual vec3 N(float u, float v) { return{ 0, 0, 0 }; }
        virtual vec3 P(float u, float v) { return{ 0, 0, 0 }; }
    };

    /* Base class for all the parametric (UV) surfaces. */
    struct ParametricSurface : public Shape
    {
        float start_u, start_v, end_u, end_v;
    };
    typedef ParametricSurface UVSurface;

    /* Rectangular shape inherited from UVSurface */
    struct Rectangle : public ParametricSurface
    {
        position center;
        vec2 size;

        Rectangle(position _center = { 0, 0, 0 }, vec2 _size = { 0, 0 })
            : center(_center)
            , size(_size)
        {}

        void split(SplitDir direction, Shape** child_a, Shape** child_b)
        {
            Rectangle* halves = new Rectangle[2];
            halves[0].center = center;
            halves[0].size = size;
            halves[1].center = center;
            halves[1].size = size;
            if (U == direction)
            {
                halves[0].start_u = start_u;
                halves[0].end_u = end_u;
                halves[1].start_u = start_u;
                halves[1].end_u = end_u;

                halves[0].start_v = start_v;
                halves[0].end_v = start_v + (end_v - start_v) / 2.0;
                halves[1].start_v = start_v + (end_v - start_v) / 2.0;
                halves[1].end_v = end_v;
            }
            else if (V == direction)
            {
                halves[0].start_u = start_u;
                halves[0].end_u = start_u + (end_u - start_u) / 2.0;
                halves[1].start_u = start_u + (end_u - start_u) / 2.0;
                halves[1].end_u = end_u;

                halves[0].start_v = start_v;
                halves[0].end_v = end_v;
                halves[1].start_v = start_v;
                halves[1].end_v = end_v;
            }
            *child_a = halves+0;
            *child_b = halves + 1;
        }

        void dice(Grid<PosNormalMat>& grid)
        {

        }
    };
}