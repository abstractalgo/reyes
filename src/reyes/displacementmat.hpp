#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        UNIFORM(DisplacementMat)
        {
            Sampler* sampler;
            float k;
            DisplacementMatUnifBlk()
                : sampler(0)
                , k(0.05f)
            {}
        };
        MATERIAL(DisplacementMat)
        {
            DISPLACE
            {
                color c = uniform.sampler->sample(vertex.uv);
                vec3 n = vertex.n.normalize();
                vec3 p(vertex.p.x + n.x * uniform.k * c.r,
                    vertex.p.y + n.y * uniform.k * c.g,
                    vertex.p.z + n.z * uniform.k * c.b);
                return p;
            }

            SHADE
            {
                return uniform.sampler->sample(vertex.uv);
            }
        };
    }
}