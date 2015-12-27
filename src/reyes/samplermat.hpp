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
                return uniform.sampler->sample(vertex.uv);
            }
        };
    }
}