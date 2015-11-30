#pragma once

#include "vecmx.hpp"

namespace reyes
{
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

    typedef position Pos;
    typedef normal Normal;

    struct PosNormal
    {
        position p;
        normal n;
    };
    struct PosColor
    {
        position p;
        color col;
    };
    struct PosNormalColor
    {
        position p;
        normal n;
        color col;
    };
}