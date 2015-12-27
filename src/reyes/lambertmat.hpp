#pragma once

#include "shading.hpp"
#include "directionalight.hpp"

namespace reyes
{
    namespace lib
    {
        // ------------------------------------------- Lambert
        MATERIAL(Lambert)
        {
            UNIFORM_BEGIN
                DirectionalLight* light;
                Sampler* texture;
            UNIFORM_END

            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                PosNormal pn(vertex.p, vertex.n);
                color c = uniform.light->sample(pn);
                color a = uniform.texture->sample(vertex.uv);
                color total(c.r*a.r, c.g*a.g, c.b*a.b, 1);
                total.r = total.r>1.0 ? 1.0f : total.r;
                total.g = total.g > 1.0 ? 1.0f : total.g;
                total.b = total.b > 1.0 ? 1.0f : total.b;
                return total;
            }
        };
    }
}