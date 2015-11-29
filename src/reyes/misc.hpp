#pragma once

#include "vecmx.hpp"

namespace reyes
{
    /* Axis-algined boundig box (2D). */
    struct AABB2
    {
        vec2 min, max;
    };

    /* Axis-aligned bounding box (3D). */
    struct AABB3
    {
        vec3 min, max;
    };

    /* RGBA32F color struct. */
    struct color
    {
        float r, g, b, a;
        color(float _r = 0, float _g = 0, float _b = 0, float _a = 0)
            : r(_r)
            , g(_g)
            , b(_b)
            , a(_a)
        {}
    };

    typedef vec3 position;
    typedef vec3 normal;
    typedef uint8_t material;

    struct PosNormalMat
    {
        position position;
        normal normal;
        material material;
    };
    struct PosColor
    {
        position position;
        color color;
    };
    struct PosNormalMatColor
    {
        position position;
        normal normal;
        material material;
        color color;
    };
}