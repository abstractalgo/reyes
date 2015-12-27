#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(DisplacementMat)
        {
            UNIFORM_BEGIN
                Sampler* sampler;
                float k;
                Uniform()
                    : sampler(0)
                    , k(0.05f)
                {}
            UNIFORM_END

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