#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        // ------------------------------------------- Lambert
        UNIFORM(Lambert)
        {
            uint8_t lightCnt;
            Light** lights;
            color color;
            LambertUnifBlk()
                : lightCnt(0)
                , lights(0)
            {}
        };
        MATERIAL(Lambert)
        {
            DISPLACE
            {
            float k = sin(vertex.uv.x * M_PI * 20.0)*cos(vertex.uv.y * 40.0);
            vec3 p = vertex.p + vertex.n * k * 0.1f;
            return p;
        }

            SHADE
            {
                PosNormal pn(vertex.p, vertex.n);
                color total;
                for (uint8_t i = 0; i < uniform.lightCnt; i++)
                {
                    color lightcol = uniform.lights[i]->sample(pn);
                    total.r += lightcol.r*uniform.color.r;
                    total.g += lightcol.g*uniform.color.g;
                    total.b += lightcol.b*uniform.color.b;
                }
                total.r = total.r>1.0 ? 1.0f : total.r;
                total.g = total.g > 1.0 ? 1.0f : total.g;
                total.b = total.b > 1.0 ? 1.0f : total.b;
                return total;
            }
        };
    }
}