#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        MATERIAL(SamplerMat)
        {
            UNIFORM_BEGIN
                Sampler* sampler;
            UNIFORM_END
                
            DISPLACE
            {
                return vertex.p;
            }

            SHADE
            {
                color c = uniform.sampler->sample(vertex.uv);
                return color(c.r*vertex.uv.x, c.g*vertex.uv.y, c.g, 1);
            }
        };
    }
}