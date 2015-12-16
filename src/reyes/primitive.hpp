#pragma once

#include <cstdint>
#include "misc.hpp"

#define CROSS(a,b) vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x)
#define DOT(a,b) (a.x*b.x+a.y*b.y+a.z*b.z)

namespace reyes
{
    namespace primitive
    {
        struct PrimitiveI
        {
            virtual Vertex interpolate(position p) = 0;
            virtual AABB2 aabb(void) = 0;
            virtual bool in(position p) = 0;
            inline bool on_side(vec3 test, vec3 v1, vec3 v2)
            {
                return ((v2.x - v1.x)*(test.y - v1.y) - (v2.y - v1.y)*(test.x - v1.x)) >= 0.0;
            }
        };
        struct Triangle : public PrimitiveI
        {
            Vertex a, b, c;

            Vertex interpolate(position p)
            {
                Vertex res;
                vec3 _a = { a.p.x, a.p.y, 1.0f };
                vec3 _b = { b.p.x, b.p.y, 1.0f };
                vec3 _c = { c.p.x, c.p.y, 1.0f };
                p.z = 1.0f;
                
                vec3 bxc = CROSS(_b, _c);
                vec3 axb = CROSS(_a, _b);
                vec3 cxa = CROSS(_c, _a);
                float denom = DOT(_a, bxc);
                
                float alpha = DOT(p, bxc) / denom;
                float beta = DOT(p, cxa) / denom;
                float gamma = DOT(p, axb) / denom;
                
                res.c = color(
                    a.c.r*alpha + b.c.r*beta + c.c.r*gamma,   // R
                    a.c.g*alpha + b.c.g*beta + c.c.g*gamma,   // G
                    a.c.b*alpha + b.c.b*beta + c.c.b*gamma,   // B
                    a.c.a*alpha + b.c.a*beta + c.c.a*gamma    // A
                );
                
                res.p = position(
                    a.p.x*alpha + b.p.x*beta + c.p.x*gamma, // X
                    a.p.y*alpha + b.p.y*beta + c.p.y*gamma, // Y
                    a.p.z*alpha + b.p.z*beta + c.p.z*gamma  // Z
                );
                
                return res;
            }

            AABB2 aabb(void)
            {
                AABB2 bb;
                bb.min = vec2(fminf(a.p.x, fminf(b.p.x, c.p.x)), fminf(a.p.y, fminf(b.p.y, c.p.y)));
                bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, c.p.x)), fmaxf(a.p.y, fmaxf(b.p.y, c.p.y)));
                return bb;
            }

            bool in(position p)
            {
                return on_side(p, a.p, b.p) && on_side(p, b.p, c.p) && on_side(p, c.p, a.p);
            }
        };

        struct Quad : public PrimitiveI
        {
            Vertex a, b, c, d;

            Vertex interpolate(position p)
            {
                Vertex res;
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
                
                // rectangle
                if (k2 == 0)
                {
                    uv.x = (p.x - _a.x) / (_b.x - _a.x);
                    uv.y = (p.y - _a.y) / (_d.y - _a.y);
                }
                // any other quadrilateral
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
                
    //#define M_ABS(a) (((a) < 0) ? -(a) : (a))
    //
    //            {
    //
    //                vec2  vc1, vc2, vcp1, vcp2;
    //                float vA, vC, vB;
    //                float s, is, t, am2bpc, tdenom_x, tdenom_y;
    //
    //                vc1 = vec2(a.p.x - c.p.x, a.p.y - c.p.y);
    //                vc2 = vec2(b.p.x - d.p.x, b.p.y - d.p.y);
    //
    //                vcp1 = vec2(a.p.x - p.x, a.p.y - p.y);
    //                vcp2 = vec2(b.p.x - p.x, b.p.y - p.y);
    //
    //                vA = vcp1.x * vc1.y - vcp1.y * vc1.x;
    //                vC = vcp2.x * vc2.y - vcp2.y * vc2.x;
    //                vB = ((vcp1.x * vc2.y - vcp1.y * vc2.x) +
    //                    (vcp2.x * vc1.y - vcp2.y * vc1.x)) * 0.5f;
    //
    //                am2bpc = vA - 2.0f * vB + vC;
    //
    //                if (am2bpc > -0.0001f && am2bpc < 0.0001f)
    //                    s = vA / (vA - vC);
    //                else
    //                    s = ((vA - vB) + sqrtf(vB * vB - vA * vC)) / am2bpc;
    //
    //                is = 1.0f - s;
    //                tdenom_x = is * vc1.x + s * vc2.x;
    //                tdenom_y = is * vc1.y + s * vc2.y;
    //
    //                if (M_ABS(tdenom_x) > M_ABS(tdenom_y))
    //                    t = (is * vcp1.x + s * vcp2.x) / tdenom_x;
    //                else
    //                    t = (is * vcp1.y + s * vcp2.y) / tdenom_y;
    //
    //                uv = vec2(is, t);
    //            }
    //#undef M_ABS
                
                res.c = color(
                    (a.c.r*(1.0f - uv.x) + b.c.r*uv.x)*(1.0f - uv.y) + (d.c.r*(1.0f - uv.x) + c.c.r*uv.x)*uv.y,   // R
                    (a.c.g*(1.0f - uv.x) + b.c.g*uv.x)*(1.0f - uv.y) + (d.c.g*(1.0f - uv.x) + c.c.g*uv.x)*uv.y,   // G
                    (a.c.b*(1.0f - uv.x) + b.c.b*uv.x)*(1.0f - uv.y) + (d.c.b*(1.0f - uv.x) + c.c.b*uv.x)*uv.y,   // B
                    (a.c.a*(1.0f - uv.x) + b.c.a*uv.x)*(1.0f - uv.y) + (d.c.a*(1.0f - uv.x) + c.c.a*uv.x)*uv.y    // A
                );
                
                res.p = (a.p*(1.0f - uv.x) + b.p*uv.x)*(1.0f - uv.y) + (d.p*(1.0f - uv.x) + c.p*uv.x)*uv.y;
                
                return res;
            }

            AABB2 aabb(void)
            {
                AABB2 bb;
                bb.min = vec2(fminf(a.p.x, fminf(b.p.x, fminf(c.p.x, d.p.x))), fminf(a.p.y, fminf(b.p.y, fminf(c.p.y, d.p.y))));
                bb.max = vec2(fmaxf(a.p.x, fmaxf(b.p.x, fmaxf(c.p.x, d.p.x))), fmaxf(a.p.y, fmaxf(b.p.y, fmaxf(c.p.y, d.p.y))));
                return bb;
            }

            bool in(position p)
            {
                bool ab = !on_side(p, a.p, b.p);
                bool bc = !on_side(p, b.p, c.p);
                bool cd = !on_side(p, c.p, d.p);
                bool da = !on_side(p, d.p, a.p);
                return ab&&bc&&cd&&da;
            }
        };
    }
}