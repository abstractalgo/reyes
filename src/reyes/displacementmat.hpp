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
                vec3 S;
                vec3 T;
                Uniform()
                    : sampler(0)
                    , k(0.05f)
                {}
            UNIFORM_END

            DISPLACE
            {
                color c = uniform.sampler->sample(vertex.uv);
                vec3 n = vertex.n.normalize();
                vec3 p = vertex.p * uniform.S + uniform.T;
                p.x += n.x * uniform.k * c.r;
                p.y += n.y * uniform.k * c.g;
                p.z += n.z * uniform.k * c.b;
                return p;
            }

            SHADE
            {
                return uniform.sampler->sample(vertex.uv);
            }
        };
    }
}