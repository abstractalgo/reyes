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
        virtual void split(SplitDir direction, Shape*& child_a, Shape*& child_b) = 0;
        virtual void dice(Grid<PosNormalMat>& grid) = 0;

        virtual vec3 N(float u, float v) { return{ 0, 0, 0 }; }
        virtual vec3 P(float u, float v) { return{ 0, 0, 0 }; }
    };

    struct Rectangle : public Shape
    {
        position center;
        vec2 size;

        Rectangle(position _center = {0,0,0}, vec2 _size = { 0, 0 })
            : center(_center)
            , size(_size)
        {}

        void split(SplitDir direction, Shape*& child_a, Shape*& child_b)
        {
            Rectangle* halves = new Rectangle[2];
            if (U == direction)
            {
                halves[0].size = vec2(size.x, size.y / 2);
                //halves[0].center = center + vec2()
                halves[1].size = vec2(size.x, size.y / 2);
            }
            child_a = halves + 0;
            child_b = halves + 1;
        }

        void dice(Grid<PosNormalMat>& grid)
        {

        }
    };
}