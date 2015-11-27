#pragma once

#include "vecmx.hpp"

namespace reyes
{
    /* Holding microgrid data. */
    class Microgrid
    {

    };

    /* Enum used to determine split direction of the Shape. */
    enum SplitDir { U, V };

    /* Abstract class for shapes. */
    class ShapeI
    {
    public:
        virtual void split(SplitDir direction, ShapeI* child_a, ShapeI* child_b) {}
        virtual void dice(Microgrid* uGrid) {}

        virtual math::vec3 N(float u, float v) { return{ 0, 0, 0 }; }
        virtual math::vec3 P(float u, float v) { return{ 0, 0, 0 }; }
    };

    class Rectangle : public ShapeI
    {
    public:
        math::vec3 center;
        math::vec2 size;

        Rectangle(math::vec3 _center, math::vec2 _size)
            : center(_center)
            , size(_size)
        {}

        void split(SplitDir direction, ShapeI* child_a, ShapeI* child_b) {}
    };
}