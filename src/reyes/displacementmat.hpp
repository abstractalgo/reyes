#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        UNIFORM(DisplacementMat)
        {
            Sampler* sampler;
        };
        MATERIAL(DisplacementMat)
        {
            DISPLACE
            {
                color c = uniform.sampler->sample(vertex.uv);
                float k = 0.05f;
                vec3 n = vertex.n.normalize();
                vec3 p(vertex.p.x + n.x * k * c.r,
                    vertex.p.y + n.y * k * c.g,
                    vertex.p.z + n.z * k * c.b);
                return p;
            }

            SHADE
            {
                return uniform.sampler->sample(vertex.uv);
            }
        };
    }
}