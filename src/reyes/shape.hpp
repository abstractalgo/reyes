#pragma once

#include "vecmx.hpp"
#include "grid.hpp"

namespace reyes
{
    /* Enum used to determine split direction of the Shape. */
    enum SplitDir { U, V };

    /* Abstract class for shapes. */
    class Shape
    {
    public:
        virtual void split(SplitDir direction, Shape* child_a, Shape* child_b) {}
        virtual void dice(Grid<PosNormalMat>* uGrid) {}

        virtual vec3 N(float u, float v) { return{ 0, 0, 0 }; }
        virtual vec3 P(float u, float v) { return{ 0, 0, 0 }; }
    };

    class Rectangle : public Shape
    {
    public:
        position center;
        vec2 size;

        Rectangle(position _center, vec2 _size)
            : center(_center)
            , size(_size)
        {}
    };
}