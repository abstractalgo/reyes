#pragma once

#include <cstdint>
#include "misc.hpp"

#define CROSS(a,b) vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x)
#define DOT(a,b) a.x*b.x+a.y*b.y+a.z*b.z

namespace reyes
{
    struct Primitive
    {};

    /* Geometrical primitive. Has position, normal and UV. */
    struct GPrimitiveI : public Primitive
    {
        virtual AABB2 aabb(void) const = 0;
    };

    /* Shaded primitive. Has (projected) position and color attributes. */
    struct SPrimitiveI : public Primitive
    {
        virtual bool in(vec3 p) const = 0;
        virtual color at(vec3 p) const = 0;
        inline bool on_side(vec3 test, vec3 v1, vec3 v2) const
        {
            return ((v2.x - v1.x)*(test.y - v1.y) - (v2.y - v1.y)*(test.x - v1.x)) >= 0.0;
        }
    };

    // --- triangle ------------------------------------------------------------

    struct GTriangle : public GPrimitiveI
    {
        PosNormalUV a, b, c;
        AABB2 aabb(void) const
        {
            AABB2 bb;
            bb.min = vec2(fminf(a.p.x, fminf(b.p.x, c.p.x)), fminf(a.p.y, fminf(b.p.y, c.p.y)));
            bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, c.p.x)), fmaxf(a.p.y, fmaxf(b.p.y, c.p.y)));
            return bb;
        }
    };

    struct STriangle : public SPrimitiveI
    {
        PosColor a, b, c;
        bool in(vec3 p) const
        {
            return on_side(p, a.p, b.p) && on_side(p, b.p, c.p) && on_side(p, c.p, a.p);
        }
        /* https://www.shadertoy.com/view/4d3GDH */
        color at(vec3 p) const
        {
            vec3 bxc = CROSS(b.p, c.p);
            vec3 axb = CROSS(a.p, b.p);
            vec3 cxa = CROSS(c.p, a.p);
            float denom = DOT(a.p, bxc);

            float alpha = DOT(p, bxc) / denom;
            float beta = DOT(p, cxa) / denom;
            float gamma = DOT(p, axb) / denom;

            return color(
                a.col.r*alpha + b.col.r*beta + c.col.r*gamma,   // R
                a.col.g*alpha + b.col.g*beta + c.col.g*gamma,   // G
                a.col.b*alpha + b.col.b*beta + c.col.b*gamma,   // B
                a.col.a*alpha + b.col.a*beta + c.col.a*gamma    // A
            );
        }
    };

    // --- quad ----------------------------------------------------------------

    struct GQuad : public GPrimitiveI
    {
        PosNormalUV a, b, c, d;
        AABB2 aabb(void) const
        {
            AABB2 bb;
            bb.min = vec2(fminf(a.p.x, fminf(b.p.x, fminf(c.p.x, d.p.x))), fminf(a.p.y, fminf(b.p.y, fminf(c.p.y, d.p.y))));
            bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, fmaxf(c.p.x, d.p.x))), fmaxf(a.p.y, fmaxf(b.p.y, fmaxf(c.p.y, d.p.y))));
            return bb;
        }
    };

    struct SQuad : public SPrimitiveI
    {
        PosColor a, b, c, d;
        bool in(vec3 p) const
        {
            bool ab = !on_side(p, a.p, b.p);
            bool bc = !on_side(p, b.p, c.p);
            bool cd = !on_side(p, c.p, d.p);
            bool da = !on_side(p, d.p, a.p);
            return ab&&bc&&cd&&da;
        }
        /* https://www.shadertoy.com/view/ldt3Wr */
        color at(vec3 p) const
        {
            vec2 uv;
            vec2 _a = { a.p.x, a.p.y };
            vec2 _b = { b.p.x, b.p.y };
            vec2 _c = { c.p.x, c.p.y };
            vec2 _d = { d.p.x, d.p.y };

            vec2 e = _b - _a;
            vec2 f = _d - _a;
            vec2 g = _a - _b + _c - _d;
            vec2 h = vec2(p.x,p.y) -_a;

            float k2 = g.x*f.y - g.y*f.x;
            float k1 = e.x*f.y - e.y*f.x + h.x*g.y - h.y*g.x;
            float k0 = h.x*e.y - h.y*e.x;

            if (k2 == 0)
            {
                uv.x = 1.0f-(-k0 / k1);
                uv.y = 1.0f-(-(h.x*f.y - h.y*f.x) / k1);
            }
            else
            {
                float w = k1*k1 - 4.0f*k0*k2;

                if (!(w < 0.0))
                {
                    w = sqrt(w);

                    float v1 = (-k1 - w) / (2.0f*k2);
                    float v2 = (-k1 + w) / (2.0f*k2);
                    float u1 = (h.x - f.x*v1) / (e.x + g.x*v1);
                    float u2 = (h.x - f.x*v2) / (e.x + g.x*v2);
                    bool  b1 = v1>0.0 && v1<1.0 && u1>0.0 && u1<1.0;
                    bool  b2 = v2>0.0 && v2<1.0 && u2>0.0 && u2 < 1.0;

                    if (b1 && !b2) uv = vec2(u1, v1);
                    if (!b1 &&  b2) uv = vec2(u2, v2);
                }
            }

            return color(uv.x*255.0f, uv.y*255.0f);

            return color(
                (a.col.r*(1.0f - uv.x) + b.col.r*uv.x)*(1.0f - uv.y) + (d.col.r*(1.0f - uv.x) + c.col.r*uv.x)*uv.y,   // R
                (a.col.g*(1.0f - uv.x) + b.col.g*uv.x)*(1.0f - uv.y) + (d.col.g*(1.0f - uv.x) + c.col.g*uv.x)*uv.y,   // G
                (a.col.b*(1.0f - uv.x) + b.col.b*uv.x)*(1.0f - uv.y) + (d.col.b*(1.0f - uv.x) + c.col.b*uv.x)*uv.y,   // B
                (a.col.a*(1.0f - uv.x) + b.col.a*uv.x)*(1.0f - uv.y) + (d.col.a*(1.0f - uv.x) + c.col.a*uv.x)*uv.y    // A
            );
        }
    };
}

#undef CROSS
#undef DOT
