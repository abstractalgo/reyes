#pragma once

#include <cstdint>
#include "color.hpp"

#define CROSS(a,b) vec3(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x)
#define DOT(a,b) (a.x*b.x+a.y*b.y+a.z*b.z)
#define edgeFn(a,b,c) ((c.x - a.p.x) * (b.p.y - a.p.y) - (c.y - a.p.y) * (b.p.x - a.p.x))

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
                Vertex _a = a;
                Vertex _b = b;
                Vertex _c = c;

                // adjust attributes
                _a.c.r /= a.p.z;
                _a.c.g /= a.p.z;
                _a.c.b /= a.p.z;

                _b.c.r /= b.p.z;
                _b.c.g /= b.p.z;
                _b.c.b /= b.p.z;

                _c.c.r /= c.p.z;
                _c.c.g /= c.p.z;
                _c.c.b /= c.p.z;

                float wa, wb, wc;
                float area = edgeFn(_a, _b, _c.p);
                wa = edgeFn(_b, _c, p) / area;
                wb = edgeFn(_c, _a, p) / area;
                wc = edgeFn(_a, _b, p) / area;
                res.p = p;
                float z = (wa * _a.p.z + wb*_b.p.z + wc*_c.p.z);
                res.p.z = z;

                res.c = color(
                    (_a.c.r*wa + _b.c.r*wb + _c.c.r*wc)*z,
                    (_a.c.g*wa + _b.c.g*wb + _c.c.g*wc)*z,
                    (_a.c.b*wa + _b.c.b*wb + _c.c.b*wc)*z,
                    1
                );


                //vec3 _a = { a.p.x, a.p.y, 1.0f };
                //vec3 _b = { b.p.x, b.p.y, 1.0f };
                //vec3 _c = { c.p.x, c.p.y, 1.0f };
                //p.z = 1.0f;
                //
                //vec3 bxc = CROSS(_b, _c);
                //vec3 axb = CROSS(_a, _b);
                //vec3 cxa = CROSS(_c, _a);
                //float denom = DOT(_a, bxc);
                //
                //float alpha = DOT(p, bxc) / denom;
                //float beta = DOT(p, cxa) / denom;
                //float gamma = DOT(p, axb) / denom;
                //
                //res.c = color(
                //    a.c.r*alpha + b.c.r*beta + c.c.r*gamma,   // R
                //    a.c.g*alpha + b.c.g*beta + c.c.g*gamma,   // G
                //    a.c.b*alpha + b.c.b*beta + c.c.b*gamma,   // B
                //    a.c.a*alpha + b.c.a*beta + c.c.a*gamma    // A
                //);
                //
                //res.p = position(
                //    a.p.x*alpha + b.p.x*beta + c.p.x*gamma, // X
                //    a.p.y*alpha + b.p.y*beta + c.p.y*gamma, // Y
                //    a.p.z*alpha + b.p.z*beta + c.p.z*gamma  // Z
                //);
                
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

                res.p = (a.p*(1.0f - uv.x) + b.p*uv.x)*(1.0f - uv.y) + (d.p*(1.0f - uv.x) + c.p*uv.x)*uv.y;

                // TODO: perspective correction
                res.c = color(
                    (a.c.r*(1.0f - uv.x) + b.c.r*uv.x)*(1.0f - uv.y) + (d.c.r*(1.0f - uv.x) + c.c.r*uv.x)*uv.y,   // R
                    (a.c.g*(1.0f - uv.x) + b.c.g*uv.x)*(1.0f - uv.y) + (d.c.g*(1.0f - uv.x) + c.c.g*uv.x)*uv.y,   // G
                    (a.c.b*(1.0f - uv.x) + b.c.b*uv.x)*(1.0f - uv.y) + (d.c.b*(1.0f - uv.x) + c.c.b*uv.x)*uv.y,   // B
                    (a.c.a*(1.0f - uv.x) + b.c.a*uv.x)*(1.0f - uv.y) + (d.c.a*(1.0f - uv.x) + c.c.a*uv.x)*uv.y    // A
                );

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