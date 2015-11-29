#pragma once

#include "material.hpp"
#include "vecmx.hpp"

namespace reyes
{
    struct PosNormal
    {
        position p;
        normal n;
    };
    struct PosNormalMat
    {
        position p;
        normal n;
        uint8_t mat;

        inline operator position() const { return p; }
        //inline operator PosColor() const { return{ p, { 0, 0, 0, 1 } }; }
    };
    struct PosColor
    {
        position p;
        color col;

        inline operator position() const { return p; }
        inline operator color() const { return col; }
        PosColor& operator=(PosNormalMat& v) { this->p = v.p; this->col = { 0, 0, 0, 1 }; }
    };
    
    
    /*struct PosNormalMatColor
    {
        position position;
        normal normal;
        material material;
        color color;
    };*/
}