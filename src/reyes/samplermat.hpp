#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(SamplerMat)
        {
            struct uniform_tag
            {
                vec3 S;
                vec3 T;
                Sampler* sampler;
            } uniform;

            DISPLACE
            {
                return vertex.p * uniform.S + uniform.T;
            }

            SHADE
            {
                color c = uniform.sampler->sample(vertex.uv);
                return color(c.r*vertex.uv.x, c.g*vertex.uv.y, c.g, 1);
            }
        };
    }
}