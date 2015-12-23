#pragma once

#include "shading.hpp"

namespace reyes
{
    namespace lib
    {
        struct SinCosShader : public Sampler
        {
            color sample(uv uv)
            {
                float k = sin(uv.x * M_PI * 20.0)*cos(uv.y * M_PI* 20.0);
                return{ k, k, k, 1 };
            }
        };
    }
}