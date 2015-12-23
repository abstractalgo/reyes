#pragma once

#include "vecmx.hpp"
#include <cstdio>

namespace reyes
{
    /* RGBA32F color struct. */
    struct color
    {
        float r, g, b, a;
        color(float _r = 0, float _g = 0, float _b = 0, float _a = 1)
            : r(_r)
            , g(_g)
            , b(_b)
            , a(_a)
        {}
    };

    typedef vec2 uv;

    struct PosNormal
    {
        position p;
        normal n;
        PosNormal(position _p, normal _n)
            : p(_p)
            , n(_n)
        {}
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
    struct PosNormalUV
    {
        position p;
        normal n;
        uv uv;
    };

    struct Vertex
    {
        position p;
        normal n;
        uv uv;
        color c;
    };
    typedef uint16_t Index;
}